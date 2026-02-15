# CorsixModStudio — Architecture Document

> **Status**: Living document — captures the as-is architecture as of Phase 5 completion.
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
│            (wxWidgets, 14.7k lines, GPL v2)             │
│                                                         │
│  ConstructFrame ─┬─ frmFiles (file browser)             │
│                  ├─ frmRgdEditor (RGD editor)           │
│                  ├─ frmScarEditor (script editor)       │
│                  ├─ frmUCSEditor (localization editor)   │
│                  ├─ frmModule (mod settings)             │
│                  ├─ frmSgaMake (archive builder)         │
│                  └─ 10 ITool plugins                    │
│                                                         │
│  ┌────────────── NO ABSTRACTION LAYER ──────────────┐   │
│  │  GUI classes directly call Rainman types          │   │
│  └──────────────────────────────────────────────────┘   │
├─────────────────────────────────────────────────────────┤
│                    Rainman Library                       │
│           (static lib, 26.5k lines, LGPL v2.1)          │
│                                                         │
│  ┌──────────┐  ┌──────────────┐  ┌──────────────────┐  │
│  │  File I/O │  │Format Parsers│  │ Lua Integration  │  │
│  │           │  │              │  │                  │  │
│  │ IFileStore│  │ CRgdFile     │  │ CLuaFile (5.0.2) │  │
│  │ IStream   │  │ CChunkyFile  │  │ CLuaFile2(5.0.2) │  │
│  │ CFileSys  │  │ CRgtFile     │  │ CLuaScript       │  │
│  │ CMemory   │  │ CRgmFile     │  │ CLuaFromRgd      │  │
│  │ CSgaFile  │  │ CBfxFile     │  │ CInheritTable    │  │
│  └──────────┘  │ CUcsFile     │  │ Lua51 (5.1.2)    │  │
│                │ CCohUcsFile  │  └──────────────────┘  │
│  ┌──────────┐  └──────────────┘                        │
│  │  Module   │  ┌──────────────┐                        │
│  │           │  │  Utilities   │                        │
│  │CModuleFile│  │              │                        │
│  │CDoWModule │  │ CRgdHashTbl  │                        │
│  │CDoWFileVw │  │ CSgaCreator  │                        │
│  │ CFileMap  │  │ Callbacks    │                        │
│  └──────────┘  │ RainmanLog   │                        │
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
| Rainman library (`src/rainman/*.cpp,*.h,*.c`) | 26,579 | ~80 | C++/C |
| CDMS application (`src/cdms/*.cpp,*.h`) | 14,680 | ~60 | C++ |
| Tests (`tests/**/*.cpp`) | 1,949 | 23 | C++ |
| Vendored Lua 5.0.2 (`lua502/`) | 11,384 | ~40 | C |
| Vendored Lua 5.1.2 (`lua512src/`) | 13,601 | ~40 | C |
| **Total (our code)** | **43,208** | **~163** | |
| **Total (including vendored)** | **68,193** | **~243** | |

### Largest Files (>500 LOC)

| File | Lines | Layer | Purpose |
|------|-------|-------|---------|
| `frmFiles_Actions.h` | 2,201 | CDMS | 30+ file action handler classes (inline) |
| `CRgdFile.cpp` | 1,982 | Rainman | Binary game data format parser |
| `CDoWModule.cpp` | 1,969 | Rainman | Dawn of War module enumeration & loading |
| `CRgdFileMacro.cpp` | 1,616 | Rainman | RGD macro application via Lua 5.1.2 |
| `CModuleFile.cpp` | 1,608 | Rainman | .module file parser and mod manager |
| `CLuaFile.cpp` | 1,582 | Rainman | Lua 5.0.2 game data file parser |
| `frmRgdEditor.cpp` | 1,539 | CDMS | RGD property grid editor |
| `CLuaFile2.cpp` | 1,306 | Rainman | Alternative Lua file parser |
| `CLuaFromRgd.cpp` | 1,264 | Rainman | RGD → Lua conversion |
| `CRgtFile.cpp` | 1,072 | Rainman | Texture format (DXT/DDS/TGA) |
| `Construct.cpp` | 983 | CDMS | Main frame (menu, tools, mod loading) |
| `CFileMap.cpp` | 980 | Rainman | File index/map across archives |
| `CSgaFile.cpp` | 966 | Rainman | SGA archive reader |
| `CLuaScript_Interface.cpp` | 890 | Rainman | Lua scripting interface |
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

#### File I/O Subsystem
**Files**: `IFileStore.h/cpp`, `CFileSystemStore.h/cpp`, `CMemoryStore.h/cpp`, `IDirectoryTraverser.h/cpp`

Foundation layer. Provides the polymorphic stream abstraction that all format parsers and higher-level stores use. `CMemoryStore` is also the primary test helper for in-memory I/O.

#### Archive Subsystem
**Files**: `CSgaFile.h/cpp`, `CSgaCreator.h/cpp`

Reads and creates Relic's SGA archives. `CSgaFile` supports both v2 (DoW) and v4 (CoH) formats with zlib decompression, MD5 validation, and CRC32 directory hashing. `CSgaCreator` builds archives from `IDirectoryTraverser` sources.

#### Format Parser Subsystem
**Files**: `CRgdFile.h/cpp`, `CRgdFileMacro.h/cpp`, `CChunkyFile.h/cpp`, `CRgtFile.h/cpp`, `CRgmFile.h/cpp`, `CBfxFile.h/cpp`

Parsers for Relic's proprietary binary formats. Most follow a `Load(IStream*)` / `Save(IOutputStream*)` pattern. `CRgdFile` is the most complex — it includes nested `CMetaNode`/`CMetaTable` inner classes and bidirectional conversion with `CLuaFile`.

#### Localization Subsystem
**Files**: `CUcsFile.h/cpp`, `CCohUcsFile.h/cpp`, `CUcsTransaction.h`

UCS (Unicode Character Set) files store localized strings keyed by numeric ID. `CUcsFile` handles DoW format (UTF-16LE), `CCohUcsFile` handles CoH's variant. Dollar-string resolution (`$12345`) maps IDs to display text.

#### Lua Integration Subsystem
**Files**: `CLuaFile.h/cpp`, `CLuaFile2.h/cpp`, `CLuaScript.h/cpp`, `CLuaScript_Interface.h/cpp`, `CLuaFromRgd.h/cpp`, `CLuaFileCache.h/cpp`, `CInheritTable.h/cpp`, `luax.h/cpp`, `luax51.cpp`, `Lua51.h`

The most complex subsystem. Two vendored Lua runtimes coexist:
- **Lua 5.0.2** (`lua502/`): Native symbol names. Used by `CLuaFile`, `CLuaFile2`, `CLuaScript`, `CRgdFile::CreateLuaState()`.
- **Lua 5.1.2** (`lua512src/`): All symbols renamed via `lua512_rename.h` (`lua_` → `lua51_`). Exposed through `Lua51.h`. Used by `CRgdFileMacro`.

`CLuaFile` is the primary Lua data file parser implementing `IMetaTable`. `CInheritTable` resolves Relic's three-way inheritance system (`Inherit()`, `Reference()`, `GameData()`). `CLuaFromRgd` converts RGD binary data back to Lua source.

#### Module Subsystem
**Files**: `CModuleFile.h/cpp`, `CDoWModule.h/cpp`, `CDoWFileView.h/cpp`, `CFileMap.h/cpp`

The highest-level subsystem. `CModuleFile` is the god class — it parses `.module` files and aggregates all data sources (filesystem folders, SGA archives, UCS files, required mods) into a single `IFileStore` + `IDirectoryTraverser` facade. `CDoWModule` scans game installations to discover available mods.

**CModuleFile is the central coupling point**: it directly depends on `CSgaFile`, `CUcsFile`, `CDoWFileView`, `CFileMap`, `CFileSystemStore`, and `Callbacks`.

#### Utilities
**Files**: `Exception.h/cpp`, `RainmanLog.h/cpp`, `Callbacks.h/cpp`, `CRgdHashTable.h/cpp`, `Util.h/cpp`, `Internal_Util.h/cpp`, `WriteTime.h/cpp`, `crc32_case_idt.h/c`, `hash.c`, `md5.h/c`, `Api.h`, `gnuc_defines.h/cpp`, `memdebug.h`

Cross-cutting concerns. `CRainmanException` provides precursor-chained exceptions. `RainmanLog` wraps spdlog. `CRgdHashTable` maps human-readable property names to Jenkins hashes. `Api.h` defines the `RAINMAN_API` export macro.

### 3.3 Class Inheritance Hierarchy

```
CRainmanException                    (base exception, heap-allocated)
└── CModStudioException              (CDMS-specific, in src/cdms/)

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
├── ConstructFrame                   (main window)
│   └── ITool                        (tool plugin interface)
│       ├── CLocaleTool, CAESetupTool, CUcsTool, CAttribSnapshotTool
│       ├── CSgaPackerTool, CExtractAllTool, CDpsCalculatorTool
│       ├── CRedButtonTool, CMakeLuaInheritTree, CRefreshFilesTool
├── frmLoading                       (progress/splash)
└── frmMessage                       (message display)

wxWindow
├── frmFiles                         (file tree browser)
│   ├── IHandler                     (file action interface)
│   │   ├── CLuaAction, CTextViewAction, CExtractAction, ...
│   │   └── (30+ action handler subclasses in frmFiles_Actions.h)
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
IGNORE_EXCEPTIONS                    // catch CRainmanException* and destroy()

// Cleanup (NEVER use delete):
pException->destroy();               // walks precursor chain, frees all
```

Exceptions form a linked list via `m_pPrecursor`, enabling stack-trace-like error chains. The `destroy()` method recursively frees the entire chain.

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

The main frame is the **god class** of the GUI layer. It:
- Owns the `CModuleFile*` (the loaded mod — central data object)
- Owns the `CRgdHashTable*` (property name ↔ hash mapping)
- Owns the `wxAuiNotebook` tab container
- Owns the `frmFiles` file browser panel
- Owns the `frmLoading` splash/progress frame
- Contains all menu event handlers (File→Open, Mod→Save, etc.)
- Registers and dispatches 10+ ITool plugins
- Has 20 duplicate `LaunchModToolN()` handlers

Key members exposed globally via `TheConstruct` macro:
```cpp
CModuleFile* GetModule();
CRgdHashTable* GetRGDHashTable();
wxAuiNotebook* GetTabs();
frmFiles* GetFileTree();
```

### 4.3 File Browser (frmFiles + frmFiles_Actions.h)

`frmFiles` renders the mod's directory tree using `wxTreeCtrl`, traversing `IDirectoryTraverser::IIterator`. On file selection, it dispatches to one of 30+ action handlers via the `IHandler` interface:

```cpp
class IHandler {
    virtual wxString VGetExt() const = 0;     // file extension to match
    virtual wxString VGetAction() const = 0;  // display name
    virtual void VHandle(wxString sFile, wxTreeItemId& oId, wxTreeItemId& oFileRoot) = 0;
};
```

All handler implementations live in `frmFiles_Actions.h` (2,201 lines). Each handler directly calls Rainman APIs — e.g., `TheConstruct->GetModule()->VOpenStream(saFile)` to open files.

### 4.4 CDMS → Rainman Coupling Map

Every CDMS file includes `Common.h`, which transitively includes `<Rainman.h>` (the god header pulling in 15 Rainman headers). Below are the **direct Rainman type usages** per CDMS file:

| CDMS File | Rainman Types Directly Used |
|-----------|---------------------------|
| `Construct.cpp` | `CModuleFile`, `CRgdHashTable` — creates/manages the loaded mod and hash table |
| `frmFiles.cpp` | `IDirectoryTraverser::IIterator` — builds file tree |
| `frmFiles_Actions.h` | `IFileStore::IStream`, `IOutputStream`, `CRgdFile`, `CLuaFile`, `CLuaFile2`, `CRgtFile`, `CChunkyFile`, `CSgaCreator`, `CInheritTable`, `CRgdHashTable`, `CModuleFile` — the most coupled file |
| `frmRgdEditor.cpp` | `IMetaNode`, `IMetaTable`, `CRgdFile`, `CUcsFile` — tree editor for RGD data |
| `frmScarEditor.cpp` | `CModuleFile` — checks module type for script behavior |
| `frmModule.cpp` | `CModuleFile`, `CFolderHandler`, `CArchiveHandler`, `CRequiredHandler`, `CCompatibleHandler` — displays module metadata |
| `Tools.cpp` | `CModuleFile`, `CUcsFile`, `CUcsHandler` — tool dispatch and UCS management |
| `Tool_AESetup.cpp` | `CModuleFile`, `CUcsFile` — UCS→DAT conversion |
| `Tool_AutoDPS.cpp` | `CRgdHashTable`, `CRgdFile`, `IMetaNode` — DPS calculation from weapon data |
| `frmUCSEditor.cpp` | `CUcsFile` — localization string editing |
| `frmSgaMake.cpp` | `CSgaCreator`, `CModuleFile` — archive creation wizard |

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
char* wxStringToAscii(const wxString& sString);  // caller must delete[]
```

**Issues**: No UTF-8 support; manual `new[]`/`delete[]` for conversions; potential memory leaks at conversion boundaries.

### 4.7 Tool Plugin System

```cpp
class ConstructFrame::ITool {
    virtual const wxString& GetName() = 0;
    virtual const wxString& GetHelpString() = 0;
    virtual const wxString& GetBitmapName() = 0;
    virtual bool DoAction(ConstructFrame* pFrame) = 0;
};
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

Tools are dispatched via 20 hard-coded `LaunchModToolN()` menu handlers that call `DoTool(sName)`.

---

## 5. Build System & Dependencies

### 5.1 CMake Structure

```
CMakeLists.txt (top-level)
├── src/rainman/CMakeLists.txt
│   ├── lua502 (STATIC library, 28 .c files)
│   ├── lua512 (STATIC library, 28 .c files, /FI lua512_rename.h)
│   └── rainman (STATIC library, GLOB *.cpp *.c, PCH pch_rainman.h)
├── src/cdms/CMakeLists.txt
│   └── CorsixModStudio (WIN32 EXE, GLOB *.cpp *.c + resource.rc)
├── tests/rainman/CMakeLists.txt
│   └── rainman_tests (EXE, explicit file list, gtest_discover_tests)
└── tests/cdms/CMakeLists.txt
    └── cdms_tests (EXE, explicit file list + 4 CDMS sources compiled in)
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

| Test File | Tested Class | Depth | Pattern |
|-----------|-------------|-------|---------|
| `exception_test.cpp` | `CRainmanException` | Good | Chaining, macros, formatting |
| `memorystore_test.cpp` | `CMemoryStore` | Good | Read/write streams, seeks |
| `sgafile_test.cpp` | `CSgaFile` | Partial | Error handling only (no valid archives) |
| `rgdfile_test.cpp` | `CRgdFile` | Good | Create, save/load round-trips, nested tables |
| `ucsfile_test.cpp` | `CUcsFile` | Good | Strings, load/resolve, dollar IDs |
| `chunkyfile_test.cpp` | `CChunkyFile` | Good | Nested chunks, save/load, metadata |
| `modulefile_test.cpp` | `CModuleFile` | Good | Config parsing, versions, archives |
| `rgdhashtable_test.cpp` | `CRgdHashTable` | Good | Hash mapping, numeric keys |
| `filesystemstore_test.cpp` | `CFileSystemStore` | Partial | Read/write operations |
| `inherittable_test.cpp` | `CInheritTable` | Good | Parentage, orphan assignment |
| `callbacks_test.cpp` | `Callbacks` | Good | Printf-style formatting |
| `crc32_case_idt_test.cpp` | `crc32_case_idt` | Good | Hash validation |
| `writetime_test.cpp` | `WriteTime` | Good | Timestamp handling |
| `internal_util_test.cpp` | `Internal_Util` | Good | String operations |
| `md5_test.cpp` | `md5` | Good | RFC 1321 compliance |
| `directorytraverser_test.cpp` | `IDirectoryTraverser` | Partial | Directory operations |
| `config_test.cpp` (CDMS) | `ConfGetColour` | Good | Color retrieval with defaults |
| `strings_test.cpp` (CDMS) | UI strings | Partial | String constant checks |
| `strconv_test.cpp` (CDMS) | `strconv` | Partial | String conversion |
| `utility_test.cpp` (CDMS) | `Utility` | Partial | Helper functions |

### 6.3 Coverage Gaps

**Untested Rainman classes** (ordered by estimated risk):

| Class | Risk | Reason |
|-------|------|--------|
| `CLuaFile` / `CLuaFile2` | **Critical** | Core data format, complex Lua state management |
| `CLuaScript` / `CLuaScript_Interface` | **Critical** | Scripting engine, Lua 5.0.2 integration |
| `CLuaFromRgd` | **High** | RGD↔Lua conversion, depends on both |
| `CSgaCreator` | **High** | Data integrity critical (creates archives) |
| `CDoWModule` | **High** | Game installation scanning, 1,969 lines |
| `CRgtFile` | **Medium** | Texture format (DXT compression) |
| `CRgmFile` | **Medium** | Material format |
| `CBfxFile` | **Medium** | Binary effects |
| `CCohUcsFile` | **Low** | CoH localization variant |
| `CDoWFileView` | **Low** | Virtual file view |
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

### 7.1 Rainman Internal Dependencies

```
Foundation (no project deps):
  gnuc_defines.h
  WriteTime.h → gnuc_defines.h
  Api.h → gnuc_defines.h
  RainmanLog.h → Api.h
  Lua51.h
  crc32_case_idt.h
  resource.h, resource1.h, memdebug.h

Core interfaces:
  IFileStore.h → gnuc_defines.h, Api.h
  Exception.h → gnuc_defines.h, Api.h
  IDirectoryTraverser.h → gnuc_defines.h, IFileStore.h, WriteTime.h, Api.h
  IMetaTable.h → gnuc_defines.h, Api.h, CMemoryStore.h

Stores:
  CMemoryStore.h → gnuc_defines.h, IFileStore.h
  CFileSystemStore.h → gnuc_defines.h, IFileStore.h, IDirectoryTraverser.h, Api.h
  CSgaFile.h → gnuc_defines.h, IFileStore.h, IDirectoryTraverser.h, CMemoryStore.h, Api.h

Utilities:
  Callbacks.h → gnuc_defines.h, Api.h
  CRgdHashTable.h → gnuc_defines.h, Api.h
  CLuaFileCache.h → gnuc_defines.h, Api.h

Format parsers:
  CChunkyFile.h → gnuc_defines.h, CMemoryStore.h, Exception.h
  CUcsFile.h → gnuc_defines.h, IFileStore.h, Api.h
  CCohUcsFile.h → IFileStore.h
  CUcsTransaction.h → gnuc_defines.h, IFileStore.h, Api.h, CUcsFile.h
  CRgdFile.h → gnuc_defines.h, IFileStore.h, IMetaTable.h, CRgdHashTable.h, CLuaFile.h, Api.h
  CRgtFile.h → gnuc_defines.h, CChunkyFile.h
  CRgmFile.h → gnuc_defines.h, CChunkyFile.h
  CBfxFile.h → CRgdFile.h

Lua:
  CLuaFile.h → gnuc_defines.h, IMetaTable.h, IFileStore.h, CLuaFileCache.h
  CLuaFile2.h → gnuc_defines.h, IMetaTable.h, IFileStore.h, CLuaFileCache.h
  CLuaScript.h → gnuc_defines.h, CLuaScript_Interface.h, Api.h
  CLuaScript_Interface.h → CDoWModule.h

High-level (most includes):
  CDoWFileView.h → gnuc_defines.h, IDirectoryTraverser.h, Api.h
  CFileMap.h → gnuc_defines.h, IDirectoryTraverser.h, Api.h, CSgaFile.h, IFileStore.h
  CDoWModule.h → gnuc_defines.h, CFileSystemStore.h, CDoWFileView.h, CSgaFile.h,
                  CUcsFile.h, Callbacks.h, Api.h
  CModuleFile.h → gnuc_defines.h, CSgaFile.h, CUcsFile.h, CDoWFileView.h, CFileMap.h,
                   CFileSystemStore.h, Api.h, Callbacks.h  ← GOD HEADER (8 includes)
  CRgdFileMacro.h → Lua51.h, CRgdFile.h, CModuleFile.h
  CLuaFromRgd.h → gnuc_defines.h, IFileStore.h, IMetaTable.h, CRgdHashTable.h,
                   CLuaFile.h, CRgdFile.h, CModuleFile.h, Api.h
  Util.h → gnuc_defines.h, CRgdHashTable.h, IDirectoryTraverser.h, Api.h
  Internal_Util.h → gnuc_defines.h, IDirectoryTraverser.h

God header:
  Rainman.h → Util.h, CDoWModule.h, CModuleFile.h, CSgaCreator.h, CBfxFile.h,
              CLuaFile.h, CLuaFile2.h, CLuaFromRgd.h, Exception.h, CChunkyFile.h,
              CRgtFile.h, CRgdFileMacro.h, CRgmFile.h, CUcsTransaction.h,
              CInheritTable.h  ← 15 INCLUDES (pulls in everything)
```

### 7.2 CDMS Dependencies

```
Foundation:
  config.h, strconv.h, strings.h, Application.h → (no project deps)

Utilities:
  Utility.h → strconv.h, strings.h

Mid-level:
  frmLoading.h, frmLuaInheritTree.h → (no project deps)
  frmFiles.h → frmLuaInheritTree.h
  Construct.h → frmLoading.h, frmFiles.h
  frmModule.h → strings.h
  frmScarEditor.h → strings.h
  frmLocaleSelector.h → Construct.h
  Tools.h → Construct.h, frmUCSSelector.h

God header:
  Common.h → Construct.h, Utility.h, strconv.h, strings.h, <Rainman.h>
  ↓
  Every .cpp file includes Common.h → gets ALL of Rainman transitively
```

### 7.3 Key Observations

1. **`Rainman.h` is a transitive god header**: It pulls in 15 Rainman headers, which transitively pull in nearly everything else. Any change to any Rainman header likely triggers a full CDMS rebuild.

2. **`Common.h` bridges the coupling**: By including both `Construct.h` and `<Rainman.h>`, it ensures every CDMS source file depends on every Rainman header.

3. **`CModuleFile.h` is the internal god header**: It includes 8 other Rainman headers, making it the heaviest single Rainman header.

4. **No circular includes detected**: The dependency graph is strictly acyclic. However, `CRgdFile` ↔ `CLuaFile` have a **logical** circular dependency (bidirectional conversion) even though the include graph is one-way (`CRgdFile.h` includes `CLuaFile.h`, not vice versa).

5. **Forward declarations are underused**: Most headers use full includes where forward declarations would suffice.

---

## 8. Technical Debt Inventory

### 8.1 Structural Issues

| ID | Issue | Severity | Location | Impact |
|----|-------|----------|----------|--------|
| S1 | **God header `Rainman.h`** pulls 15 headers | High | `include/rainman/Rainman.h` | Any Rainman change rebuilds all CDMS |
| S2 | **God header `Common.h`** pulls Rainman.h + Construct.h | High | `src/cdms/Common.h` | Every CDMS file depends on everything |
| S3 | **God class `CModuleFile`** (1,608 LOC, 60+ methods, 8 header deps) | High | `CModuleFile.h/cpp` | Central coupling point |
| S4 | **God class `ConstructFrame`** (983 LOC, owns module + tabs + tools + hash table) | High | `Construct.h/cpp` | All GUI state in one class |
| S5 | **Monster file `frmFiles_Actions.h`** (2,201 LOC, 30+ inline classes) | High | `frmFiles_Actions.h` | Unmaintainable inline handlers |
| S6 | **No GUI↔Data abstraction layer** | High | All CDMS files | GUI directly manipulates Rainman types |
| S7 | **Forward declarations underused** | Medium | Most .h files | Unnecessary compile-time coupling |
| S8 | **Flat directory structure** | Medium | `src/rainman/` | 80 files in one directory, no subsystem grouping |

### 8.2 Memory & Safety Issues

| ID | Issue | Severity | Location | Impact |
|----|-------|----------|----------|--------|
| M1 | **Manual stream ownership** (raw `new`/`delete`) | High | All IFileStore users | Leak risk on exception paths |
| M2 | **Heap-allocated exceptions** with manual `destroy()` | High | Exception.h, ~50 throw sites | Leak risk if destroy() forgotten |
| M3 | **Raw `new[]`/`delete[]` string conversions** | Medium | `strconv.h/cpp` | Leak risk at conversion boundaries |
| M4 | **No RAII** for any resource type | High | Pervasive | Systematic leak risk |
| M5 | **`unsigned long` instead of `size_t`** in APIs | Low | All interfaces | 32-bit limitation on x64 (but safe for game data) |

### 8.3 Code Quality Issues

| ID | Issue | Severity | Location | Impact |
|----|-------|----------|----------|--------|
| Q1 | **20 duplicate `LaunchModToolN()` handlers** | Low | `Construct.h/cpp` | Violation of DRY |
| Q2 | **No UTF-8 support** in string conversions | Medium | `strconv.h/cpp` | Incorrect for non-ASCII game data |
| Q3 | **Bidirectional RGD↔Lua dependency** | Medium | `CRgdFile` ↔ `CLuaFile` | Logical circular dependency |
| Q4 | **Windows-only APIs** (`LoadLibraryW`, `_wfopen`, etc.) | Low | Multiple files | Not portable (acceptable for now) |
| Q5 | **~55% of classes untested** (esp. Lua subsystem) | High | `tests/` | Refactoring without safety net |

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

Most classes require `VInit()` before use. Some accept init data via `void* pInitData`. There is no RAII — callers must manage the lifecycle manually.

### 9.3 File Format Load/Save

```cpp
// Typical parser lifecycle:
CRgdFile rgd;
IFileStore::IStream* pStream = store.VOpenStream("data.rgd");
rgd.Load(pStream);      // parser does NOT take ownership
delete pStream;          // caller cleans up stream

// Save:
IFileStore::IOutputStream* pOut = store.VOpenOutputStream("data.rgd", true);
rgd.Save(pOut);          // parser does NOT take ownership
delete pOut;             // caller cleans up
```

### 9.4 CMemoryStore in Tests

```cpp
CMemoryStore store;
store.VInit();

// Wrap raw data as a readable stream:
char* pRange = store.MemoryRange((void*)data, dataLen);
IFileStore::IStream* pStream = store.VOpenStream(pRange);
// ... read from pStream ...
delete pStream;

// Or use static helpers:
auto* pStream = CMemoryStore::OpenStreamExt(pData, len, false);
auto* pOutStream = CMemoryStore::OpenOutputStreamExt();
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
