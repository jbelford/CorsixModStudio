<#
.SYNOPSIS
    Run clang-tidy on CorsixModStudio source files using compile_commands.json.

.DESCRIPTION
    Configures a Ninja build (tidy-ninja preset) to generate compile_commands.json,
    then runs clang-tidy on the specified files (or all non-vendor sources).

.PARAMETER Files
    Optional list of source files to analyse. Defaults to all non-vendor .cpp files
    under src/rainman/ and src/cdms/.

.PARAMETER Fix
    Pass --fix to clang-tidy to auto-apply fixes.

.EXAMPLE
    .\tools\run-clang-tidy.ps1
    .\tools\run-clang-tidy.ps1 src/rainman/core/Exception.cpp
    .\tools\run-clang-tidy.ps1 -Fix
#>
[CmdletBinding()]
param(
    [Parameter(Position = 0, ValueFromRemainingArguments)]
    [string[]]$Files,

    [switch]$Fix,

    [string]$LogFile
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'
$repoRoot = (Resolve-Path "$PSScriptRoot\..").Path

# --- Locate clang-tidy -----------------------------------------------------------
$clangTidy = Get-Command clang-tidy -ErrorAction SilentlyContinue |
             Select-Object -ExpandProperty Source
if (-not $clangTidy) {
    $vsWhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $vsWhere) {
        $vsPath = & $vsWhere -latest -property installationPath
        $clangTidy = Get-ChildItem "$vsPath\VC\Tools\Llvm\x64\bin\clang-tidy.exe" -ErrorAction SilentlyContinue |
                     Select-Object -First 1 -ExpandProperty FullName
    }
}
if (-not $clangTidy) {
    Write-Error "clang-tidy not found. Install the C++ Clang tools via the VS Installer."
    exit 1
}
Write-Host "Using: $clangTidy" -ForegroundColor DarkGray

# --- Ensure compile_commands.json exists via tidy-ninja preset --------------------
$buildDir = "$repoRoot\build\tidy-ninja"
$ccj = "$buildDir\compile_commands.json"

if (-not (Test-Path $ccj)) {
    Write-Host "Generating compile_commands.json (cmake --preset tidy-ninja)..." -ForegroundColor Cyan

    # tidy-ninja uses Ninja which needs MSVC env. Run vcvarsall + cmake in one
    # cmd session so all environment variables (PATH, INCLUDE, LIB, etc.) are set.
    $vsWherePath = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    $vsInstall = & $vsWherePath -latest -property installationPath
    $vcvarsall = "$vsInstall\VC\Auxiliary\Build\vcvarsall.bat"
    $ninja = "$vsInstall\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"

    Push-Location $repoRoot
    cmd /c "`"$vcvarsall`" x64 && cmake --preset tidy-ninja -DCMAKE_MAKE_PROGRAM=`"$ninja`"" 2>&1 |
        ForEach-Object { Write-Host $_ }
    Pop-Location

    if (-not (Test-Path $ccj)) {
        Write-Error "Failed to generate compile_commands.json"
        exit 1
    }
}

# --- Collect source files ---------------------------------------------------------
if (-not $Files -or $Files.Count -eq 0) {
    $src = "$repoRoot\src"
    $collected = @()
    $collected += Get-ChildItem "$src\rainman" -Recurse -Filter "*.cpp" |
                  Where-Object { $_.FullName -notmatch "vendor" }
    $collected += Get-ChildItem "$src\cdms" -Recurse -Filter "*.cpp" -ErrorAction SilentlyContinue
    $Files = $collected | ForEach-Object { $_.FullName }
}
else {
    # Resolve relative paths
    $Files = $Files | ForEach-Object { (Resolve-Path $_).Path }
}

Write-Host "Analysing $($Files.Count) file(s)..." -ForegroundColor Cyan

# --- Run clang-tidy ---------------------------------------------------------------
$fixArg = if ($Fix) { "--fix" } else { $null }
$allWarnings = [System.Collections.Concurrent.ConcurrentBag[string]]::new()

$Files | ForEach-Object -Parallel {
    $ct       = $using:clangTidy
    $bd       = $using:buildDir
    $fa       = $using:fixArg
    $bag      = $using:allWarnings

    $args = @($_, "-p", $bd, "--extra-arg=/Y-")
    if ($fa) { $args += $fa }

    $out = & $ct @args 2>&1
    foreach ($line in $out) {
        if ($line -match "warning:.*\[" -and $line -notmatch "non-user code|Suppressed") {
            $bag.Add($line)
        }
    }
} -ThrottleLimit ([Environment]::ProcessorCount)

# --- Summary ----------------------------------------------------------------------
$warnings = @($allWarnings)

if ($LogFile) {
    $warnings | Sort-Object | Set-Content $LogFile
    Write-Host "Wrote $($warnings.Count) warning(s) to $LogFile" -ForegroundColor DarkGray
}

Write-Host "`n=== $($warnings.Count) warning(s) ===" -ForegroundColor Yellow

if ($warnings.Count -gt 0) {
    $warnings | ForEach-Object {
        if ($_ -match '\[([^\]]+)\]\s*$') { $Matches[1] } else { "unknown" }
    } | Group-Object | Sort-Object Count -Descending | ForEach-Object {
        Write-Host ("  {0,4} x {1}" -f $_.Count, $_.Name)
    }
}

exit ($warnings.Count -gt 0 ? 1 : 0)
