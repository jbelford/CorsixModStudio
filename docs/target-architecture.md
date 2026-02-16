# CorsixModStudio — Target Architecture & Refactoring Roadmap

> **Status**: Active — Phases A–E½ complete. Phase F (Modernization) is next. Based on the as-is analysis in [architecture.md](architecture.md).

---

## Table of Contents

1. [Goals & Principles](#1-goals--principles)
2. [Target Architecture Overview](#2-target-architecture-overview)
3. [Rainman Library Target](#3-rainman-library-target)
4. [CDMS GUI Target](#4-cdms-gui-target)
5. [Testing Strategy](#5-testing-strategy)
6. [Safe Refactoring Entry Points](#6-safe-refactoring-entry-points)
7. [Migration Roadmap](#7-migration-roadmap)
8. [Constraints & Non-Goals](#8-constraints--non-goals)

---

## 1. Goals & Principles

### Goals

1. **Stability first** — every change must keep the build green (`ctest --preset debug` passes)
2. **Reduce coupling** — a change to one subsystem shouldn't force rebuilding everything
3. **Enable testing** — make it possible to test components in isolation
4. **Incremental progress** — no "big rewrite"; each step is a small, reviewable change
5. **Preserve behavior** — the application must continue to work identically

### Principles

- **Additive before subtractive** — add new wrappers, tests, and abstractions before removing old code
- **Boy Scout Rule** — leave code better than you found it, but only the code you're touching
- **Build time matters** — reducing header coupling has compounding benefits
- **Tests are the safety net** — test before refactoring, not after
- **Naming conventions preserved** — keep `C`/`I`/`V`/`m_` conventions in existing code

---

## 2. Target Architecture Overview

```
┌───────────────────────────────────────────────────────────┐
│                    CDMS GUI Application                    │
│              (wxWidgets, views + tools)                    │
│                                                           │
│  ┌─────────────────────────────────────────────────────┐  │
│  │               Service / Adapter Layer               │  │
│  │  ModuleService, FileService, FormatService, etc.    │  │
│  │  (Translates between wx types and Rainman types)    │  │
│  └─────────────────────────────────────────────────────┘  │
├───────────────────────────────────────────────────────────┤
│                     Rainman Library                        │
│                                                           │
│  ┌────────┐ ┌─────────┐ ┌────────┐ ┌──────────────────┐  │
│  │  core/  │ │   io/   │ │archive/│ │    formats/      │  │
│  │Exception│ │IFileStore│ │CSgaFile│ │CRgdFile,CRgtFile│  │
│  │Logging │ │CFileSys │ │CSgaCrtr│ │CChunky,CRgm,CBfx│  │
│  │Util    │ │CMemory  │ └────────┘ └──────────────────┘  │
│  └────────┘ └─────────┘                                   │
│  ┌─────────────┐ ┌──────────────┐ ┌──────────────────┐   │
│  │     lua/     │ │localization/ │ │     module/      │   │
│  │CLuaFile      │ │CUcsFile      │ │CModuleFile       │   │
│  │CLuaScript    │ │CCohUcsFile   │ │CDoWModule        │   │
│  │CInheritTable │ │              │ │CFileMap          │   │
│  └─────────────┘ └──────────────┘ └──────────────────┘   │
├───────────────────────────────────────────────────────────┤
│              Vendored Lua 5.0.2 + 5.1.2                   │
└───────────────────────────────────────────────────────────┘
```

### Key Differences from Current State

| Aspect | Current | Target |
|--------|---------|--------|
| Rainman layout | ✅ Grouped into 8 subdirectories (`core/`, `io/`, `archive/`, `formats/`, `lua/`, `localization/`, `module/`, `util/`) | Grouped into 8 subdirectories |
| CDMS→Rainman coupling | ✅ Service/adapter layer (`ModuleService`, `FileService`, `FormatService`, `HashService`) | Via service/adapter layer |
| Header coupling | ✅ `Rainman.h` removed; directory-qualified includes | Targeted per-class includes; forward declarations |
| Stream ownership | ✅ RAII wrappers (`StreamGuard`) alongside existing API | RAII wrappers alongside existing API |
| Test coverage | ✅ ~85% class-level (38 test files: 28 Rainman + 10 CDMS) | 80%+ class-level |
| God classes | `CModuleFile`, `ConstructFrame`, `frmFiles_Actions.h` | Decomposed into focused classes |

---

## 3. Rainman Library Target

### 3.1 Subdirectory Structure

```
src/rainman/
├── core/
│   ├── Api.h                    # RAINMAN_API export macro
│   ├── Exception.h/cpp          # CRainmanException
│   ├── RainmanLog.h/cpp         # spdlog wrapper
│   ├── Callbacks.h/cpp          # pfnStatusCallback
│   ├── gnuc_defines.h/cpp       # GNUC compatibility
│   ├── memdebug.h               # Debug memory tracking
│   ├── WriteTime.h/cpp          # File timestamps
│   ├── Internal_Util.h/cpp      # Internal helpers
│   └── pch_rainman.h            # Precompiled header
│
├── io/
│   ├── IFileStore.h/cpp         # Stream interface
│   ├── IDirectoryTraverser.h/cpp# Directory traversal interface
│   ├── CFileSystemStore.h/cpp   # OS filesystem
│   ├── CMemoryStore.h/cpp       # In-memory buffers
│   └── StreamGuard.h            # NEW: RAII wrapper for IStream*
│
├── archive/
│   ├── CSgaFile.h/cpp           # SGA reader
│   └── CSgaCreator.h/cpp        # SGA writer
│
├── formats/
│   ├── IMetaTable.h/cpp         # Game data node interface
│   ├── CRgdFile.h/cpp           # Binary game data
│   ├── CRgdFileMacro.h/cpp      # RGD macro system
│   ├── CRgdHashTable.h/cpp      # Property name hashing
│   ├── CChunkyFile.h/cpp        # Relic chunked container
│   ├── CRgtFile.h/cpp           # Texture format
│   ├── CRgmFile.h/cpp           # Material format
│   └── CBfxFile.h/cpp           # Binary effects
│
├── lua/
│   ├── CLuaFile.h/cpp           # Lua 5.0.2 data parser
│   ├── CLuaFile2.h/cpp          # Alternative Lua parser
│   ├── CLuaFileCache.h/cpp      # Lua file cache
│   ├── CLuaFromRgd.h/cpp        # RGD → Lua conversion
│   ├── CLuaScript.h/cpp         # Script execution
│   ├── CLuaScript_Interface.h/cpp # Script API (currently dead code)
│   ├── CInheritTable.h/cpp      # Inheritance resolution
│   ├── Lua51.h                  # Lua 5.1.2 renamed API
│   ├── luax.h/cpp               # Lua 5.0.2 extensions
│   └── luax51.cpp               # Lua 5.1.2 extensions
│
├── localization/
│   ├── CUcsFile.h/cpp           # DoW UCS format
│   ├── CCohUcsFile.h/cpp        # CoH UCS format (dead code)
│   └── CUcsTransaction.h       # UCS transaction wrapper
│
├── module/
│   ├── CModuleFile.h/cpp        # .module file parser
│   ├── CDoWModule.h/cpp         # Game installation scanner
│   ├── CDoWFileView.h/cpp       # Virtual file view
│   └── CFileMap.h/cpp           # File index
│
├── util/
│   ├── Util.h/cpp               # General utilities
│   ├── crc32_case_idt.h/c       # CRC32 hashing
│   ├── hash.c                   # Jenkins hash
│   └── md5.h/c                  # MD5 hashing
│
├── vendor/
│   ├── lua502/                  # Vendored Lua 5.0.2 (do not modify)
│   └── lua512/                  # Vendored Lua 5.1.2 (do not modify)
│
├── pch_rainman.h                # Precompiled header
└── CMakeLists.txt               # Build configuration
```

### 3.2 Header Coupling Reduction

**Phase 1: Forward declarations** (purely additive, no API changes):

| Header | Current Include | Replace With |
|--------|----------------|-------------|
| `CModuleFile.h` | `#include "CSgaFile.h"` | `class CSgaFile;` (pointer members only) |
| `CModuleFile.h` | `#include "CUcsFile.h"` | `class CUcsFile;` (pointer members only) |
| `CModuleFile.h` | `#include "CDoWFileView.h"` | `class CDoWFileView;` (pointer members only) |
| `CModuleFile.h` | `#include "CFileMap.h"` | `class CFileMap;` (pointer members only) |
| `CModuleFile.h` | `#include "CFileSystemStore.h"` | `class CFileSystemStore;` (pointer member) |
| `CLuaFromRgd.h` | `#include "CRgdFile.h"` | `class CRgdFile;` (pointer params) |
| `CLuaFromRgd.h` | `#include "CLuaFile.h"` | `class CLuaFile;` (pointer params) |
| `CLuaFromRgd.h` | `#include "CModuleFile.h"` | `class CModuleFile;` (pointer params) |
| `CFileMap.h` | `#include "CSgaFile.h"` | `class CSgaFile;` (pointer params) |
| `CRgdFileMacro.h` | `#include "CRgdFile.h"` | `class CRgdFile;` (pointer member) |
| `CRgdFileMacro.h` | `#include "CModuleFile.h"` | `class CModuleFile;` (pointer param) |

**Phase 2: Eliminate god headers**:
- Replace `#include <Rainman.h>` in each CDMS `.cpp` file with specific includes (e.g., `#include "CModuleFile.h"`)
- Replace `Common.h`'s `#include <Rainman.h>` with nothing — each `.cpp` includes what it needs
- Keep `Rainman.h` as a convenience header but discourage its use

### 3.3 RAII Stream Wrapper (New, Additive)

A new `StreamGuard.h` alongside existing code — callers can opt in gradually:

```cpp
// io/StreamGuard.h — RAII wrapper for IFileStore streams
#pragma once
#include "IFileStore.h"

namespace rainman {

// Owns an IStream* and deletes it on destruction.
class StreamGuard {
public:
    explicit StreamGuard(IFileStore::IStream* p = nullptr) noexcept : m_p(p) {}
    ~StreamGuard() { delete m_p; }

    StreamGuard(StreamGuard&& other) noexcept : m_p(other.m_p) { other.m_p = nullptr; }
    StreamGuard& operator=(StreamGuard&& other) noexcept {
        if (this != &other) { delete m_p; m_p = other.m_p; other.m_p = nullptr; }
        return *this;
    }

    StreamGuard(const StreamGuard&) = delete;
    StreamGuard& operator=(const StreamGuard&) = delete;

    IFileStore::IStream* get() const noexcept { return m_p; }
    IFileStore::IStream* operator->() const noexcept { return m_p; }
    IFileStore::IStream* release() noexcept { auto* p = m_p; m_p = nullptr; return p; }
    explicit operator bool() const noexcept { return m_p != nullptr; }

private:
    IFileStore::IStream* m_p;
};

// Same for IOutputStream
class OutputStreamGuard {
public:
    explicit OutputStreamGuard(IFileStore::IOutputStream* p = nullptr) noexcept : m_p(p) {}
    ~OutputStreamGuard() { delete m_p; }

    OutputStreamGuard(OutputStreamGuard&& other) noexcept : m_p(other.m_p) { other.m_p = nullptr; }
    OutputStreamGuard& operator=(OutputStreamGuard&& other) noexcept {
        if (this != &other) { delete m_p; m_p = other.m_p; other.m_p = nullptr; }
        return *this;
    }

    OutputStreamGuard(const OutputStreamGuard&) = delete;
    OutputStreamGuard& operator=(const OutputStreamGuard&) = delete;

    IFileStore::IOutputStream* get() const noexcept { return m_p; }
    IFileStore::IOutputStream* operator->() const noexcept { return m_p; }
    IFileStore::IOutputStream* release() noexcept { auto* p = m_p; m_p = nullptr; return p; }
    explicit operator bool() const noexcept { return m_p != nullptr; }

private:
    IFileStore::IOutputStream* m_p;
};

} // namespace rainman
```

Usage (opt-in, alongside existing manual code):
```cpp
// Before:
IFileStore::IStream* pStream = store.VOpenStream("file.rgd");
rgd.Load(pStream);
delete pStream;

// After:
rainman::StreamGuard stream(store.VOpenStream("file.rgd"));
rgd.Load(stream.get());
// auto-deleted
```

---

## 4. CDMS GUI Target

### 4.1 Service Layer

Insert a thin adapter layer between GUI classes and Rainman. Each service wraps a specific Rainman subsystem and translates between Rainman types (`char*`, `CRainmanException*`) and wx types (`wxString`, `wxColour`):

```
cdms/services/
├── ModuleService.h/cpp      # Wraps CModuleFile (load, save, properties)
├── FileService.h/cpp        # Wraps IFileStore/IDirectoryTraverser (open, iterate)
├── FormatService.h/cpp      # Wraps CRgdFile, CUcsFile, etc. (load, edit, save)
└── HashService.h/cpp        # Wraps CRgdHashTable
```

Example:
```cpp
// services/FileService.h
class FileService {
public:
    explicit FileService(CModuleFile* pModule);

    // Returns a stream guard (RAII). Throws wxString on error.
    rainman::StreamGuard OpenStream(const wxString& sPath);

    // Iterates a directory, calling fn for each entry. Manages iterator lifetime.
    void IterateDirectory(const wxString& sPath,
                          std::function<void(const wxString& name, bool isDir)> fn);
};
```

**Migration strategy**: Create services alongside existing code. New code uses services; old code continues calling Rainman directly. Over time, migrate old code to services.

### 4.2 ConstructFrame Decomposition

Break `ConstructFrame` into composed helper classes (Phase E handles MenuController + ToolRegistry; Phase F handles ModuleManager + TabManager):

```
ConstructFrame
├── owns: MenuController        # Menu bar setup and event routing (Phase E)
├── owns: ToolRegistry           # ITool registration and dispatch (Phase E)
├── owns: ModuleManager          # CModuleFile* lifecycle (Phase F)
└── owns: TabManager             # wxAuiNotebook tab management (Phase F)
```

### 4.3 frmFiles_Actions.h Extraction

Split the 31 inline action handler classes into individual files under `cdms/actions/`:

```
cdms/actions/
├── TextViewAction.h/cpp
├── ExtractAction.h/cpp
├── RgdAction.h/cpp
├── ScarAction.h/cpp
├── LuaBurnAction.h/cpp
└── ... (31 total)
```

Each action class moves from being inline in the header to a proper .h/.cpp pair.

### 4.4 String Handling Modernization

- Use `wxString` as the primary string type in the CDMS layer
- Convert at the Rainman boundary (in services), not in every GUI class
- Replace manual `new[]`/`delete[]` with `wxString::ToUTF8()` / `wxString::FromUTF8()`

---

## 5. Testing Strategy

### 5.1 Priority Order for New Tests

Based on risk assessment and testability analysis:

| Priority | Class | Testability | Approach |
|----------|-------|-------------|----------|
| ✅ ~~**Critical**~~ | `CSgaCreator` | Easy (9/10) | ~~Round-trip: create SGA → read with CSgaFile → verify~~ Done (`sgacreator_test.cpp`) |
| ✅ ~~**High**~~ | `CBfxFile` | Easy (7/10) | ~~Extends CRgdFile; test Lua export~~ Done (`cbfxfile_test.cpp`) |
| ✅ ~~**High**~~ | `CRgmFile` | Medium (6/10) | ~~Load from CMemoryStore, verify material properties~~ Done (`rgmfile_test.cpp`) |
| ✅ ~~**High**~~ | `CLuaFile2` | Medium (5/10) | ~~Load Lua script from CMemoryStore, verify metadata~~ Done (`luafile2_test.cpp`) |
| ✅ ~~**Medium**~~ | `CLuaFromRgd` | Medium (6/10) | ~~Create CRgdFile in memory → convert → verify Lua output~~ Done (`luafromrgd_test.cpp`) |
| ✅ ~~**Medium**~~ | `CRgtFile` | Medium (5/10) | ~~Load minimal TGA from memory, save/reload round-trip~~ Done (`rgtfile_test.cpp`) |
| ✅ ~~**Medium**~~ | `CLuaFile` | Hard (4/10) | ~~IMetaTable interface testing via CMemoryStore~~ Done (`luafile_test.cpp`) |
| ✅ ~~**Low**~~ | `CLuaScript` | Easy (8/10) | ~~Load/execute scripts, error handling~~ Done (`luascript_test.cpp`) |
| ❌ Skip | `CLuaScript_Interface` | N/A | Dead code (`#if 0`) |
| ❌ Skip | `CCohUcsFile` | N/A | Dead code (`#if 0`) |
| ❌ Skip | `CDoWModule` | Hard (3/10) | Requires real game installation |

### 5.2 Test Patterns to Follow

**Model test files** (reference for new tests):
- `rgdfile_test.cpp` — best example of round-trip testing with nested data
- `chunkyfile_test.cpp` — good example of binary format testing
- `memorystore_test.cpp` — demonstrates CMemoryStore patterns

**Standard test structure**:
```cpp
class CSgaCreatorTest : public ::testing::Test {
protected:
    CMemoryStore m_store;
    void SetUp() override { m_store.VInit(); }
};

TEST_F(CSgaCreatorTest, CreateAndReadBack) {
    // Arrange: create test files in temp directory
    // Act: CSgaCreator::CreateSga(...)
    // Assert: load with CSgaFile, verify contents match
}
```

### 5.3 Dead Code Identified

| File | Status | Action |
|------|--------|--------|
| `CLuaScript_Interface.h/cpp` | All code in `#if 0` block | Skip testing; consider removal |
| `CCohUcsFile.h/cpp` | All code in `#if 0` block | Skip testing; consider removal |
| `CRedButtonTool` (CDMS) | Appears unused | Investigate before removing |

---

## 6. Safe Refactoring Entry Points

These are **purely additive changes** that cannot break existing behavior. They are the recommended starting points:

### 6.1 Tier 1: Zero Risk (New Files Only)

| Change | Files | Risk |
|--------|-------|------|
| Add `StreamGuard.h` RAII wrapper | New file only | None |
| Add `CSgaCreator` tests | New test file only | None |
| Add `CBfxFile` tests | New test file only | None |
| Add `CRgmFile` tests | New test file only | None |
| Add `CLuaFile2` tests | New test file only | None |

### 6.2 Tier 2: Low Risk (Header-Only Changes)

| Change | Files Modified | Risk |
|--------|---------------|------|
| Add forward declarations in `CModuleFile.h` | 1 header + 1 cpp | Low — move includes to .cpp |
| Add forward declarations in `CLuaFromRgd.h` | 1 header + 1 cpp | Low |
| Add forward declarations in `CFileMap.h` | 1 header + 1 cpp | Low |
| Add forward declarations in `CRgdFileMacro.h` | 1 header + 1 cpp | Low |

### 6.3 Tier 3: Medium Risk (Structural Changes)

| Change | Files Modified | Risk |
|--------|---------------|------|
| Create `cdms/services/` adapter layer | New files + modify some CDMS .cpp | Medium — new code path |
| Split `frmFiles_Actions.h` into separate files | 1 header → 30+ files | Medium — must preserve all behavior |
| Replace `Common.h` → `Rainman.h` with per-file includes | All CDMS .cpp files | Medium — must not miss any |
| Move Rainman files into subdirectories | All includes update | Medium — must update all paths |

### 6.4 Tier 4: High Risk (API Changes)

| Change | Impact | Risk |
|--------|--------|------|
| Smart pointer migration for streams | All stream callers | High — behavioral change |
| Decompose `CModuleFile` into sub-classes | All CModuleFile callers | High |
| Decompose `ConstructFrame` | All CDMS frames | High |
| Exception handling modernization | All catch sites | High |

---

## 7. Migration Roadmap

Each phase is a standalone unit of work. Complete one before starting the next. Every phase ends with all tests passing.

### Phase A: Foundation (Tests + RAII Wrappers) ✅

1. ~~Add `StreamGuard.h` RAII wrapper (new file, no existing code changes)~~
2. ~~Write `CSgaCreator` round-trip tests~~
3. ~~Write `CBfxFile` tests~~
4. ~~Write `CRgmFile` tests~~
5. ~~Write `CLuaFile2` basic tests~~
6. ~~Use `StreamGuard` in new test code as a proving ground~~

### Phase B: Header Decoupling ✅

1. ~~Add forward declarations to `CModuleFile.h` (move 5 includes to .cpp)~~
2. ~~Add forward declarations to `CLuaFromRgd.h`, `CFileMap.h`, `CRgdFileMacro.h`~~
3. ~~Verify build times improved~~
4. ~~Replace per-file `#include <Rainman.h>` in CDMS `.cpp` files with targeted includes~~
5. ~~Remove `<Rainman.h>` from `Common.h`~~

### Phase C: Rainman Subdirectory Reorganization ✅

1. ~~Create subdirectories: `core/`, `io/`, `archive/`, `formats/`, `lua/`, `localization/`, `module/`, `util/`~~
2. ~~Move files in small batches (one subdirectory at a time)~~
3. ~~Update `CMakeLists.txt` glob patterns and include paths~~
4. ~~Verify full build + test after each batch~~
5. ~~Delete unused vendored `zLib/` directory~~
6. ~~Adopt directory-qualified includes (`#include "core/Exception.h"`, etc.)~~

### Phase D: CDMS Service Layer ✅

1. ~~Create `Result<T>` error type in `services/Result.h`~~
2. ~~Create `services/` directory and update `CMakeLists.txt`~~
3. ~~Create `ModuleService` wrapping `CModuleFile` operations~~
4. ~~Create `FileService` wrapping `IFileStore`/`IDirectoryTraverser`~~
5. ~~Create `FormatService` wrapping format parsers (RGD, Chunky, RGT, RGM, BFX, UCS)~~
6. ~~Create `HashService` wrapping `CRgdHashTable` with lazy init~~
7. ~~Migrate `Construct.cpp` to use services (`DoLoadSga`, `SetLocale`, `GetRgdHashTable`)~~
8. ~~Migrate `frmFiles.cpp` tree traversal + action handlers to use `FileService`~~
9. ~~Migrate format editor files to use `FormatService` (`frmRgdEditor`, `frmRgdMacro`, `frmRgmMaterialEditor`, `frmImage`, `frmScarEditor`, `frmUCSEditor`)~~
10. ~~Migrate module-related files to use `ModuleService` (`frmModule`, `frmMassExtract`, `frmUCSSelector`, `Tool_AESetup`)~~
11. ~~Migrate file-navigation files to use `FileService` (`frmFileSelector`, `frmLuaInheritTree`)~~
12. ~~Migrate tool files (`Tool_AutoDPS`, `Tools.cpp`)~~
13. ~~Migrate utility/config files (`Utility.h`, `config.cpp`, `frmSgaMake.cpp`)~~
14. ~~Remove unused Rainman includes from migrated CDMS files~~
15. ~~Final clean build + test verification~~

### Phase E: CDMS Structural Cleanup ✅

#### E1 — Extract `frmFiles_Actions.h` into Individual Action Files ✅

1. ~~Create `src/cdms/actions/` directory~~
2. ~~Update `CMakeLists.txt` glob to include `actions/*.cpp`~~
3. ~~Move `OnlyFilename()` helper to `actions/ActionUtil.h`~~
4. ~~Extract each of the 31 inline action classes into individual `actions/FooAction.h` files~~
5. ~~Update `frmFiles.cpp` to include individual action headers instead of `frmFiles_Actions.h`~~
6. ~~Delete `frmFiles_Actions.h`~~
7. ~~Build + test verification~~

#### E2 — Extract `ITool` Interface to Own Header ✅

1. ~~Create `src/cdms/ITool.h` with the ITool interface (was nested inside `ConstructFrame`)~~
2. ~~Update `Construct.h` — `using ITool = ::ITool;` preserves backward compatibility~~
3. ~~Build + test~~

#### E3 — Extract `ToolRegistry` from `ConstructFrame` ✅

1. ~~Create `src/cdms/ToolRegistry.h/.cpp`~~
2. ~~Move `m_vTools`, `GetToolCount()`, `GetTool()`, `DoTool()` into `ToolRegistry`~~
3. ~~Add `LaunchTool(size_t index)` method~~
4. ~~`ConstructFrame` holds a `ToolRegistry m_toolRegistry` member~~
5. ~~ToolRegistry destructor owns tool lifetime (replaces manual cleanup loop)~~
6. ~~Build + test~~

#### E4 — Replace `LaunchModToolN()` with Array-Driven Dispatch ✅

1. ~~Replace 20 `EVT_MENU(IDM_ModToolN, ...)` entries with `Bind()` calls during menu construction~~
2. ~~Create single `OnToolMenuCommand(wxCommandEvent&)` handler (extracts tool index from event ID)~~
3. ~~Remove `FN_MOD_TOOL` macro and all 20 expansions~~
4. ~~Remove 20 `LaunchModToolN` declarations from `Construct.h`~~
5. ~~Replace `IDM_ModTool1`–`IDM_ModTool20` with `IDM_ModToolBase` + `IDM_ModToolLast` (100 slots)~~
6. ~~Build + test~~

#### E5 — Extract `MenuController` from `ConstructFrame` ✅

1. ~~Create `src/cdms/MenuController.h/.cpp`~~
2. ~~Move menu bar construction (~80 lines) from `ConstructFrame` constructor into `MenuController::Build()`~~
3. ~~`MenuController` receives `wxFrame*` and `ToolRegistry&` references~~
4. ~~`ConstructFrame` holds a `MenuController m_menuController` member~~
5. ~~Menu enable/disable logic stays in `ConstructFrame` (coupled with mod state transitions)~~
6. ~~Build + test~~

### Phase E½: Remaining Rainman Test Coverage ✅

1. ~~Write `CLuaFromRgd` tests (RGD→Lua conversion round-trip)~~
2. ~~Write `CRgtFile` tests (load minimal TGA/DDS from `CMemoryStore`)~~
3. ~~Write `CLuaFile` tests (Lua state management; defer if CLuaFile2 covers enough)~~
4. ~~Write `CLuaScript` tests (simple wrapper; low risk)~~

### Phase F: Modernization (Long-Term)

1. ~~Replace manual `delete pStream` with `std::unique_ptr<IFileStore::IStream>` (deleted `StreamGuard.h`)~~
2. ~~Add `ExceptionDeleter` for `std::unique_ptr<CRainmanException, ExceptionDeleter>`, replacing manual `destroy()` calls~~
3. ~~Modernize string handling: `wxStringToAscii()` returns `std::unique_ptr<char[]>`, eliminating manual `delete[]`~~
4. ~~Extract `ModuleManager` from `ConstructFrame` (module lifecycle, services, hash table)~~
5. ~~Extract `TabManager` from `ConstructFrame` (wxAuiNotebook tab management)~~
6. Consider `CModuleFile` decomposition (only with full test coverage)

---

## 8. Constraints & Non-Goals

### Hard Constraints

- **`CDMSSrc_055/`** is read-only — never modify
- **Lua 5.0.2 and 5.1.2** must not be upgraded (game data compatibility)
- **Build stays green** — `ctest --preset debug` must pass after every commit
- **LGPL v2.1 headers** preserved on all Rainman files
- **Existing API surface** — `CModuleFile`, `IFileStore`, etc. cannot change signatures without updating all callers
- **Hungarian notation** — preserve `C`/`I`/`V`/`m_` conventions in existing code

### Non-Goals (Explicitly Out of Scope)

- Cross-platform support (Windows-only is acceptable)
- Smart pointer migration of existing code (too risky without full test coverage)
- Rewriting the exception system (heap-allocated pattern is stable, just needs RAII wrapper)
- Upgrading wxWidgets beyond 3.x
- Adding new features until stability is achieved
- Performance optimization (correctness and maintainability first)
