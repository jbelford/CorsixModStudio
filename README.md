# Corsix's Mod Studio: Definitive Edition

## What is Corsix's Mod Studio?

Corsix's Mod Studio: Definitive Edition (CDMS) is a modding IDE for Relic Entertainment's games — primarily **Dawn of War** (and its expansions Dark Crusade, Soulstorm, Winter Assault) and **Company of Heroes**. Originally written by [Corsix](mailto:corsix@gmail.com) around 2006–2007, this Definitive Edition is a ground-up modernization of the original codebase, bringing it to C++20, 64-bit, and modern tooling while preserving full compatibility with Relic's game file formats.

### Features

- **Mod management** — create, load, and configure game mods with dependency tracking
- **File editors** — visual editors for RGD (game data), UCS (localization strings), SCAR (scripts), RGM (materials), and NIS (cutscene scripts)
- **SCAR editor with Lua Language Server** — real-time diagnostics, autocomplete, hover-to-inspect, go-to-definition, and find-in-file powered by [LuaLS](https://github.com/LuaLS/lua-language-server), with SCAR API stubs for full engine function coverage
- **Archive tools** — read, extract, and create SGA archives (Relic's proprietary game archive format) with parallel compression
- **Lua integration** — inheritance tree viewer, script editor, and Lua runtime for data manipulation
- **Build pipeline** — compile and package mods into distributable SGA archives
- **Async operations** — background module loading with progress reporting and cancellation
- **DPI-aware SVG icons** — scalable, color-coded file type icons using the Lucide icon set

The software was released as open source under the **GNU GPL v2** (CDMS app) and **GNU LGPL v2.1** (Rainman library).

## Why Modernize?

The original codebase targets technology from 2006:

| Component         | Original                        | Definitive Edition                                        |
| ----------------- | ------------------------------- | --------------------------------------------------------- |
| C++ Standard      | Pre-C++11 (VS 2003/2005 era)    | C++20                                                     |
| Architecture      | 32-bit (x86)                    | 64-bit (x64)                                              |
| wxWidgets         | 2.8.0                           | 3.2+                                                      |
| Lua               | 5.0.2 + 5.1.2 (DLL)             | Vendored 5.0.2 + vendored 5.1.2 (static, renamed symbols) |
| zLib              | Bundled ancient copy            | vcpkg 1.3.1                                               |
| libsquish         | Runtime-loaded DLL              | Static link via vcpkg                                     |
| Build System      | Manual VS project (COMPILE.txt) | CMake 4.1 + vcpkg + presets                               |
| Compiler          | MSVC 2003/2005                  | MSVC 2026 (v19.50)                                        |
| Testing           | None                            | Google Test (635 tests)                                   |
| Static Analysis   | None                            | clang-tidy                                                |
| Memory Management | Raw `new`/`delete` throughout   | `std::unique_ptr`, `std::shared_ptr`, `std::string`       |
| Concurrency       | Single-threaded                 | Thread pool + async task runners                          |
| Logging           | None                            | spdlog (structured logging)                               |
| Script Editor     | Basic syntax highlighting       | Full LSP integration (LuaLS)                              |

The software no longer compiled on modern compilers and exhibited bugs on current Windows versions. This project brings it back to life — and significantly beyond the original.

## Architecture

The codebase has three layers

### Rainman Library (static lib)

The core I/O library for Relic game file formats. No UI dependencies — pure data parsing and manipulation. Organized into subdirectories: `core/`, `io/`, `archive/`, `formats/`, `lua/`, `localization/`, `module/`, `util/`.

**Key classes:**

- `CSgaFile` — SGA archive reader (v2, v4) with zLib decompression
- `CSgaCreator` — SGA archive writer with parallel compression via thread pool
- `CRgdFile` — Binary game data (RGD) with Lua integration
- `CUcsFile` — Unicode localization string tables with `CUcsMap` (dual ordered/unordered map)
- `CChunkyFile` — Relic's chunked container format
- `CModuleFile` — Game mod descriptor (.module files)
- `CResourceLoader` — Parallel mod resource loading with deferred error propagation
- `CRgtFile` — Texture files (DXT1/3/5, DDS, TGA)
- `IFileStore` / `CMemoryStore` / `CFileSystemStore` — Polymorphic I/O abstraction
- `CInheritTable` — Lua-based data inheritance resolution
- `CThreadPool` — Process-wide thread pool singleton (`std::thread::hardware_concurrency()` workers)

### CDMS Service Layer

An intermediate layer between Rainman and the GUI, providing error-safe wrappers and concurrency:

- `services/` — `ModuleService`, `FileService`, `FormatService`, `HashService` wrap Rainman calls with `Result<T>` error handling
- `async/` — `CTaskRunner` (pure C++) / `CWxTaskRunner` (posts results to main thread via `CallAfter`) for background work, `CCancellationToken` for cooperative cancellation, `CProgressChannel` with Rainman callback bridge

### CDMS GUI Application (wxWidgets 3.2)

The GUI application uses a **Model-View-Presenter (MVP)** architecture with 14 presenters and 16 view interfaces for separation of concerns and unit testability.

**Presenters** (`src/cdms/presenters/`):
`CModuleLoadPresenter`, `CFileTreePresenter`, `CScarEditorPresenter`, `CRgdEditorPresenter`, `CRgdMacroPresenter`, `CUcsEditorPresenter`, `CSgaMakePresenter`, `CLuaInheritTreePresenter`, `CModuleSettingsPresenter`, `CMassExtractPresenter`, `CImagePresenter`, `CRefreshFilesPresenter`, `CUcsToDatPresenter`, `CDpsCalculatorPresenter`

**View interfaces** (`src/cdms/views/interfaces/`):
`IMainFrameView`, `IFileTreeView`, `IScarEditorView`, `IRgdEditorView`, `IRgdMacroView`, `IUcsEditorView`, `ISgaMakeView`, `ILuaInheritTreeView`, `IModuleSettingsView`, `IMassExtractView`, `IImageView`, `IRefreshFilesView`, `IUcsToDatView`, `IDpsCalculatorView`, `ISaveable`, `ISearchable`

**Key views:**

- `ConstructFrame` — Main window with splitter, AUI notebook tabs, and file tree
- `frmFiles` — File browser with tree view, SVG icons, and expand/collapse navigation
- `frmScarEditor` — SCAR script editor with LSP integration, FindBar, hover popup, and go-to-definition
- `frmRgdEditor` — Property grid editor for RGD game data files
- `frmUCSEditor` — Unicode string table editor
- `frmSgaMake` — SGA archive creation wizard
- `frmModule` — Mod settings editor (dependencies, data folders, archives)
- `frmLoading` — Async loading dialog with progress bar and cancel button

### Lua Language Server Integration

The SCAR editor integrates [LuaLS](https://github.com/LuaLS/lua-language-server) (lua-language-server) for a modern editing experience:

- **Real-time diagnostics** — errors, warnings, and hints displayed inline with squiggly underlines
- **Autocomplete** — context-aware completions for SCAR engine functions and mod code
- **Hover-to-inspect** — rich popup with syntax-highlighted signatures, markdown descriptions, and merged diagnostics
- **Go to Definition** — Ctrl+Click or F12 to jump to function/variable definitions
- **Find in File** — VS Code-style Ctrl+F bar with match highlighting, count, and navigation
- **SCAR API stubs** — auto-generated type definitions for ~500+ Dawn of War and ~1000+ Company of Heroes engine functions (from ScarDoc HTML and scardoc.dat)
- **`--?` annotation translation** — Relic's `--?` doc comments are translated on-the-fly to LuaLS-compatible `---@param`/`---@return` annotations
- **Status bar indicator** — real-time LSP status (Idle → Starting → Loading → Ready) with colored dot
- **Toggle on/off** — View menu option to enable/disable the language server

See [`docs/lua-language-server-integration.md`](docs/lua-language-server-integration.md) for detailed documentation.

## Project Structure

```
CorsixModStudio/
├── CMakeLists.txt                  # Top-level CMake build
├── CMakePresets.json               # Build presets (default, tidy, debug, release)
├── vcpkg.json                      # Dependency manifest
├── triplets/                       # Custom vcpkg triplets (static CRT, wxWidgets fixes)
├── src/
│   ├── rainman/                    # Rainman library (static lib)
│   │   ├── CMakeLists.txt
│   │   ├── core/                   # Exceptions, logging (spdlog), callbacks, API macros, CThreadPool
│   │   ├── io/                     # IFileStore, CMemoryStore, CFileSystemStore, StreamGuard
│   │   ├── archive/                # CSgaFile, CSgaCreator (parallel compression)
│   │   ├── formats/                # CRgdFile, CChunkyFile, CRgtFile, CRgmFile, CBfxFile
│   │   ├── lua/                    # CLuaFile, CInheritTable, CLuaScript, CLuaFileCache
│   │   ├── localization/           # CUcsFile, CCohUcsFile, CUcsMap, CUcsTransaction
│   │   ├── module/                 # CModuleFile, CDoWModule, CDoWFileView, CResourceLoader
│   │   ├── util/                   # CRC32, hash, MD5
│   │   └── vendor/                 # Vendored Lua 5.0.2 + 5.1.2 (do not modify)
│   └── cdms/                       # CDMS GUI application (wxWidgets 3.2)
│       ├── CMakeLists.txt
│       ├── presenters/             # 14 MVP presenters (business logic)
│       ├── views/                  # wxWidgets frames + view interfaces
│       │   ├── interfaces/         # 16 I*View pure-virtual interfaces
│       │   ├── frm*.cpp/h          # Concrete wxWidgets frames
│       │   ├── FindBar.cpp/h       # VS Code-style find-in-file
│       │   ├── CHoverPopup.cpp/h   # Rich hover popup with syntax highlighting
│       │   └── CLspStatusPanel.*   # LSP status bar indicator
│       ├── services/               # Result<T>, ModuleService, FileService, etc.
│       ├── async/                  # CTaskRunner, CWxTaskRunner, CCancellationToken
│       ├── actions/                # File-type action handlers (incl. NIS support)
│       ├── res/                    # SVG icons (Lucide, DPI-aware)
│       ├── frame/                  # Main window, AUI notebook management
│       ├── common/                 # Common.h, strconv, config, strings
│       └── tools/                  # ITool interface and tool implementations
├── tests/
│   ├── rainman/                    # Rainman unit tests (Google Test)
│   └── cdms/                       # CDMS unit tests (Google Test, incl. presenter mocks)
├── tools/
│   ├── run-clang-tidy.ps1          # Static analysis runner
│   ├── scar-to-luadefs.py          # SCAR → LuaLS stub generator
│   ├── scardoc-to-luadefs.py       # ScarDoc HTML → LuaLS stub generator
│   └── git-hooks/                  # Pre-commit / post-commit hooks
├── docs/                           # Architecture docs, file format specs, LSP integration
├── Mod_Studio_Files/               # Runtime data files (game definitions, templates)
└── CDMSSrc_055/                    # Original source code (read-only reference)
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

# Release build (LTO, static CRT)
cmake --build --preset release

# Static analysis with clang-tidy
cmake --preset tidy
cmake --build --preset tidy-debug

# Run clang-tidy manually
.\tools\run-clang-tidy.ps1                                    # all sources
.\tools\run-clang-tidy.ps1 src/rainman/archive/CSgaFile.cpp   # single file
.\tools\run-clang-tidy.ps1 -Fix                               # auto-fix
```

### Dependencies (auto-installed by vcpkg)

- **zlib 1.3.1** — compression for SGA archives
- **Google Test 1.17** — unit testing framework
- **libsquish** — DXT texture compression (for CRgtFile)
- **wxWidgets 3.x** — GUI framework
- **spdlog** — structured logging

### Release Build Optimizations

The release configuration enables link-time optimization (`/GL` + `/LTCG`), global data optimization (`/Gw`), dead-code elimination (`/OPT:REF,ICF`), and static CRT linking (`/MT`) via a custom vcpkg overlay triplet.

## What's New in the Definitive Edition

A summary of major improvements over the original 2006 codebase:

### Modern C++ & 64-bit

- Full C++20 compilation with MSVC 2026 (x64)
- Smart pointer refactoring: 19 classes converted from raw `new`/`delete`/`strdup`/`free` to `std::unique_ptr`, `std::shared_ptr`, `std::string`
- Stack-based exception handling (no heap-allocated exceptions)
- Thread pool (`CThreadPool`) replaces scattered `std::async` calls
- Parallel SGA archive compression and mod resource loading

### Lua Language Server (LuaLS) for SCAR Editing

- Real-time diagnostics, autocomplete, hover-to-inspect, and go-to-definition
- Auto-generated SCAR API stubs covering ~500 (DoW) and ~1000+ (CoH) engine functions
- On-the-fly translation of Relic's `--?` doc comments to LuaLS `---@` annotations
- Status bar indicator and View menu toggle

### Model-View-Presenter Architecture

- 14 presenters and 16 view interfaces decouple business logic from wxWidgets
- Presenters are unit-testable with mock view implementations
- Async task runners (`CTaskRunner` / `CWxTaskRunner`) with cancellation and progress

### Editor Improvements

- Find-in-file (Ctrl+F) with match highlighting, count display, and keyboard navigation
- Rich hover popup with syntax-highlighted signatures and markdown descriptions
- NIS (Non-Interactive Sequence / cutscene script) file type support
- Improved file tree with expand/collapse navigation and default file icons

### DPI-Aware SVG Icon System

- Lucide icon set (MIT license) rendered via `wxBitmapBundle::FromSVG()`
- Color-coded by file type: scripts, game data, textures, models, cutscenes
- Scales cleanly at any DPI

### Build & Quality

- CMake presets with vcpkg integration (one-command build)
- 635 unit tests across Rainman and CDMS layers
- clang-tidy static analysis with `run-clang-tidy.ps1` script
- Release build with LTO, static CRT, and dead-code elimination
- spdlog structured logging throughout

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
- **MVP pattern**: New editors should follow the presenter/view-interface pattern. Presenters depend on `I*View` interfaces, not concrete frames.
- **Git**: Never use `git commit --amend`. Always create new commits.

### Modern C++ — Boy-Scout Rule

This codebase follows a **boy-scout modernization** approach: when touching existing code, improve it toward modern C++20 if the change is safe and localized. Examples:

- `NULL` → `nullptr`
- Add `override` to virtual method overrides
- Replace raw `new[]`/`delete[]` with `std::vector` or `std::unique_ptr` (internal code only)
- C-style casts → `static_cast`/`reinterpret_cast`
- Index-based loops → range-based `for`
- Add `const` to parameters and methods that don't mutate

**Preserve** at API boundaries: `unsigned long` in existing virtual signatures, `RAINMAN_API` macro, `C`/`I`/`V`/`m_` naming conventions, include guards (`_C_CLASS_NAME_H_` style), LGPL headers, and vendored Lua code.

For detailed AI-assisted development guidelines, see [`.github/copilot-instructions.md`](.github/copilot-instructions.md).

## Documentation

The `docs/` directory contains detailed specifications:

- [`architecture.md`](docs/architecture.md) — System architecture overview
- [`target-architecture-v2.md`](docs/target-architecture-v2.md) — Target architecture design
- [`lua-language-server-integration.md`](docs/lua-language-server-integration.md) — LuaLS integration design and implementation
- [`sga-archive-format.md`](docs/sga-archive-format.md) — SGA archive file format specification
- [`rgd-game-data-format.md`](docs/rgd-game-data-format.md) — RGD binary data format
- [`relic-chunky-format.md`](docs/relic-chunky-format.md) — Relic Chunky container format
- [`ucs-localisation-format.md`](docs/ucs-localisation-format.md) — UCS localization format
- [`module-file-format.md`](docs/module-file-format.md) — .module file format
- [`data-inheritance-system.md`](docs/data-inheritance-system.md) — Lua-based data inheritance
- [`hash-algorithms.md`](docs/hash-algorithms.md) — Hash algorithms used in Relic formats
- [`texture-material-script-formats.md`](docs/texture-material-script-formats.md) — Texture and material formats
- [`known-bugs.md`](docs/known-bugs.md) — Known issues and workarounds

## License

- **CDMS Application**: GNU General Public License v2.0
- **Rainman Library**: GNU Lesser General Public License v2.1
- **Vendored Lua 5.0.2 / 5.1.2**: MIT License
- **Lucide Icons**: ISC License
