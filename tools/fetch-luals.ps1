<#
.SYNOPSIS
    Downloads the Lua Language Server (LuaLS) binary from GitHub releases.

.DESCRIPTION
    Fetches the latest (or specified) LuaLS release for Windows x64 and
    extracts it to Mod_Studio_Files/lsp/bin/. Skips download if the binary
    already exists at the expected version.

.PARAMETER Version
    LuaLS release tag (e.g., "3.13.6"). Defaults to "latest".

.EXAMPLE
    .\tools\fetch-luals.ps1
    .\tools\fetch-luals.ps1 -Version 3.13.6
#>
param(
    [string]$Version = "latest"
)

$ErrorActionPreference = "Stop"

$ProjectRoot = Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path)
$OutputDir = Join-Path $ProjectRoot "Mod_Studio_Files\lsp\bin"
$ExePath = Join-Path $OutputDir "lua-language-server.exe"

# Check if already downloaded
if (Test-Path $ExePath) {
    Write-Host "LuaLS already exists at: $ExePath"
    Write-Host "Delete the bin/ directory to force re-download."
    return
}

# Determine download URL
$BaseUrl = "https://github.com/LuaLS/lua-language-server/releases"
if ($Version -eq "latest") {
    $ApiUrl = "https://api.github.com/repos/LuaLS/lua-language-server/releases/latest"
    Write-Host "Fetching latest LuaLS release info..."
    $Release = Invoke-RestMethod -Uri $ApiUrl -Headers @{ "User-Agent" = "CorsixModStudio" }
    $Version = $Release.tag_name
    Write-Host "Latest version: $Version"
}

$AssetName = "lua-language-server-$Version-win32-x64.zip"
$DownloadUrl = "$BaseUrl/download/$Version/$AssetName"
$ZipPath = Join-Path $env:TEMP $AssetName

Write-Host "Downloading: $DownloadUrl"
Invoke-WebRequest -Uri $DownloadUrl -OutFile $ZipPath -UseBasicParsing

# Extract
Write-Host "Extracting to: $OutputDir"
New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
Expand-Archive -Path $ZipPath -DestinationPath $OutputDir -Force

# Verify
if (Test-Path $ExePath) {
    Write-Host "LuaLS installed successfully at: $ExePath"
} else {
    # Some releases use a nested directory structure
    $Nested = Get-ChildItem -Path $OutputDir -Recurse -Filter "lua-language-server.exe" | Select-Object -First 1
    if ($Nested) {
        Write-Host "LuaLS found at: $($Nested.FullName)"
    } else {
        Write-Error "Failed to find lua-language-server.exe after extraction"
    }
}

# Cleanup
Remove-Item $ZipPath -Force -ErrorAction SilentlyContinue
Write-Host "Done."
