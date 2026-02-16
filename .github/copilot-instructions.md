# Copilot Instructions for CorsixModStudio

## Build & Test

**Always use CMake presets** — never pass raw `-G`, `-A`, or `-DCMAKE_TOOLCHAIN_FILE` flags manually.

```powershell
# Prerequisites: VCPKG_ROOT environment variable must point to vcpkg installation

# Configure (uses VS generator — works out of the box)
cmake --preset default

# Build
cmake --build --preset debug     # or: --preset release

# Run all tests
ctest --preset debug

# Run a single test by name (supports regex)
ctest --preset debug -R "CRainmanException.BasicCreation"

# --- clang-tidy analysis (via Visual Studio CMake preset vendor settings) ---
cmake --preset tidy
cmake --build --preset tidy-debug

# Run clang-tidy manually (command-line)
.\tools\run-clang-tidy.ps1                         # all Rainman sources
.\tools\run-clang-tidy.ps1 src/rainman/archive/CSgaFile.cpp # single file
.\tools\run-clang-tidy.ps1 -Fix                    # auto-fix
```

## Read-Only Reference Code

**`CDMSSrc_055/`** contains the original unmodified Corsix source code and is kept as a **read-only reference**. Never modify any files under this directory. All development happens in `src/`.

## Architecture

This is a modernization of a ~2006 C++ modding IDE for Dawn of War / Company of Heroes. The codebase has three layers:

**Rainman** (`src/rainman/`) — Static library (~25k lines) for Relic game file format I/O. No UI dependencies. Organized into subdirectories:
- `core/` — Exception handling, logging (`spdlog`), callbacks, API macros, utility types
- `io/` — `IFileStore`, `CMemoryStore`, `CFileSystemStore`, `StreamGuard` (RAII wrapper)
- `archive/` — `CSgaFile` (SGA reader), `CSgaCreator` (SGA writer)
- `formats/` — `CRgdFile`, `CChunkyFile`, `CRgtFile`, `CRgmFile`, `CBfxFile`, `IMetaTable`, `CRgdHashTable`
- `lua/` — `CLuaFile`, `CLuaFile2`, `CInheritTable`, `CLuaFromRgd`, `CLuaScript`, `luax`
- `localization/` — `CUcsFile`, `CCohUcsFile`, `CUcsTransaction`
- `module/` — `CModuleFile`, `CDoWModule`, `CDoWFileView`, `CFileMap`, `CModuleMetadata`, `CModuleParser`, `CResourceLoader`
- `util/` — `Util`, `crc32_case_idt`, `hash`, `md5`

All includes use fully-qualified paths with the `rainman/` prefix (e.g., `#include "rainman/core/Exception.h"`, `#include <rainman/io/IFileStore.h>`).

**CDMS Services** (`src/cdms/services/`, `src/cdms/async/`) — Intermediate layer between Rainman and the GUI:
- `services/` — `ModuleService`, `FileService`, `FormatService`, `HashService` wrap Rainman calls with `Result<T>` error handling (see `Result.h`). Use `ResultFromException()` to convert `CRainmanException*` chains into `Result<void>::Err`.
- `async/` — `CCancellationToken` (header-only), `CProgressChannel` (with RainmanCallback bridge), `CTaskRunner` (pure C++ `std::thread`), `CWxTaskRunner` (posts to main thread via `CallAfter`).

**CDMS GUI** (`src/cdms/`) — wxWidgets 3.2 application. Frame classes (`frm*.cpp`), action handlers (`actions/*.h`), tab/menu management. Service classes accessed via `GetModuleService()`, `GetFileService()`, etc. on the `ConstructFrame`.

**Vendored Lua 5.0.2** (`src/rainman/vendor/lua502/`) — Corsix's patched Lua 5.0.2, built as static library `lua502`. Do not modify or upgrade — game data compatibility requires this exact version.

**Vendored Lua 5.1.2** (`src/rainman/vendor/lua512/`) — Built as static library `lua512` with all public symbols renamed via `lua512_rename.h` (`lua_` → `lua51_`) to avoid linker conflicts with Lua 5.0.2. Consumer code uses `lua/Lua51.h` which declares the renamed API directly.

## Key Conventions

### Error handling
The codebase uses heap-allocated exception objects, not stack-based C++ exceptions. Exceptions are created with `new` and cleaned up with `destroy()`, never `delete`:
```cpp
throw new CRainmanException(__FILE__, __LINE__, "message");
// Cleanup:
pException->destroy();  // NOT delete
```
Use the macros: `QUICK_THROW("msg")`, `CATCH_THROW("msg")`, `IGNORE_EXCEPTIONS`, `CHECK_MEM(new Foo)`.

In the CDMS service layer, catch exceptions and convert them to `Result<T>`:
```cpp
catch (CRainmanException* pE) { return ResultFromException(pE); }
```

### API export macro
All public Rainman classes use `RAINMAN_API` (defined in `rainman/core/Api.h`). It expands to nothing in the current static-lib build (`RAINMAN_NO_EXPORTS` is defined), but must remain on class declarations for future DLL compatibility.

### Naming
- Classes: `C` prefix for concrete (`CSgaFile`), `I` prefix for interfaces (`IFileStore`)
- Members: `m_` prefix with Hungarian notation (`m_sMessage` = string, `m_iLine` = integer, `m_pPrecursor` = pointer, `m_bInited` = bool)
- Virtual methods: `V` prefix (`VInit`, `VRead`, `VSeek`)

### Tests
Tests use Google Test in `tests/rainman/` and `tests/cdms/`. Test files are named `*_test.cpp` and must be **manually added** to the corresponding `CMakeLists.txt` — test files are **not** auto-discovered via glob.

CDMS tests must also list the specific CDMS `.cpp` source files they depend on in `tests/cdms/CMakeLists.txt` (since tests link against Rainman but compile CDMS sources directly).

### Source file discovery
Rainman and CDMS **production** source files are auto-discovered via `file(GLOB)` — just create the file and rebuild. Test files must be explicitly listed.

### Stream ownership
`VOpenStream()` / `VOpenOutputStream()` return heap-allocated streams. The **caller** owns the returned stream and must `delete` it. `Load()` / `Save()` methods on parser classes do **not** take ownership of the stream passed to them.

### Testing with CMemoryStore
Use `CMemoryStore` for in-memory I/O in tests instead of touching the filesystem:
```cpp
CMemoryStore store;
store.VInit();
// Wrap raw data as a readable stream:
char* pRange = store.MemoryRange((void*)data, dataLen);
IFileStore::IStream* pStream = store.VOpenStream(pRange);
// ... read from pStream, then delete pStream
```

### Multi-inclusion headers
`strings.h` uses a `S_CPP_` macro pattern to switch between `extern` declarations and definitions on re-inclusion. Do **not** add `#pragma once` to this file. `strings.cpp` pre-defines the include guard, includes `Common.h`, then undefines it and re-includes with `S_CPP_` defined.

## Tool Usage

### Documentation lookups
Use **Context7** (`context7-resolve-library-id` → `context7-query-docs`) to look up documentation for libraries, frameworks, and general programming technologies (e.g., CMake, Google Test, wxWidgets, Lua, vcpkg).

### Windows & Microsoft technologies
Use **Microsoft Learn** (`microsoft-learn-microsoft_docs_search`, `microsoft-learn-microsoft_code_sample_search`, `microsoft-learn-microsoft_docs_fetch`) for Windows-specific APIs, MSVC compiler behavior, Win32, COM, DirectX, and any other Microsoft/Azure documentation.

### Crash debugging
Use **WinDbg** tools (`windbg-open_windbg_dump`, `windbg-run_windbg_cmd`, `windbg-open_windbg_remote`) to analyze crash dumps (`.dmp` files) and connect to remote debugging sessions. Use `windbg-list_windbg_dumps` to discover available dump files.

### Boy-scout modernization
This is a 2006 codebase being ported to C++20/x64. Apply the **boy-scout rule**: when touching existing code, improve it toward modern C++ if the change is safe and localized. Examples:
- Replace `NULL` with `nullptr`
- Add `override` to virtual method overrides
- Replace `new[]`/`delete[]` with `std::vector` or `std::unique_ptr` (internal code only)
- Replace C-style casts with `static_cast`/`reinterpret_cast`
- Use range-based `for` instead of index loops where it fits
- Add `const` to parameters and methods that don't mutate
- Use `auto` where the type is obvious from the right-hand side

**Do not** modernize across API boundaries in the same change — keep the diff focused. These legacy patterns must be preserved:
- `unsigned long` in existing public/virtual API signatures (changing would cascade)
- Heap-allocated exceptions (`throw new CRainmanException`) and `destroy()` cleanup
- `RAINMAN_API` macro on public classes
- `C`/`I`/`V`/`m_` naming conventions and Hungarian prefixes
- Include guards (`_C_CLASS_NAME_H_` style) — do not replace with `#pragma once`
- LGPL v2.1 file headers
- Vendored Lua code (`vendor/lua502/`, `vendor/lua512/`) — never modify
