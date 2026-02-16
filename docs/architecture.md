# CorsixModStudio — Architecture Document

> **Status**: Living document — captures the as-is architecture after all migration phases (A–F) are complete.
> **Audience**: Contributors, AI assistants, and future maintainers.

---

## Table of Contents

1. [System Overview](#1-system-overview)
2. [Codebase Metrics](#2-codebase-metrics)
3. [Rainman Library](#3-rainman-library)
4. [CDMS GUI Application](#4-cdms-gui-application)
5. [Build System & Dependencies](#5-build-system--dependencies)
6. [Test Infrastructure](#6-test-infrastructure)
7. [Include Dependency Graph](#7-include-dependency-graph)
8. [Technical Debt Inventory](#8-technical-debt-inventory)
9. [Key Design Patterns](#9-key-design-patterns)
10. [Glossary](#10-glossary)

---

## 1. System Overview

CorsixModStudio is a modding IDE for Relic Entertainment's Dawn of War and Company of Heroes games. The architecture is a two-layer system:

```
┌─────────────────────────────────────────────────────────┐
│                   CDMS GUI Application                  │
│            (wxWidgets, 16.5k lines, GPL v2)             │
│                                                         │
│  ConstructFrame ─┬─ ModuleManager (mod lifecycle)       │
│                  ├─ TabManager (wxAuiNotebook tabs)     │
│                  ├─ MenuController (menu bar setup)     │
│                  ├─ ToolRegistry (ITool dispatch)       │
│                  ├─ frmFiles (file browser)             │
│                  ├─ frmRgdEditor (RGD editor)           │
│                  ├─ frmScarEditor (script editor)       │
│                  ├─ frmUCSEditor (localization editor)   │
│                  ├─ frmModule (mod settings)             │
│                  ├─ frmSgaMake (archive builder)         │
│                  └─ 10 ITool plugins                    │
│                                                         │
│  ┌──────────── Service / Adapter Layer ───────────────┐ │
│  │  ModuleService, FileService, FormatService,        │ │
│  │  HashService — wraps Rainman types → wx types      │ │
│  └────────────────────────────────────────────────────┘ │
├─────────────────────────────────────────────────────────┤
│                    Rainman Library                       │
│           (static lib, 26.8k lines, LGPL v2.1)          │
│                                                         │
│  ┌──────────┐  ┌──────────────┐  ┌──────────────────┐  │
│  │ core/    │  │ formats/     │  │ lua/             │  │
│  │Exception │  │ CRgdFile     │  │ CLuaFile (5.0.2) │  │
│  │Logging   │  │ CChunkyFile  │  │ CLuaFile2(5.0.2) │  │
│  │Callbacks │  │ CRgtFile     │  │ CLuaScript       │  │
│  └──────────┘  │ CRgmFile     │  │ CLuaFromRgd      │  │
│  ┌──────────┐  │ CBfxFile     │  │ CInheritTable    │  │
│  │ io/      │  └──────────────┘  │ Lua51 (5.1.2)    │  │
│  │IFileStore│  ┌──────────────┐  └──────────────────┘  │
│  │CFileSys  │  │localization/ │                        │
│  │CMemory   │  │ CUcsFile     │  ┌──────────────────┐  │
│  └──────────┘  │ CCohUcsFile  │  │ util/            │  │
│  ┌──────────┐  └──────────────┘  │ CRgdHashTbl      │  │
│  │ archive/ │  ┌──────────────┐  │ CSgaCreator      │  │
│  │CSgaFile  │  │ module/      │  │ Callbacks        │  │
│  │CSgaCrtr  │  │CModuleFile   │  │ RainmanLog       │  │
│  └──────────┘  │CDoWModule    │  └──────────────────┘  │
│                │CDoWFileView  │                        │
│                │ CFileMap     │                        │
│                └──────────────┘                        │
├─────────────────────────────────────────────────────────┤
│              Vendored Lua Runtimes                      │
│   lua502 (11.4k lines)    lua512 (13.6k lines)         │
│   Native symbols          Renamed: lua_ → lua51_        │
└─────────────────────────────────────────────────────────┘
```

---

## 2. Codebase Metrics

| Component | Lines of Code | Files | Language |
|-----------|--------------|-------|----------|
| Rainman library (`src/rainman/` excl vendor) | 26,801 | 88 | C++/C |
| CDMS application (`src/cdms/`) | 16,503 | 115 | C++ |
| Tests (`tests/**/*.cpp`) | 4,420 | 37 | C++ |
| Vendored Lua 5.0.2 (`vendor/lua502/`) | 11,384 | ~40 | C |
| Vendored Lua 5.1.2 (`vendor/lua512/`) | 13,601 | ~40 | C |
| **Total (our code)** | **47,724** | **~240** | |
| **Total (including vendored)** | **72,709** | **~320** | |

### Largest Files (>500 LOC)

| File | Lines | Layer | Purpose |
|------|-------|-------|---------|
| `CRgdFile.cpp` | 1,982 | Rainman | Binary game data format parser |
| `CDoWModule.cpp` | 1,969 | Rainman | Dawn of War module enumeration & loading |
| `CRgdFileMacro.cpp` | 1,616 | Rainman | RGD macro application via Lua 5.1.2 |
| `CLuaFile.cpp` | 1,582 | Rainman | Lua 5.0.2 game data file parser |
| `frmRgdEditor.cpp` | 1,539 | CDMS | RGD property grid editor |
| `CLuaFile2.cpp` | 1,306 | Rainman | Alternative Lua file parser |
| `CLuaFromRgd.cpp` | 1,264 | Rainman | RGD → Lua conversion |
| `CRgtFile.cpp` | 1,072 | Rainman | Texture format (DXT/DDS/TGA) |
| `Construct.cpp` | 718 | CDMS | Main frame (delegates to ModuleManager, TabManager, etc.) |
| `CFileMap.cpp` | 980 | Rainman | File index/map across archives |
| `CSgaFile.cpp` | 966 | Rainman | SGA archive reader |
| `CLuaScript_Interface.cpp` | 890 | Rainman | Lua scripting interface (dead code — `#if 0`) |
| `CModuleFile.cpp` | 775 | Rainman | .module file façade (delegates to CModuleParser, CResourceLoader) |
| `CFileSystemStore.cpp` | 759 | Rainman | OS filesystem I/O |
| `frmScarEditor.cpp` | 716 | CDMS | SCAR script editor |

---

## 3. Rainman Library

### 3.1 Core Interfaces

The library is built on three polymorphic interface hierarchies:

#### IFileStore — Stream-Based I/O

```
IFileStore
├── VInit(void* pInitData = 0)
├── VOpenStream(const char* sIdentifier) → IStream*          [pure virtual]
└── VOpenOutputStream(const char* sId, bool bErase) → IOutputStream*

    IFileStore::IStream
    ├── VRead(unsigned long iCount, unsigned long iSize, void* pDest)  [pure virtual]
    ├── VSeek(long iPos, SeekLocation SeekFrom = SL_Current)           [pure virtual]
    └── VTell() → long                                                 [pure virtual]

    IFileStore::IOutputStream : IStream
    └── VWrite(unsigned long iCount, unsigned long iSize, const void* pSrc)  [pure virtual]
```

**Implementations:**

| Class | Inherits | Purpose |
|-------|----------|---------|
| `CFileSystemStore` | `IFileStore`, `IDirectoryTraverser` | OS filesystem (FILE*-based streams) |
| `CMemoryStore` | `IFileStore` | In-memory buffers (used in tests, archive decompression) |
| `CSgaFile` | `IFileStore`, `IDirectoryTraverser` | SGA archive reader (v2/v4, zlib decompression) |
| `CModuleFile` | `IFileStore`, `IDirectoryTraverser` | .module file aggregator (delegates to sub-stores) |
| `CDoWModule` | `IFileStore`, `IDirectoryTraverser` | Game installation scanner |
| `CDoWFileView` | `IFileStore`, `IDirectoryTraverser` | Virtual file view across module sources |
| `CFileMap` | `IFileStore`, `IDirectoryTraverser` | Indexed file lookup across archives |

**Stream ownership**: `VOpenStream()` / `VOpenOutputStream()` return heap-allocated streams. The **caller owns** the returned pointer and must `delete` it. `Load()` / `Save()` methods on parser classes do **not** take ownership of the stream passed to them.

#### IDirectoryTraverser — Directory Enumeration

```
IDirectoryTraverser
├── VIterate(const char* sPath) → IIterator*          [pure virtual]
├── VGetEntryPointCount() → unsigned long              [pure virtual]
├── VGetEntryPoint(unsigned long iID) → const char*    [pure virtual]
├── VCreateFolderIn(const char* sPath, const char* sNewFolderName)  [pure virtual]
├── VGetLastWriteTime(const char* sPath) → tLastWriteTime           [pure virtual]
└── VDirectoryExists(const char* sPath) → bool

    IDirectoryTraverser::IIterator
    ├── VGetType() → eTypes                            [pure virtual]
    ├── VOpenSubDir() → IIterator*                     [pure virtual]
    ├── VOpenFile() → IFileStore::IStream*             [pure virtual]
    ├── VGetName() → const char*                       [pure virtual]
    ├── VGetFullPath() → const char*                   [pure virtual]
    ├── VGetDirectoryPath() → const char*              [pure virtual]
    ├── VGetLastWriteTime() → tLastWriteTime           [pure virtual]
    ├── VNextItem() → eErrors                          [pure virtual]
    └── VGetTag(long iTag) → void*
```

All classes that implement `IFileStore` (except `CMemoryStore`) also implement `IDirectoryTraverser`.

#### IMetaNode / IMetaTable — Game Data Representation

```
IMetaNode
├── VGetType() / VSetType(eDataTypes)                  [pure virtual]
├── VGetName() / VSetName(const char*)                 [pure virtual]
├── VGetNameHash() / VSetNameHash(unsigned long)
├── VGetValueFloat() / VSetValueFloat(float)           [pure virtual]
├── VGetValueInteger() / VSetValueInteger(unsigned long)[pure virtual]
├── VGetValueBool() / VSetValueBool(bool)              [pure virtual]
├── VGetValueString() / VSetValueString(const char*)   [pure virtual]
├── VGetValueWString() / VSetValueWString(const wchar_t*) [pure virtual]
├── VGetValueMetatable() → IMetaTable*                 [pure virtual]
├── VGetNodeAsRainmanRgd() → CMemoryStore::COutStream* [pure virtual]
└── SGetNodeFromRainmanRgd(IFileStore::IStream*, bool) [pure virtual]

    IMetaNode::IMetaTable : IMetaNode
    ├── VGetChildCount() → unsigned long               [pure virtual]
    ├── VGetChild(unsigned long) → IMetaNode*          [pure virtual]
    ├── VGetReferenceType() / VSetReferenceType()      [pure virtual]
    ├── VGetReferenceString() / VSetReferenceString()   [pure virtual]
    ├── VGetReferenceWString() / VSetReferenceWString() [pure virtual]
    ├── VAddChild(const char*) → IMetaNode*            [pure virtual]
    ├── VDeleteChild(unsigned long)                    [pure virtual]
    ├── VSupportsRefresh() → bool
    └── VDoRefresh()
```

**Implementations:**
- `CRgdFile` implements `IMetaNode` — binary RGD game data (with nested `CMetaNode`, `CMetaTable`)
- `CLuaFile` implements `IMetaNode::IMetaTable` — Lua-based game data
- `CBfxFile` inherits from `CRgdFile` — binary effects (specializes RGD)

### 3.2 Subsystem Map

Rainman source files are organized into 8 subdirectories under `src/rainman/`. All includes use the `rainman/` prefix (e.g., `#include "rainman/core/Exception.h"`).

#### File I/O Subsystem
**Files**: `io/IFileStore.h/cpp`, `io/CFileSystemStore.h/cpp`, `io/CMemoryStore.h/cpp`, `io/IDirectoryTraverser.h/cpp`

Foundation layer. Provides the polymorphic stream abstraction that all format parsers and higher-level stores use. `CMemoryStore` is also the primary test helper for in-memory I/O. Stream ownership uses `std::unique_ptr<IFileStore::IStream>` for RAII.

#### Archive Subsystem
**Files**: `archive/CSgaFile.h/cpp`, `archive/CSgaCreator.h/cpp`

Reads and creates Relic's SGA archives. `CSgaFile` supports both v2 (DoW) and v4 (CoH) formats with zlib decompression, MD5 validation, and CRC32 directory hashing. `CSgaCreator` builds archives from `IDirectoryTraverser` sources.

#### Format Parser Subsystem
**Files**: `formats/CRgdFile.h/cpp`, `formats/CRgdFileMacro.h/cpp`, `formats/CChunkyFile.h/cpp`, `formats/CRgtFile.h/cpp`, `formats/CRgmFile.h/cpp`, `formats/CBfxFile.h/cpp`, `formats/IMetaTable.h/cpp`, `formats/CRgdHashTable.h/cpp`

Parsers for Relic's proprietary binary formats. Most follow a `Load(IStream*)` / `Save(IOutputStream*)` pattern. `CRgdFile` is the most complex — it includes nested `CMetaNode`/`CMetaTable` inner classes and bidirectional conversion with `CLuaFile`.

#### Localization Subsystem
**Files**: `localization/CUcsFile.h/cpp`, `localization/CCohUcsFile.h/cpp`, `localization/CUcsTransaction.h`

UCS (Unicode Character Set) files store localized strings keyed by numeric ID. `CUcsFile` handles DoW format (UTF-16LE), `CCohUcsFile` handles CoH's variant (dead code — `#if 0`). Dollar-string resolution (`$12345`) maps IDs to display text.

#### Lua Integration Subsystem
**Files**: `lua/CLuaFile.h/cpp`, `lua/CLuaFile2.h/cpp`, `lua/CLuaScript.h/cpp`, `lua/CLuaScript_Interface.h/cpp`, `lua/CLuaFromRgd.h/cpp`, `lua/CLuaFileCache.h/cpp`, `lua/CInheritTable.h/cpp`, `lua/luax.h/cpp`, `lua/luax51.cpp`, `lua/Lua51.h`

The most complex subsystem. Two vendored Lua runtimes coexist:
- **Lua 5.0.2** (`vendor/lua502/`): Native symbol names. Used by `CLuaFile`, `CLuaFile2`, `CLuaScript`, `CRgdFile::CreateLuaState()`.
- **Lua 5.1.2** (`vendor/lua512/`): All symbols renamed via `lua512_rename.h` (`lua_` → `lua51_`). Exposed through `lua/Lua51.h`. Used by `CRgdFileMacro`.

`CLuaFile` is the primary Lua data file parser implementing `IMetaTable`. `CInheritTable` resolves Relic's three-way inheritance system (`Inherit()`, `Reference()`, `GameData()`). `CLuaFromRgd` converts RGD binary data back to Lua source.

#### Module Subsystem
**Files**: `module/CModuleFile.h/cpp`, `module/CModuleMetadata.h`, `module/CModuleParser.h/cpp`, `module/CResourceLoader.h/cpp`, `module/CDoWModule.h/cpp`, `module/CDoWFileView.h/cpp`, `module/CFileMap.h/cpp`

The highest-level subsystem. `CModuleFile` is the façade — it parses `.module` files and aggregates all data sources (filesystem folders, SGA archives, UCS files, required mods) into a single `IFileStore` + `IDirectoryTraverser` interface. Its responsibilities are decomposed into:
- **`CModuleMetadata`** — RAII struct holding parsed directive fields
- **`CModuleParser`** — INI parsing and module type heuristics
- **`CResourceLoader`** — Resource loading orchestration (archives, UCS files, required mods)

`CDoWModule` scans game installations to discover available mods.

`CModuleFile.h` uses forward declarations for `CSgaFile`, `CUcsFile`, `CDoWFileView`, `CFileMap`, and `CFileSystemStore` to minimize header coupling.

#### Utilities
**Files**: `core/Exception.h/cpp`, `core/RainmanLog.h/cpp`, `core/Callbacks.h/cpp`, `core/Api.h`, `core/gnuc_defines.h/cpp`, `core/memdebug.h`, `core/Internal_Util.h/cpp`, `core/WriteTime.h/cpp`, `core/pch_rainman.h`, `util/Util.h/cpp`, `util/crc32_case_idt.h/c`, `util/hash.c`, `util/md5.h/c`

Cross-cutting concerns. `CRainmanException` provides precursor-chained exceptions with `ExceptionDeleter` for RAII cleanup. `RainmanLog` wraps spdlog. `CRgdHashTable` maps human-readable property names to Jenkins hashes. `Api.h` defines the `RAINMAN_API` export macro.

### 3.3 Class Inheritance Hierarchy

```
CRainmanException                    (base exception, heap-allocated)
└── CModStudioException              (CDMS-specific, in src/cdms/)

ExceptionDeleter                     (RAII: std::unique_ptr<CRainmanException, ExceptionDeleter>)

IFileStore                           (stream I/O interface)
├── CFileSystemStore                 (also: IDirectoryTraverser)
│   ├── CStream : IFileStore::IStream
│   └── COutputStream : IFileStore::IOutputStream
├── CMemoryStore
│   ├── CStream : IFileStore::IStream
│   └── COutStream : IFileStore::IOutputStream
├── CSgaFile                         (also: IDirectoryTraverser)
│   ├── CStream : IFileStore::IStream
│   └── CIterator : IDirectoryTraverser::IIterator
├── CModuleFile                      (also: IDirectoryTraverser)
│   ├── CModuleMetadata              (parsed directive fields, RAII struct)
│   ├── CModuleParser                (INI parsing + module type heuristics)
│   └── CResourceLoader             (resource loading orchestration)
├── CDoWModule                       (also: IDirectoryTraverser)
├── CDoWFileView                     (also: IDirectoryTraverser)
│   └── CIterator : IDirectoryTraverser::IIterator
└── CFileMap                         (also: IDirectoryTraverser)
    └── CIterator : IDirectoryTraverser::IIterator

IMetaNode                            (game data node interface)
├── CRgdFile                         (binary RGD data)
│   ├── CMetaNode : (nested)
│   └── CMetaTable : (nested)
│   └── CBfxFile : CRgdFile          (binary effects, inherits RGD)
└── IMetaNode::IMetaTable : IMetaNode
    └── CLuaFile                     (Lua game data)

wxApp
└── CDMSApplication                  (CDMS entry point)

wxFrame
├── ConstructFrame                   (main window — delegates to helper classes)
│   ├── owns: ModuleManager          (CModuleFile* lifecycle, services, hash table)
│   ├── owns: TabManager             (wxAuiNotebook tab management)
│   ├── owns: MenuController         (menu bar construction)
│   └── owns: ToolRegistry           (ITool registration + dispatch)
├── frmLoading                       (progress/splash)
└── frmMessage                       (message display)

ITool                                (standalone tool plugin interface, was nested in ConstructFrame)
├── CLocaleTool, CAESetupTool, CUcsTool, CAttribSnapshotTool
├── CSgaPackerTool, CExtractAllTool, CDpsCalculatorTool
├── CRedButtonTool, CMakeLuaInheritTree, CRefreshFilesTool

wxWindow
├── frmFiles                         (file tree browser)
│   ├── IHandler                     (file action interface)
│   │   ├── CLuaAction, CTextViewAction, CExtractAction, ...
│   │   └── (32 action handler files in src/cdms/actions/)
│   └── CFilesTreeItemData : wxTreeItemData
├── frmWelcome, frmLuaInheritTree, frmImageViewer
├── frmRGDEditor, frmRgmMaterialEditor
├── frmUCSEditor, frmScarEditor
└── frmModule
    ├── pgMain : wxWindow
    ├── pgDataFolders : wxWindow
    ├── pgDataArchives : pgDataFolders
    ├── pgRequiredMods : pgDataFolders
    └── pgCompatibleMods : pgDataFolders

wxDialog
├── frmCredits, frmFileSelector, frmLocaleSelector
├── frmNewMod, frmMassExtract, frmRgdMacro
├── frmSgaMake, frmUCSSelector, frmUCSOutOfRange
├── frmTabDialog
└── frmUCSToDAT                      (in Tool_AESetup.h)
```

### 3.4 Exception Handling

Exceptions use a custom heap-allocated pattern, **not** standard C++ exceptions:

```cpp
// Throwing (always heap-allocated with new):
throw new CRainmanException(__FILE__, __LINE__, "message");
throw new CRainmanException(pPrecursor, __FILE__, __LINE__, "format %s", arg);

// Convenience macros:
QUICK_THROW("message")              // throws new CRainmanException
CATCH_THROW("context")              // catch(...) { throw new ... with precursor }
CHECK_MEM(ptr)                       // if (!ptr) throw new ... "out of memory"
IGNORE_EXCEPTIONS                    // catch CRainmanException* and destroy() via ExceptionDeleter

// RAII cleanup (preferred):
std::unique_ptr<CRainmanException, ExceptionDeleter> guard(pException);
// auto-calls pException->destroy() on scope exit

// Manual cleanup (legacy):
pException->destroy();               // walks precursor chain, frees all
```

Exceptions form a linked list via `m_pPrecursor`, enabling stack-trace-like error chains. The `destroy()` method recursively frees the entire chain. `ExceptionDeleter` (defined in `core/Exception.h`) wraps this for `std::unique_ptr` usage.

### 3.5 Callback System

Long-running operations accept a status callback for progress reporting:

```cpp
typedef void (*pfnStatusCallback)(const char* sMessage, void* pToken);

// Usage with macros:
void CModuleFile::LoadModuleFile(const char* sFileName, CALLBACK_ARG_OPT);
// CALLBACK_ARG_OPT expands to: pfnStatusCallback fnStatusCallback = 0, void* fnStatusCallbackTag = 0

// Reporting progress:
CallCallback(THE_CALLBACK, "Loading archive %s...", sArchiveName);
```

### 3.6 Logging

```cpp
// Initialization (once at startup):
RainmanLog::init("logs");

// Usage via macros:
RAINMAN_LOG_INFO("Loaded {} files", count);
RAINMAN_LOG_WARN("Missing archive: {}", name);
CDMS_LOG_ERROR("Failed to open module");
```

Wraps spdlog with file + console sinks. Separate loggers for Rainman and CDMS.

---

## 4. CDMS GUI Application

### 4.1 Application Lifecycle

```
CDMSApplication::OnInit()
├── RainmanLog::init()
├── wxFileConfig setup (absolute path to avoid APPDATA prefix)
├── Create ConstructFrame (maximized)
│   ├── Menu bar (File, Mod, Tools, Play, Help)
│   ├── wxSplitterWindow
│   │   ├── Left: frmFiles (file tree)
│   │   └── Right: wxAuiNotebook (tabbed editors)
│   ├── Register 10 ITool plugins
│   └── Show frmWelcome in notebook
└── SetTopWindow(ConstructFrame)

CDMSApplication::OnRun()
├── wxApp::OnRun() with CRainmanException catch
└── On exception: MessageBox with error chain

CDMSApplication::OnExit()
└── wxFileConfig::Flush()
```

### 4.2 Main Frame (ConstructFrame)

The main frame delegates responsibilities to focused helper classes:

- **`ModuleManager`** — owns the `CModuleFile*` lifecycle, service layer instances (`ModuleService`, `FileService`, `HashService`), and `CRgdHashTable*`
- **`TabManager`** — owns the `wxAuiNotebook` tab container
- **`MenuController`** — builds the menu bar (receives `wxFrame*` and `ToolRegistry&`)
- **`ToolRegistry`** — registers and dispatches `ITool` plugins; owns tool lifetime
- `frmFiles` — file browser panel (left splitter pane)
- `frmLoading` — splash/progress frame

Key members exposed globally via `TheConstruct` macro:
```cpp
CModuleFile* GetModule();
CRgdHashTable* GetRgdHashTable();
wxAuiNotebook* GetTabs();
frmFiles* GetFilesList();
// Service accessors (via ModuleManager):
ModuleService& GetModuleService();
FileService& GetFileService();
HashService& GetHashService();
```

### 4.3 File Browser (frmFiles + actions/)

`frmFiles` renders the mod's directory tree using `wxTreeCtrl`, traversing `IDirectoryTraverser::IIterator`. On file selection, it dispatches to one of 32 action handlers via the `IHandler` interface:

```cpp
class IHandler {
    virtual wxString VGetExt() const = 0;     // file extension to match
    virtual wxString VGetAction() const = 0;  // display name
    virtual void VHandle(wxString sFile, wxTreeItemId& oId, wxTreeItemId& oFileRoot) = 0;
};
```

Each handler implementation lives in its own header file under `src/cdms/actions/` (e.g., `CRGDAction.h`, `CTextViewAction.h`, `CExtractAction.h`). A shared `ActionUtil.h` provides the `OnlyFilename()` helper. Handlers access Rainman via service layer — e.g., `TheConstruct->GetFileService()`.

### 4.4 CDMS → Rainman Coupling

CDMS accesses Rainman primarily through the **service layer** (`src/cdms/services/`). Each service wraps a Rainman subsystem and translates between Rainman types (`char*`, `CRainmanException*`) and wx types (`wxString`, error strings). Services use `Result<T>` (tag-dispatch based) for error handling.

```
cdms/services/
├── Result.h             # Result<T> with OkTag/ErrTag, ResultFromException()
├── ModuleService.h/cpp  # Wraps CModuleFile (load, save, properties)
├── FileService.h/cpp    # Wraps IFileStore/IDirectoryTraverser (open, iterate)
├── FormatService.h/cpp  # Wraps format parsers (RGD, Chunky, RGT, RGM, BFX, UCS)
└── HashService.h/cpp    # Wraps CRgdHashTable with lazy init
```

`Common.h` no longer includes any Rainman headers. Each CDMS `.cpp` file includes only the specific Rainman headers it needs (directory-qualified, e.g., `#include <rainman/module/CModuleFile.h>`).

### 4.5 Configuration System

```cpp
// config.h wraps wxConfigBase
#define TheConfig wxConfigBase::Get()

// Usage:
wxColour c = ConfGetColour(TheConfig, "Editor/Background", wxColour(255,255,255));
wxString path = ConfGetDoWFolder(TheConfig);  // falls back to Rainman detection
```

State persisted to an INI file via `wxFileConfig`. Includes game installation paths, editor colors, splitter positions, and window sizes. Real-time writes on event handlers; flushed on exit.

### 4.6 String Handling

The codebase bridges three string worlds:
- **Rainman**: `char*` (ANSI/Latin-1), `wchar_t*` (for UCS)
- **wxWidgets**: `wxString` (Unicode-aware)
- **Game data**: UTF-16LE (UCS files), ASCII (Lua/module files)

`strconv.h/cpp` provides conversion functions:
```cpp
wchar_t* AsciiToUnicode(const char* sAscii);
char* UnicodeToAscii(const wchar_t* wsUnicode);
wxString AsciiTowxString(const char* sAscii);
std::unique_ptr<char[]> wxStringToAscii(const wxString& sString);  // RAII, no manual delete
```

**Remaining issue**: No UTF-8 support; ANSI/Latin-1 assumed throughout.

### 4.7 Tool Plugin System

```cpp
// ITool.h — standalone interface (was nested in ConstructFrame)
class ITool {
    virtual const wxString& GetName() = 0;
    virtual const wxString& GetHelpString() = 0;
    virtual const wxString& GetBitmapName() = 0;
    virtual bool DoAction(ConstructFrame* pFrame) = 0;
};
// Backward compat: ConstructFrame::ITool = ::ITool (via using declaration)
```

10 tools registered in `Construct.cpp` constructor:
1. `CLocaleTool` — language selector
2. `CAESetupTool` — area effect setup
3. `CUcsTool` — UCS editor launcher
4. `CAttribSnapshotTool` — attribute snapshot
5. `CSgaPackerTool` — SGA compression
6. `CExtractAllTool` — bulk extraction
7. `CDpsCalculatorTool` — DPS calculator (outputs HTML)
8. `CRedButtonTool` — (unused/placeholder)
9. `CMakeLuaInheritTree` — inheritance visualization
10. `CRefreshFilesTool` — file tree refresh

Tools are dispatched via `ToolRegistry` using array-driven dispatch: a single `OnToolMenuCommand(wxCommandEvent&)` handler extracts the tool index from `IDM_ModToolBase + index` event IDs. Up to 100 tool slots are reserved.

---

## 5. Build System & Dependencies

### 5.1 CMake Structure

```
CMakeLists.txt (top-level)
├── src/rainman/CMakeLists.txt
│   ├── lua502 (STATIC library, 28 .c files, in vendor/lua502/)
│   ├── lua512 (STATIC library, 28 .c files, /FI lua512_rename.h, in vendor/lua512/)
│   └── rainman (STATIC library, GLOB **/*.cpp **/*.c across subdirs, PCH pch_rainman.h)
├── src/cdms/CMakeLists.txt
│   └── CorsixModStudio (WIN32 EXE, GLOB *.cpp *.c + actions/*.h + resource.rc)
├── tests/rainman/CMakeLists.txt
│   └── rainman_tests (EXE, explicit file list, gtest_discover_tests)
└── tests/cdms/CMakeLists.txt
    └── cdms_tests (EXE, explicit file list + CDMS sources compiled in)
```

### 5.2 Dependencies

| Dependency | Version | Source | Used By |
|-----------|---------|--------|---------|
| zlib | 1.3.1 | vcpkg | Rainman (SGA decompression) |
| Google Test | 1.17 | vcpkg | Tests |
| libsquish | latest | vcpkg | Rainman (DXT texture compression) |
| wxWidgets | 3.x | vcpkg | CDMS (GUI framework) |
| spdlog | latest | vcpkg | Rainman (logging) |
| Lua 5.0.2 | 5.0.2 | vendored | Rainman (game data compat) |
| Lua 5.1.2 | 5.1.2 | vendored | Rainman (macro system, renamed symbols) |

### 5.3 Build Presets

```powershell
cmake --preset default        # VS generator, vcpkg integration
cmake --build --preset debug  # Debug build
cmake --build --preset release # Release build
ctest --preset debug          # Run all tests
cmake --preset tidy           # Ninja + clang-tidy
cmake --build --preset tidy-debug  # Build with static analysis
```

### 5.4 Precompiled Header

`pch_rainman.h` includes: `Exception.h`, `memdebug.h`, `RainmanLog.h`, plus standard library headers. C files (`crc32_case_idt.c`, `hash.c`, `md5.c`) skip the PCH.

---

## 6. Test Infrastructure

### 6.1 Framework

Google Test with `gtest_discover_tests()` for auto-discovery. Single `test_main.cpp` entry point initializes `RainmanLog` at WARN level. Tests run in parallel via `ctest -j`.

### 6.2 Test Coverage

**Rainman tests** (26 test files in `tests/rainman/`):

| Test File | Tested Class | Depth | Pattern |
|-----------|-------------|-------|---------|
| `exception_test.cpp` | `CRainmanException` | Good | Chaining, macros, formatting |
| `exceptiondeleter_test.cpp` | `ExceptionDeleter` | Good | RAII cleanup, unique_ptr integration |
| `memorystore_test.cpp` | `CMemoryStore` | Good | Read/write streams, seeks |
| `sgafile_test.cpp` | `CSgaFile` | Partial | Error handling only (no valid archives) |
| `sgacreator_test.cpp` | `CSgaCreator` | Good | Round-trip create → read → verify |
| `rgdfile_test.cpp` | `CRgdFile` | Good | Create, save/load round-trips, nested tables |
| `chunkyfile_test.cpp` | `CChunkyFile` | Good | Nested chunks, save/load, metadata |
| `rgdhashtable_test.cpp` | `CRgdHashTable` | Good | Hash mapping, numeric keys |
| `rgtfile_test.cpp` | `CRgtFile` | Good | TGA load, DXT codec, save/reload |
| `rgmfile_test.cpp` | `CRgmFile` | Good | Load from CMemoryStore, material properties |
| `cbfxfile_test.cpp` | `CBfxFile` | Good | RGD extension, Lua export |
| `ucsfile_test.cpp` | `CUcsFile` | Good | Strings, load/resolve, dollar IDs |
| `modulefile_test.cpp` | `CModuleFile` | Good | Config parsing, versions, archives, decomposition |
| `filesystemstore_test.cpp` | `CFileSystemStore` | Partial | Read/write operations |
| `inherittable_test.cpp` | `CInheritTable` | Good | Parentage, orphan assignment |
| `luafile_test.cpp` | `CLuaFile` | Good | IMetaTable interface via CMemoryStore |
| `luafile2_test.cpp` | `CLuaFile2` | Good | Load Lua script, verify metadata |
| `luascript_test.cpp` | `CLuaScript` | Good | Load/execute scripts, error handling |
| `luafromrgd_test.cpp` | `CLuaFromRgd` | Good | RGD→Lua conversion round-trip |
| `callbacks_test.cpp` | `Callbacks` | Good | Printf-style formatting |
| `crc32_case_idt_test.cpp` | `crc32_case_idt` | Good | Hash validation |
| `writetime_test.cpp` | `WriteTime` | Good | Timestamp handling |
| `internal_util_test.cpp` | `Internal_Util` | Good | String operations |
| `md5_test.cpp` | `md5` | Good | RFC 1321 compliance |
| `directorytraverser_test.cpp` | `IDirectoryTraverser` | Partial | Directory operations |

**CDMS tests** (10 test files in `tests/cdms/`):

| Test File | Tested Class | Depth | Pattern |
|-----------|-------------|-------|---------|
| `result_test.cpp` | `Result<T>` | Good | Ok/Err construction, tag dispatch |
| `moduleservice_test.cpp` | `ModuleService` | Partial | Service wrapper, error paths |
| `fileservice_test.cpp` | `FileService` | Partial | Stream/iterator wrapping |
| `formatservice_test.cpp` | `FormatService` | Partial | Format parser wrapping |
| `hashservice_test.cpp` | `HashService` | Partial | Lazy init, hash lookup |
| `config_test.cpp` | `ConfGetColour` | Good | Color retrieval with defaults |
| `strings_test.cpp` | UI strings | Partial | String constant checks |
| `strconv_test.cpp` | `strconv` | Partial | String conversion |
| `utility_test.cpp` | `Utility` | Partial | Helper functions |

**Total: 338 tests passing** (36 test files: 26 Rainman + 10 CDMS).

### 6.3 Remaining Coverage Gaps

Most Rainman classes now have test coverage (~85% class-level). The following remain untested:

| Class | Risk | Reason |
|-------|------|--------|
| `CDoWModule` | **High** | Requires real game installation; 1,969 lines |
| `CLuaScript_Interface` | N/A | Dead code (`#if 0`) |
| `CCohUcsFile` | N/A | Dead code (`#if 0`) |
| `CDoWFileView` | **Low** | Virtual file view, depends on game data |
| `CFileMap` | **Low** | File index (980 lines but lower risk) |
| `CLuaFileCache` | **Low** | Caching layer |
| `CUcsTransaction` | **Low** | Transaction wrapper |

**Testing patterns used**:
- `CMemoryStore` for in-memory I/O (avoids filesystem)
- Round-trip testing (save → load → verify)
- Temporary directory testing with cleanup
- Exception macro verification

---

## 7. Include Dependency Graph

All Rainman includes now use directory-qualified paths with the `rainman/` prefix (e.g., `#include "rainman/core/Exception.h"`).

### 7.1 Rainman Internal Dependencies

```
Foundation (no project deps):
  core/gnuc_defines.h
  core/WriteTime.h → core/gnuc_defines.h
  core/Api.h → core/gnuc_defines.h
  core/RainmanLog.h → core/Api.h
  lua/Lua51.h
  util/crc32_case_idt.h
  core/memdebug.h

Core interfaces:
  io/IFileStore.h → core/gnuc_defines.h, core/Api.h
  core/Exception.h → core/gnuc_defines.h, core/Api.h, <memory> (ExceptionDeleter)
  io/IDirectoryTraverser.h → core/gnuc_defines.h, io/IFileStore.h, core/WriteTime.h, core/Api.h
  formats/IMetaTable.h → core/gnuc_defines.h, core/Api.h, io/CMemoryStore.h

Stores:
  io/CMemoryStore.h → core/gnuc_defines.h, io/IFileStore.h
  io/CFileSystemStore.h → core/gnuc_defines.h, io/IFileStore.h, io/IDirectoryTraverser.h, core/Api.h
  archive/CSgaFile.h → core/gnuc_defines.h, io/IFileStore.h, io/IDirectoryTraverser.h, io/CMemoryStore.h, core/Api.h

Format parsers:
  formats/CChunkyFile.h → core/gnuc_defines.h, io/CMemoryStore.h, core/Exception.h
  formats/CRgdFile.h → core/gnuc_defines.h, io/IFileStore.h, formats/IMetaTable.h, formats/CRgdHashTable.h, lua/CLuaFile.h, core/Api.h
  formats/CBfxFile.h → formats/CRgdFile.h
  localization/CUcsFile.h → core/gnuc_defines.h, io/IFileStore.h, core/Api.h

Lua:
  lua/CLuaFile.h → core/gnuc_defines.h, formats/IMetaTable.h, io/IFileStore.h, lua/CLuaFileCache.h
  lua/CLuaFile2.h → core/gnuc_defines.h, formats/IMetaTable.h, io/IFileStore.h, lua/CLuaFileCache.h

High-level (uses forward declarations to minimize coupling):
  module/CModuleFile.h → core/gnuc_defines.h, io/IFileStore.h, io/IDirectoryTraverser.h,
                          core/Api.h, core/Callbacks.h, module/CModuleMetadata.h
                          + forward decls: CSgaFile, CUcsFile, CDoWFileView, CFileMap, CFileSystemStore
  module/CDoWModule.h → core/gnuc_defines.h, io/CFileSystemStore.h, module/CDoWFileView.h,
                         archive/CSgaFile.h, localization/CUcsFile.h, core/Callbacks.h, core/Api.h
  formats/CRgdFileMacro.h → lua/Lua51.h, formats/CRgdFile.h + forward decl: CModuleFile
  lua/CLuaFromRgd.h → core/gnuc_defines.h, io/IFileStore.h, formats/IMetaTable.h,
                       formats/CRgdHashTable.h, lua/CLuaFile.h, core/Api.h
                       + forward decls: CRgdFile, CModuleFile
```

### 7.2 CDMS Dependencies

```
Foundation:
  config.h, strconv.h, strings.h, Application.h → (no Rainman deps)

Utilities:
  Utility.h → strconv.h, strings.h

Service layer:
  services/Result.h → (standalone, no Rainman deps)
  services/ModuleService.h → Result.h + forward decl: CModuleFile
  services/FileService.h → Result.h + forward decls: CModuleFile, IFileStore, IDirectoryTraverser
  services/FormatService.h → Result.h + forward decls
  services/HashService.h → Result.h + forward decl: CRgdHashTable

Structural helpers:
  ITool.h → (standalone interface)
  ToolRegistry.h → ITool.h
  MenuController.h → ToolRegistry.h
  ModuleManager.h → services/*.h
  TabManager.h → (wxAuiNotebook wrapper)

Mid-level:
  frmLoading.h, frmLuaInheritTree.h → (no Rainman deps)
  frmFiles.h → frmLuaInheritTree.h
  Construct.h → frmLoading.h, TabManager.h, frmFiles.h, <rainman/module/CModuleFile.h>,
                ModuleManager.h, ITool.h, ToolRegistry.h, MenuController.h

Common.h → Construct.h, Utility.h, strconv.h, strings.h
  (NO Rainman.h — each .cpp includes only the specific Rainman headers it needs)
```

### 7.3 Key Observations

1. **`Rainman.h` deleted**: The god header that pulled in 15 Rainman headers no longer exists. Each CDMS file includes only what it needs.

2. **`Common.h` is lightweight**: No longer includes any Rainman headers. Just `Construct.h`, `Utility.h`, `strconv.h`, `strings.h`.

3. **`CModuleFile.h` uses forward declarations**: Reduced from 8 Rainman includes to 5 core includes + forward declarations for `CSgaFile`, `CUcsFile`, `CDoWFileView`, `CFileMap`, `CFileSystemStore`.

4. **No circular includes**: The dependency graph remains strictly acyclic. `CRgdFile` ↔ `CLuaFile` have a logical circular dependency (bidirectional conversion) but the include graph is one-way.

5. **Service layer decouples CDMS from Rainman headers**: Service headers use forward declarations; only the `.cpp` files include the actual Rainman headers.

---

## 8. Technical Debt Inventory

### 8.1 Structural Issues (Resolved)

All structural issues from the original assessment have been resolved:

| ID | Issue | Resolution |
|----|-------|------------|
| ~~S1~~ | God header `Rainman.h` | **Deleted.** Each file includes only what it needs. |
| ~~S2~~ | `Common.h` pulls Rainman.h | **Fixed.** `Common.h` no longer includes any Rainman headers. |
| ~~S3~~ | God class `CModuleFile` (1,608 LOC) | **Decomposed** to 775 LOC. Extracted `CModuleMetadata`, `CModuleParser`, `CResourceLoader`. |
| ~~S4~~ | God class `ConstructFrame` (983 LOC) | **Decomposed** to 718 LOC. Extracted `ModuleManager`, `TabManager`, `MenuController`, `ToolRegistry`. |
| ~~S5~~ | Monster file `frmFiles_Actions.h` (2,201 LOC) | **Deleted.** Split into 32 individual action files in `src/cdms/actions/`. |
| ~~S6~~ | No GUI↔Data abstraction layer | **Service layer added** (`ModuleService`, `FileService`, `FormatService`, `HashService`). |
| ~~S7~~ | Forward declarations underused | **Fixed.** `CModuleFile.h`, `CLuaFromRgd.h`, `CFileMap.h`, `CRgdFileMacro.h` use forward declarations. |
| ~~S8~~ | Flat directory structure | **Reorganized** into 8 subdirectories (`core/`, `io/`, `archive/`, `formats/`, `lua/`, `localization/`, `module/`, `util/`). |

### 8.2 Memory & Safety Issues (Mostly Resolved)

| ID | Issue | Status |
|----|-------|--------|
| ~~M1~~ | Manual stream ownership | **Fixed.** `std::unique_ptr<IFileStore::IStream>` used throughout. |
| ~~M2~~ | Manual `destroy()` for exceptions | **Fixed.** `ExceptionDeleter` enables `std::unique_ptr<CRainmanException, ExceptionDeleter>`. |
| ~~M3~~ | Raw `new[]`/`delete[]` string conversions | **Fixed.** `wxStringToAscii` returns `std::unique_ptr<char[]>`. |
| ~~M4~~ | No RAII for any resource type | **Fixed.** RAII added for streams, exceptions, and string conversions. |
| M5 | **`unsigned long` instead of `size_t`** in APIs | Low | Remains — changing would cascade through all interfaces. Safe for game data sizes. |

### 8.3 Code Quality Issues (Mostly Resolved)

| ID | Issue | Status |
|----|-------|--------|
| ~~Q1~~ | 20 duplicate `LaunchModToolN()` handlers | **Fixed.** Array-driven dispatch via `ToolRegistry` with single `OnToolMenuCommand()`. |
| Q2 | **No UTF-8 support** in string conversions | Medium | Remains — ANSI/Latin-1 assumed throughout. |
| Q3 | **Bidirectional RGD↔Lua dependency** | Medium | Remains — `CRgdFile` ↔ `CLuaFile` logical circular dependency. Include graph is one-way. |
| Q4 | **Windows-only APIs** (`LoadLibraryW`, `_wfopen`, etc.) | Low | Remains — acceptable, cross-platform is a non-goal. |
| ~~Q5~~ | ~55% of classes untested | **Fixed.** ~85% class-level coverage (338 tests, 36 test files). |

---

## 9. Key Design Patterns

### 9.1 Naming Conventions

| Convention | Example | Meaning |
|-----------|---------|---------|
| `C` prefix | `CSgaFile` | Concrete class |
| `I` prefix | `IFileStore` | Interface (abstract base class) |
| `m_` prefix | `m_sMessage` | Member variable |
| `m_s` | `m_sFileName` | String member |
| `m_i` | `m_iLineCount` | Integer member |
| `m_p` | `m_pStream` | Pointer member |
| `m_b` | `m_bInited` | Boolean member |
| `V` prefix | `VOpenStream` | Virtual method |
| `S` prefix | `SGetNodeFromRainmanRgd` | Static-like method (on instance) |
| `frm` prefix | `frmRgdEditor` | GUI frame/window class |
| `pg` prefix | `pgMain` | Page within a tabbed frame |

### 9.2 Resource Lifecycle

```
Create → VInit() → Use → destructor
         ↑                    ↑
         Required before      delete or scope exit
         any operation
```

Most classes require `VInit()` before use. Some accept init data via `void* pInitData`. RAII wrappers exist for streams (`std::unique_ptr<IStream>`), exceptions (`ExceptionDeleter`), and string conversions (`std::unique_ptr<char[]>`), but the underlying Rainman classes still use manual lifecycle management.

### 9.3 File Format Load/Save

```cpp
// Modern pattern (preferred):
CRgdFile rgd;
std::unique_ptr<IFileStore::IStream> pStream(store.VOpenStream("data.rgd"));
rgd.Load(pStream.get());      // parser does NOT take ownership
// stream auto-deleted on scope exit

// Save:
std::unique_ptr<IFileStore::IOutputStream> pOut(store.VOpenOutputStream("data.rgd", true));
rgd.Save(pOut.get());          // parser does NOT take ownership
// stream auto-deleted on scope exit

// Legacy pattern (still exists in some code):
IFileStore::IStream* pStream = store.VOpenStream("data.rgd");
rgd.Load(pStream);
delete pStream;
```

### 9.4 CMemoryStore in Tests

```cpp
CMemoryStore store;
store.VInit();

// Wrap raw data as a readable stream:
char* pRange = store.MemoryRange((void*)data, dataLen);
std::unique_ptr<IFileStore::IStream> pStream(store.VOpenStream(pRange));
// ... read from pStream.get() ...

// Or use static helpers:
std::unique_ptr<IFileStore::IStream> pStream(CMemoryStore::OpenStreamExt(pData, len, false));
std::unique_ptr<IFileStore::IOutputStream> pOutStream(CMemoryStore::OpenOutputStreamExt());
```

---

## 10. Glossary

| Term | Definition |
|------|-----------|
| **BFX** | Binary effects file — extends RGD format |
| **CDMS** | Corsix Dawn of War Mod Studio — the GUI application |
| **Chunky** | Relic's generic container format with FOLD (directory) and DATA (payload) chunks |
| **CoH** | Company of Heroes |
| **DoW** | Dawn of War |
| **DXT** | DirectX Texture Compression (DXT1, DXT3, DXT5) |
| **God class/header** | A class or header with too many responsibilities/dependencies |
| **Jenkins hash** | Bob Jenkins' lookup hash — used by RGD for property name hashing |
| **Rainman** | The core I/O library (named after a Corsix convention) |
| **RGD** | Relic Game Data — binary key-value format for game tuning |
| **RGM** | Relic Game Material — material definition format |
| **RGT** | Relic Game Texture — texture container (wraps DDS/TGA with mip-maps) |
| **SCAR** | Scripting At Relic — Lua-based scripting language for DoW |
| **SGA** | Relic's game archive format (versions 2 and 4) |
| **UCS** | Unicode Character Set — localization string table |
| **`.module`** | Mod descriptor file (INI-like format defining a mod's structure) |
