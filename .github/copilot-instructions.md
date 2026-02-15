# Copilot Instructions for CorsixModStudio

## Build & Test

```powershell
# Prerequisites: VCPKG_ROOT environment variable must point to vcpkg installation

# Configure
cmake -B build -S . -G "Visual Studio 18 2026" -A x64 `
    -DCMAKE_TOOLCHAIN_FILE="$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"

# Build
cmake --build build --config Debug

# Run all tests
ctest --test-dir build -C Debug -j8 --output-on-failure

# Run a single test by name (supports regex)
ctest --test-dir build -C Debug -R "CRainmanException.BasicCreation" --output-on-failure
```

## Read-Only Reference Code

**`CDMSSrc_055/`** contains the original unmodified Corsix source code and is kept as a **read-only reference**. Never modify any files under this directory. All development happens in `src/`.

## Architecture

This is a modernization of a ~2006 C++ modding IDE for Dawn of War / Company of Heroes. The codebase has two layers:

**Rainman** (`src/rainman/`) — Static library (~25k lines) for Relic game file format I/O. No UI dependencies. This is where most active development happens (Phases 2–3). Key abstractions:
- `IFileStore` → `CMemoryStore`, `CFileSystemStore`, `CSgaFile` — polymorphic I/O with `VOpenStream()`/`VOpenOutputStream()` returning `IStream`/`IOutputStream` objects
- `CRgdFile`, `CUcsFile`, `CChunkyFile`, `CModuleFile` — parsers for Relic's proprietary formats
- `CSgaCreator` — SGA archive writer
- `CInheritTable` — Lua-based data inheritance resolution

**CDMS** (`src/cdms/`) — wxWidgets GUI application built on Rainman. Not yet ported (Phase 4, blocked on wxWidgets 3.2 migration).

**Vendored Lua 5.0.2** (`src/rainman/lua502/`) — Corsix's patched Lua 5.0.2, built as static library `lua502`. Do not modify or upgrade — game data compatibility requires this exact version.

**Vendored Lua 5.1.2** (`src/rainman/lua512src/`) — Built as static library `lua512` with all public symbols renamed via `lua512_rename.h` (`lua_` → `lua51_`) to avoid linker conflicts with Lua 5.0.2. The `lua512/` directory contains only type headers for the older runtime-loading path (`LoadLibraryW` / `GetProcAddress` in `Lua51.cpp`).

## Key Conventions

### Error handling
The codebase uses heap-allocated exception objects, not stack-based C++ exceptions. Exceptions are created with `new` and cleaned up with `destroy()`, never `delete`:
```cpp
throw new CRainmanException(__FILE__, __LINE__, "message");
// Cleanup:
pException->destroy();  // NOT delete
```
Use the macros: `QUICK_THROW("msg")`, `CATCH_THROW("msg")`, `IGNORE_EXCEPTIONS`, `CHECK_MEM(new Foo)`.

### API export macro
All public Rainman classes use `RAINMAN_API` (defined in `Api.h`). It expands to nothing in the current static-lib build (`RAINMAN_NO_EXPORTS` is defined), but must remain on class declarations for future DLL compatibility.

### Naming
- Classes: `C` prefix for concrete (`CSgaFile`), `I` prefix for interfaces (`IFileStore`)
- Members: `m_` prefix with Hungarian notation (`m_sMessage` = string, `m_iLine` = integer, `m_pPrecursor` = pointer, `m_bInited` = bool)
- Virtual methods: `V` prefix (`VInit`, `VRead`, `VSeek`)

### Tests
Tests use Google Test in `tests/rainman/`. Test files are named `*_test.cpp` and must be manually added to `tests/rainman/CMakeLists.txt`. Tests are auto-discovered via `gtest_discover_tests()`.

### Source file discovery
Rainman and CDMS source files are auto-discovered via `file(GLOB)` — just create the file and rebuild. Test files are **not** globbed; they must be explicitly listed in the test `CMakeLists.txt`.

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

### Legacy code considerations
This is a 2006 codebase being ported to C++17/x64. Expect and preserve:
- `unsigned long` instead of `size_t` or `uint32_t` in existing APIs (changing would cascade)
- Raw `new`/`delete` memory management (no smart pointers in existing code)
- Windows-only APIs (`LoadLibraryW`, `GetProcAddress`, `_wfopen`)
- Original file headers with LGPL v2.1 copyright notices must be preserved
- Include guards use `_C_CLASS_NAME_H_` style (e.g., `_C_SGA_FILE_H_`)
