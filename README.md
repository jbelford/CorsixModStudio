# CorsixModStudio — Modernization Project

## What is CorsixModStudio?

CorsixModStudio (CDMS) is a modding IDE for Relic Entertainment's games — primarily **Dawn of War** (and its expansions Dark Crusade, Soulstorm, Winter Assault) and **Company of Heroes**. Originally written by [Corsix](mailto:corsix@gmail.com) around 2006–2007, it provides a comprehensive suite of tools for creating and editing game mods:

- **Mod management** — create, load, and configure game mods with dependency tracking
- **File editors** — visual editors for RGD (game data), UCS (localization strings), SCAR (scripts), and RGM (materials)
- **Archive tools** — read, extract, and create SGA archives (Relic's proprietary game archive format)
- **Lua integration** — inheritance tree viewer, script editor, and Lua runtime for data manipulation
- **Build pipeline** — compile and package mods into distributable SGA archives

The software was released as open source under the **GNU GPL v2** (CDMS app) and **GNU LGPL v2.1** (Rainman library).

## Why Modernize?

The original codebase targets technology from 2006:

| Component | Original | Modern Target |
|-----------|----------|---------------|
| C++ Standard | Pre-C++11 (VS 2003/2005 era) | C++20 |
| wxWidgets | 2.8.0 | 3.2+ |
| Lua | 5.0.2 + 5.1.2 | Vendored 5.0.2 + vendored 5.1.2 (renamed symbols) |
| zLib | Bundled ancient copy | vcpkg 1.3.1 |
| Build System | Manual VS project (COMPILE.txt) | CMake 4.1 + vcpkg |
| Compiler | MSVC 2003/2005 | MSVC 2026 (v19.50) |
| Testing | None | Google Test |

The software no longer compiles on modern compilers and exhibits bugs on current Windows versions. This project brings it back to life.

## Architecture

The codebase has three layers totaling ~40,000 lines of C/C++:

### Rainman Library (~25,700 lines)
The core I/O library for Relic game file formats. No UI dependencies — pure data parsing and manipulation. Organized into subdirectories: `core/`, `io/`, `archive/`, `formats/`, `lua/`, `localization/`, `module/`, `util/`.

**Key classes:**
- `CSgaFile` — SGA archive reader (v2, v4) with zLib decompression
- `CRgdFile` — Binary game data (RGD) with Lua integration
- `CUcsFile` — Unicode localization string tables
- `CChunkyFile` — Relic's chunked container format
- `CModuleFile` — Game mod descriptor (.module files)
- `CRgtFile` — Texture files (DXT1/3/5, DDS, TGA)
- `CSgaCreator` — SGA archive writer
- `IFileStore` / `CMemoryStore` / `CFileSystemStore` — Polymorphic I/O abstraction
- `CInheritTable` — Lua-based data inheritance resolution

### CDMS Service Layer
An intermediate layer between Rainman and the GUI, providing error-safe wrappers and concurrency:
- `services/` — `ModuleService`, `FileService`, `FormatService`, `HashService` wrap Rainman calls with `Result<T>` error handling
- `async/` — `CTaskRunner` / `CWxTaskRunner` for background work, `CCancellationToken`, `CProgressChannel`

### CDMS GUI Application (~14,200 lines)
The wxWidgets 3.2 GUI application built on top of Rainman via the service layer.

**Key frames:**
- `ConstructFrame` — Main window with splitter, AUI notebook tabs, and file tree
- `frmFiles` — File browser with tree view of mod structure
- `frmRgdEditor` — Property grid editor for RGD game data files
- `frmScarEditor` — SCAR script editor (wxStyledTextCtrl)
- `frmUCSEditor` — Unicode string table editor
- `frmSgaMake` — SGA archive creation wizard
- `frmModule` — Mod settings editor (dependencies, data folders, archives)

## Project Structure

```
CorsixModStudio/
├── CMakeLists.txt                  # Top-level CMake build
├── vcpkg.json                      # Dependency manifest (zlib, gtest, libsquish, wxwidgets)
├── src/
│   ├── rainman/                    # Rainman library (static lib)
│   │   ├── CMakeLists.txt
│   │   ├── include/rainman/        # Public umbrella header
│   │   ├── core/                   # Exceptions, logging (spdlog), callbacks, API macros
│   │   ├── io/                     # IFileStore, CMemoryStore, CFileSystemStore
│   │   ├── archive/                # CSgaFile, CSgaCreator
│   │   ├── formats/                # CRgdFile, CChunkyFile, CRgtFile, CRgmFile, CBfxFile
│   │   ├── lua/                    # CLuaFile, CInheritTable, CLuaScript
│   │   ├── localization/           # CUcsFile, CCohUcsFile
│   │   ├── module/                 # CModuleFile, CDoWModule, CDoWFileView
│   │   ├── util/                   # CRC32, hash, MD5
│   │   └── vendor/                 # Vendored Lua 5.0.2 + 5.1.2 (do not modify)
│   └── cdms/                       # CDMS GUI application (wxWidgets 3.2)
│       ├── CMakeLists.txt
│       ├── services/               # Result<T>, ModuleService, FileService, etc.
│       ├── async/                  # CTaskRunner, CWxTaskRunner, CCancellationToken
│       ├── actions/                # File-type action handlers
│       ├── res/                    # Icons and bitmaps
│       └── *.cpp, *.h              # GUI frames, tabs, menus
├── tests/
│   ├── rainman/                    # Rainman unit tests (Google Test)
│   │   ├── CMakeLists.txt
│   │   └── *_test.cpp
│   └── cdms/                       # CDMS unit tests (Google Test)
│       ├── CMakeLists.txt
│       └── *_test.cpp
├── Mod_Studio_Files/               # Runtime data files (game definitions, templates)
└── CDMSSrc_055/                    # Original source archives (reference)
```

## Building

### Prerequisites
- **Visual Studio 2022 or later** (tested with VS 2026 Enterprise)
- **vcpkg** — `git clone https://github.com/microsoft/vcpkg.git C:\vcpkg && C:\vcpkg\bootstrap-vcpkg.bat`
- **CMake 3.20+** (bundled with Visual Studio)

### Build Commands
```powershell
# Set environment
$env:VCPKG_ROOT = "C:\vcpkg"

# Configure + Build + Test (uses CMake presets)
cmake --preset default
cmake --build --preset debug
ctest --preset debug

# Release build
cmake --build --preset release

# Build with clang-tidy analysis (in Visual Studio)
cmake --preset tidy
cmake --build --preset tidy-debug
```

### Dependencies (auto-installed by vcpkg)
- **zlib 1.3.1** — compression for SGA archives
- **Google Test 1.17** — unit testing framework
- **libsquish** — DXT texture compression (for CRgtFile)
- **wxWidgets 3.x** — GUI framework
- **spdlog** — structured logging

## Progress

### ✅ Phase 1: Project Scaffolding — COMPLETE
- CMake build system with C++20, vcpkg integration
- vcpkg manifest for dependency management
- Modern directory layout (`src/`, `tests/`)

### ✅ Phase 2: Rainman Library Compilation — COMPLETE
- All 39 source files ported to compile on MSVC 2026 / C++20
- Key fixes applied:
  - Replaced `__asm nop` (x86 only) with `__nop()` intrinsic (x64 compatible)
  - Extracted `crc32_case_idt()` from bundled zlib into standalone implementation
  - Fixed hardcoded Lua 5.1.2 include path (`e:\CPP\lua-5.1.2\src\luaconf.h` → local)
  - Switched 7 files from bundled `zLib/zlib.h` to system `<zlib.h>`
  - Disabled unused `CLuaScript_Interface.cpp` (all call sites already commented out)
  - Updated `Rainman.h` umbrella header to use direct includes (no `../` paths)
  - Set `RAINMAN_NO_EXPORTS` (static lib, no `__declspec(dllimport/export)` needed)
- Vendored Lua 5.0.2 builds as static library (26 C files + patched ltm/lvm)
- Replaced runtime-loaded Lua 5.1.2 DLL with statically linked `lua512` library (symbols renamed via `lua512_rename.h` to avoid conflicts with Lua 5.0.2)
- Replaced runtime-loaded `squish.dll` with statically linked libsquish (via vcpkg)
- **Result: 0 errors, warnings only (size_t → unsigned long narrowing, expected for 32→64-bit port)**

### ✅ Phase 3: Rainman Tests — COMPLETE
- Google Test framework wired up with `gtest_discover_tests` + `ctest -j` parallelism
- Test suites covering: `CRainmanException`, `CMemoryStore`, `Crc32CaseIdt`, `CFileSystemStore`, `CSgaFile`, `CRgdFile`, `CUcsFile`, `CChunkyFile`, `CModuleFile`, `CRgdHashTable`, `CInheritTable`, `CCallbacks`, `Internal_Util`, `Md5`, `WriteTime`

### ✅ Phase 4: CDMS GUI Compilation — COMPLETE
- All 65+ CDMS source files copied and building under CMake
- Ported wxWidgets 2.8 → 3.2 (`wxPGId` → `wxPGProperty*`, `wxFileConfig` constructor, `TRUE` → `true`)
- Fixed C++20 issues, Windows resource file (`resource.rc`) with application icon
- Added wxWidgets, libsquish, and spdlog to vcpkg dependencies

### ✅ Phase 5: CDMS GUI Tests — COMPLETE
- Google Test infrastructure for CDMS non-UI logic
- Test suites covering: `strconv`, `config`, `strings`, `Utility`, `Result<T>`, `ModuleService`, `FileService`, `FormatService`, `HashService`, `CCancellationToken`, `CProgressChannel`, `CTaskRunner`

### ⬜ Phase 6: Integration Testing — NOT STARTED
- Full end-to-end build, app launch verification, .module file loading

## Key Technical Decisions

### Lua Strategy
The original code uses two Lua versions for different purposes:
- **Lua 5.0.2** — compiled and linked statically for core game data parsing (RGD files use Lua internally)
- **Lua 5.1.2** — now statically linked (originally runtime-loaded via `LoadLibraryW`). All public symbols are renamed via `lua512_rename.h` (`lua_` → `lua51_`) to avoid linker conflicts with Lua 5.0.2.

We vendor both Lua versions' source directly (including Corsix's patches to `ltm.c/h` and `lvm.c/h` in 5.0.2).

### zLib Strategy
The original bundled a modified zlib with a custom `crc32_case_idt()` function for case-insensitive file path hashing (essential for Windows file system compatibility in Relic's formats). We now use system zlib from vcpkg and extracted `crc32_case_idt()` into a standalone file with its own CRC table.

### Static vs Dynamic Library
Rainman was originally a DLL (`__declspec(dllexport/import)`). We build it as a **static library** for simplicity — the only consumer is the CDMS executable. The `RAINMAN_NO_EXPORTS` define makes `RAINMAN_API` expand to nothing.

## Contributing

### Conventions

- **Naming**: `C` prefix for concrete classes, `I` for interfaces, `V` for virtual methods, `m_` with Hungarian prefixes for members. CDMS window classes use `frm` prefix (`frmRgdEditor`), event IDs use `ID_` prefix.
- **Error handling**: Stack-based exceptions (`throw CRainmanException(...)`, catch by `const CRainmanException &`). In the service layer, convert to `Result<T>` via `ResultFromException()`.
- **Include order**: CDMS `.cpp` files include `Common.h` first, then Rainman headers (with fully-qualified `rainman/` prefix).
- **Concurrency**: Use `CThreadPool::Instance().Submit()` (`rainman/core/CThreadPool.h`) instead of `std::async`.
- **Stream ownership**: `VOpenStream()` / `VOpenOutputStream()` return caller-owned heap-allocated streams — the caller must `delete` them.
- **Source discovery**: Production sources are auto-discovered via `file(GLOB)`. Test files must be explicitly listed in their `CMakeLists.txt`.

### Modern C++ — Boy-Scout Rule

This codebase follows a **boy-scout modernization** approach: when touching existing code, improve it toward modern C++20 if the change is safe and localized. Examples:
- `NULL` → `nullptr`
- Add `override` to virtual method overrides
- Replace raw `new[]`/`delete[]` with `std::vector` or `std::unique_ptr` (internal code only)
- C-style casts → `static_cast`/`reinterpret_cast`
- Index-based loops → range-based `for`
- Add `const` to parameters and methods that don't mutate

**Preserve** at API boundaries: `unsigned long` in existing virtual signatures, heap-allocated exceptions (`throw new CRainmanException`), `RAINMAN_API` macro, `C`/`I`/`V`/`m_` naming conventions, include guards, LGPL headers, and vendored Lua code.

For detailed AI-assisted development guidelines, see [`.github/copilot-instructions.md`](.github/copilot-instructions.md).

## License
- **CDMS Application**: GNU General Public License v2.0
- **Rainman Library**: GNU Lesser General Public License v2.1
- **Lua 5.0.2**: MIT License
