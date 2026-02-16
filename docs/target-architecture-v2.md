# CorsixModStudio — Target Architecture v2: Concurrency & MVP

> **Status**: Proposed — awaiting review. Supersedes [target-architecture.md](target-architecture.md) (Phases A–F complete).
> **Audience**: Contributors, AI assistants, and future maintainers.

---

## Table of Contents

1. [Executive Summary](#1-executive-summary)
2. [Goals & Principles](#2-goals--principles)
3. [Current State Assessment](#3-current-state-assessment)
4. [Concurrency Architecture](#4-concurrency-architecture)
5. [Rainman Internal Parallelism](#5-rainman-internal-parallelism)
6. [MVP Architecture for CDMS](#6-mvp-architecture-for-cdms)
7. [CDMS Directory Restructuring](#7-cdms-directory-restructuring)
8. [Migration Roadmap](#8-migration-roadmap)
9. [Constraints & Non-Goals](#9-constraints--non-goals)

---

## 1. Executive Summary

The v1 architecture migration (Phases A–F) delivered a well-organized, tested (338 tests), and maintainable codebase. This v2 architecture addresses two remaining structural problems:

1. **The application is entirely single-threaded.** Heavy Rainman operations (module loading, SGA creation, mass extraction) block the UI thread for 5–30 seconds, using `wxSafeYield()` as a band-aid. Users perceive the app as frozen.

2. **The CDMS GUI layer lacks formal architecture.** View classes (frmRgdEditor, frmFiles, etc.) mix data loading, business logic, and UI presentation in single classes, making them hard to test, maintain, and extend.

**This document proposes:**
- A `TaskRunner` infrastructure using `std::thread`/`std::future`/`std::mutex` to move heavy work off the UI thread
- Internal Rainman parallelism for archive loading and SGA compression
- An MVP (Model-View-Presenter) pattern for CDMS, separating concerns into testable layers
- A new CDMS directory structure grouping files by architectural role
- An incremental migration roadmap (Phases G–L) building on the existing foundation

---

## 2. Goals & Principles

### Goals

1. **Responsive UI** — no operation blocks the UI thread for more than 100ms
2. **Structured concurrency** — background work has clear ownership, cancellation, and error propagation
3. **Separation of concerns** — each CDMS class has a single architectural role (Model, View, or Presenter)
4. **Testable presenters** — business logic can be tested without instantiating wx controls
5. **Incremental migration** — each phase is a standalone unit; build stays green after every commit

### Principles

- **Threading at boundaries, not everywhere** — only the CDMS↔Rainman boundary introduces threads; individual Rainman classes remain single-threaded per-instance
- **Own what you create** — the thread that allocates an object owns it; transfer ownership explicitly
- **Post, don't poll** — background threads post results to the main thread via `CallAfter()`; the main thread never busy-waits
- **Same conventions** — preserve `C`/`I`/`V`/`m_` naming; new async/MVP classes follow the same style

---

## 3. Current State Assessment

### 3.1 UI-Blocking Operations Inventory

Every `wxSafeYield()` call site represents a blocking operation on the UI thread:

| Operation | File | Rainman API | Duration | Severity |
|-----------|------|-------------|----------|----------|
| Module loading | `Construct.cpp:770` | `CModuleFile::LoadModuleFile()` | 5–15 sec | **Critical** |
| Resource reload | `Construct.cpp:780` | `CModuleFile::ReloadResources()` | 10–30 sec | **Critical** |
| SGA-as-mod loading | `Construct.cpp:629` | `ModuleService::LoadSgaAsMod()` | 5–10 sec | **Critical** |
| RGD macro execution | `frmRgdMacro.cpp:465` | `CRgdFileMacro::loadMacro()` + run | 5–30 sec | **Critical** |
| SGA creation | `frmSgaMake.cpp:222` | `CSgaCreator::CreateSga()` | 10–60 sec | **High** |
| DPS calculation | `Tools.cpp:238` | `AutoDPS_Scan()` + `Analyse()` | 5–10 sec | **High** |
| Lua inherit tree | `frmLuaInheritTree.cpp:61` | `CMakeLuaInheritTree::Do()` | 5–10 sec | **High** |
| UCS→DAT conversion | `Tool_AESetup.cpp:116` | `UCSToDATConvertor::doConvertion()` | 2–5 sec | **Medium** |
| Mass extraction | `frmMassExtract.cpp` | `_DoExtract()` loop | 5–60 sec | **High** |
| File tree population | `Construct.cpp:501` | `FillFromIDirectoryTraverser()` | 2–10 sec | **Medium** |

### 3.2 Rainman Thread-Safety Blockers

| Issue | Location | Severity | Strategy |
|-------|----------|----------|----------|
| Shared `lua_State*` in `CLuaFileCache` | `lua/CLuaFileCache.cpp` | **Blocking** | Thread-local Lua states or mutex-guarded pool |
| `CLuaFileCache` linked list (no locking) | `lua/CLuaFileCache.h` | **Blocking** | Mutex on `Fetch()`/`AddToCache()`/`FreeState()` |
| `CModuleFile` vectors modified during `ReloadResources()` | `module/CModuleFile.cpp` | **Blocking** | Build on background thread, transfer ownership to main |
| `CFileMap` containers unsynchronized | `module/CFileMap.cpp` | **Blocking** | Build on background thread, transfer ownership to main |
| `FILE*` handles in `CFileSystemStore` | `io/CFileSystemStore.cpp` | **Blocking** | Per-instance streams, never share across threads |
| `pfnStatusCallback` calls into UI from Rainman | `core/Callbacks.h` | **Caution** | Replace with thread-safe progress channel |

### 3.3 Current CDMS Structure

All 40+ CDMS files live in a flat directory with `frm*` classes mixing Model, View, and Presenter concerns. Each editor class directly holds Rainman objects, manages wx controls, and contains business logic in one class:

```
src/cdms/           (flat — ~40 files)
├── actions/         (32 file action handlers — Phase E extraction)
├── services/        (4 service wrappers — Phase D extraction)
├── frm*.h/cpp       (12 editor/dialog classes — monolithic M+V+P)
├── Construct.*      (main frame — delegates to helpers)
├── ModuleManager.*  (module lifecycle — Phase F extraction)
├── TabManager.*     (tab management — Phase F extraction)
├── MenuController.* (menu building — Phase E extraction)
├── ToolRegistry.*   (tool dispatch — Phase E extraction)
└── Tools.*          (10 tool implementations)
```

---

## 4. Concurrency Architecture

### 4.1 Design Overview

```
┌─────────────────────────────────────────────────────────────┐
│                    CDMS GUI (Main Thread)                    │
│                                                             │
│  Presenters ──→ TaskRunner::Run(task, onProgress, onDone)   │
│       ↑                    │                                │
│       │           ┌────────┴────────┐                       │
│       │           │ CallAfter()     │ (thread-safe)         │
│       │           │ posts to main   │                       │
│       │           │ event loop      │                       │
│       │           └────────┬────────┘                       │
│       │                    ↑                                │
│  onDone(Result<T>)    onProgress(msg)                       │
├─────────────────────────────────────────────────────────────┤
│                    Background Thread(s)                      │
│                                                             │
│  TaskRunner worker thread:                                  │
│    1. Execute Rainman operation                             │
│    2. Call progress channel (thread-safe → CallAfter)       │
│    3. Catch exceptions → wrap in Result<T>                  │
│    4. Post result → CallAfter(onDone)                       │
│                                                             │
│  ┌──────────────────────────────────────────────────────┐   │
│  │              Rainman Library (per-instance)           │   │
│  │  Each background task creates its own Rainman         │   │
│  │  objects. No sharing across threads.                   │   │
│  └──────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────┘
```

### 4.2 TaskRunner Class Design

The `TaskRunner` is a thin wrapper over `std::thread` that provides:
- Background task execution with result delivery to the main thread
- Thread-safe progress reporting via a progress channel
- Cancellation support via an atomic flag
- Exception marshaling across the thread boundary

```cpp
// async/TaskRunner.h — Pure C++ (no wx dependency in header)

#include <future>
#include <functional>
#include <atomic>
#include <string>

// Progress channel: thread-safe way to report progress from background
class ProgressChannel {
public:
    using ProgressCallback = std::function<void(const std::string&)>;

    explicit ProgressChannel(ProgressCallback onProgress);

    // Called from background thread — posts to main thread
    void Report(const std::string& message);

    // Rainman-compatible: usable as pfnStatusCallback
    static void __cdecl RainmanCallback(const char* sMsg, void* pTag);

private:
    ProgressCallback m_fnOnProgress;
};

// Cancellation token: shared between caller and background task
class CancellationToken {
public:
    void Cancel() { m_bCancelled.store(true); }
    bool IsCancelled() const { return m_bCancelled.load(); }
private:
    std::atomic<bool> m_bCancelled{false};
};

// TaskRunner: launches background work and delivers results to main thread
class TaskRunner {
public:
    // Run a task on a background thread.
    // - task: the work function (runs on background thread)
    // - onProgress: called on MAIN thread with progress messages
    // - onDone: called on MAIN thread with the result
    template<typename T>
    void Run(
        std::function<T(ProgressChannel&, CancellationToken&)> task,
        std::function<void(const std::string&)> onProgress,
        std::function<void(Result<T>)> onDone
    );

    // Cancel the currently running task (if any)
    void Cancel();

    // Is a task currently running?
    bool IsRunning() const;

private:
    std::shared_ptr<CancellationToken> m_pToken;
    std::thread m_thread;
    std::atomic<bool> m_bRunning{false};
};
```

### 4.3 wx Integration: CallAfter Bridge

The `TaskRunner` uses `wxEvtHandler::CallAfter()` to marshal callbacks from the background thread to the main thread. This is the **only** point where wx and threading interact:

```cpp
// async/WxTaskRunner.h — wx-aware wrapper

#include "TaskRunner.h"
#include <wx/event.h>

class WxTaskRunner : public TaskRunner {
public:
    explicit WxTaskRunner(wxEvtHandler* pHandler);

    // Wraps Run() to use CallAfter for onProgress and onDone
    template<typename T>
    void RunAsync(
        std::function<T(ProgressChannel&, CancellationToken&)> task,
        std::function<void(const std::string&)> onProgress,
        std::function<void(Result<T>)> onDone
    );

private:
    wxEvtHandler* m_pHandler;  // Target for CallAfter
};
```

### 4.4 Usage Example: Module Loading

Before (current — blocks UI):
```cpp
void ConstructFrame::DoLoadMod(...) {
    m_pLoadingForm = new frmLoading(AppStr(mod_loading));
    m_pLoadingForm->Show(true);
    wxSafeYield(m_pLoadingForm);

    pMod->LoadModuleFile(sFile, LoadModCallback);          // BLOCKS 5-15 sec
    pMod->ReloadResources(RR_All, ..., LoadModCallback);   // BLOCKS 10-30 sec

    SetModule(pMod, sPath);   // GUI update
    m_pLoadingForm->Close();
}
```

After (proposed — UI stays responsive):
```cpp
void ModuleLoadPresenter::StartLoad(const wxString& sPath) {
    m_pView->ShowLoadingDialog();

    m_taskRunner.RunAsync<CModuleFile*>(
        // Background thread:
        [sPath](ProgressChannel& progress, CancellationToken& cancel) {
            auto sFile = wxStringToAscii(sPath);
            auto* pMod = new CModuleFile;
            pMod->LoadModuleFile(sFile.get(),
                ProgressChannel::RainmanCallback, &progress);

            if (cancel.IsCancelled()) { delete pMod; return nullptr; }

            pMod->ReloadResources(RR_All, RR_All, RR_All,
                ProgressChannel::RainmanCallback, &progress);
            return pMod;
        },
        // Progress (main thread):
        [this](const std::string& msg) {
            m_pView->UpdateProgress(msg);
        },
        // Done (main thread):
        [this, sPath](Result<CModuleFile*> result) {
            m_pView->HideLoadingDialog();
            if (!result.ok()) {
                m_pView->ShowError(result.error());
                return;
            }
            m_pModel->SetModule(result.value(), sPath);
            m_pView->OnModuleLoaded();
        }
    );
}
```

### 4.5 Progress Channel → pfnStatusCallback Bridge

Rainman's existing `pfnStatusCallback` pattern is bridged to the `ProgressChannel` via a static callback:

```cpp
void ProgressChannel::RainmanCallback(const char* sMsg, void* pTag) {
    auto* pChannel = static_cast<ProgressChannel*>(pTag);
    pChannel->Report(sMsg);  // Posts to main thread via CallAfter
}
```

This means **zero changes to Rainman APIs** — the existing callback signatures work unchanged. The `ProgressChannel` just redirects the callback from the background thread to the main thread.

### 4.6 Exception Handling Across Threads

Rainman's heap-allocated exceptions (`throw new CRainmanException`) cannot propagate across thread boundaries via normal C++ exception mechanics. The `TaskRunner` catches them on the background thread and wraps them in `Result<T>`:

```cpp
// Inside TaskRunner::Run():
try {
    T value = task(progress, cancel);
    CallAfter([onDone, value]() { onDone(Result<T>::Ok(value)); });
} catch (CRainmanException* pE) {
    auto result = ResultFromExceptionT<T>(pE);  // Calls destroy()
    CallAfter([onDone, result]() { onDone(result); });
} catch (const std::exception& e) {
    auto result = Result<T>::Err(wxString::FromUTF8(e.what()));
    CallAfter([onDone, result]() { onDone(result); });
}
```

### 4.7 Concurrency Rules

1. **One task per `TaskRunner` instance** — prevents overlapping operations on the same data
2. **Background thread owns what it creates** — the `CModuleFile*` created in the background is transferred to the main thread via `Result<T>`
3. **No shared Rainman objects between threads** — each background task creates its own Rainman instances
4. **Cancel before new task** — if a task is running, cancel it before starting a new one
5. **Lua states are never shared** — each background task that needs Lua creates its own `lua_State*`

---

## 5. Rainman Internal Parallelism

### 5.1 Parallel Archive Loading

The highest-impact parallelism opportunity is in `CResourceLoader::Load()`, where SGA archives are loaded sequentially in a loop. Archives are independent — loading archive A doesn't depend on archive B.

```
Current (sequential):
  for each archive in module.archives:
      CSgaFile* pSga = new CSgaFile()
      pSga->Load(archivePath)           // 10-50ms per archive
      fileMap->RegisterSource(pSga)     // Fast, needs mutex

Proposed (parallel):
  std::vector<std::future<CSgaFile*>> futures;
  for each archive in module.archives:
      futures.push_back(std::async(std::launch::async, [&]() {
          CSgaFile* pSga = new CSgaFile();
          pSga->Load(archivePath);
          return pSga;
      }));

  for auto& f : futures:
      CSgaFile* pSga = f.get();
      fileMap->RegisterSource(pSga);    // Sequential registration
```

**Requirements:**
- `CSgaFile::Load()` must be safe for concurrent calls on **different instances** (it already is — each instance owns its own FILE*)
- `CFileMap::RegisterSource()` needs a `std::mutex` guard (currently unsynchronized)
- Progress reporting needs aggregation across threads

**Expected speedup:** 3–5x on mods with 5+ SGA archives (I/O bound; limited by disk throughput on HDDs, significant on SSDs).

### 5.2 Parallel SGA Compression

`CSgaCreator::CreateSga()` compresses files one-by-one using zlib. Per-file compression is CPU-bound and independent:

```
Current (sequential):
  for each file:
      read file into buffer
      compress2(outBuf, &outLen, inBuf, inLen, level)  // CPU-bound
      write compressed data to output

Proposed (parallel):
  // Phase 1: Parallel compression (thread pool)
  for each file:
      pool.submit([&]() {
          read file into buffer
          compress2(outBuf, &outLen, inBuf, inLen, level)
          return CompressedFile{data, len, crc, metadata}
      });

  // Phase 2: Sequential write (single thread)
  for each compressed result:
      write to output file
```

**Requirements:**
- Separate read buffers per thread (already heap-allocated per file)
- Output file writing must remain sequential (SGA format requires ordered entries)
- Memory management: limit concurrent in-flight buffers to avoid OOM

**Expected speedup:** ~Nx on N-core machines (compression is CPU-bound; zlib is thread-safe for independent calls).

### 5.3 Thread Safety Retrofits Required

Before enabling any internal parallelism, these minimal thread-safety changes are needed:

| Class | Change | Scope |
|-------|--------|-------|
| `CFileMap` | Add `std::mutex` around `RegisterSource()` | 1 method |
| `CLuaFileCache` | Option A: Add `std::mutex` around all public methods. Option B: Make thread-local (preferred) | 3 methods |
| `CResourceLoader` | Use `std::async` for archive loading loop | 1 method |
| `CSgaCreator` | Use thread pool for compression loop | 1 method |

### 5.4 What NOT to Parallelize

| Component | Reason |
|-----------|--------|
| `CModuleParser` (INI parsing) | < 5ms — not worth the complexity |
| `CFileMap` tree building | Heavy shared-state mutations; would need complete redesign |
| Individual `CSgaFile` reads | Per-instance FILE* is inherently sequential |
| `CLuaFile` loading | Lua state is not thread-safe; need separate states per thread |
| `CRgdFile` parsing | Small files (~1ms each); parallelism overhead would dominate |

---

## 6. MVP Architecture for CDMS

### 6.1 Pattern Definition

The MVP (Model-View-Presenter) pattern separates each CDMS editor/panel into three layers:

```
┌──────────────────────────────────────────────────────────┐
│                      Presenter                            │
│  Coordinates Model ↔ View. Contains business logic.      │
│  Owns TaskRunner for async operations.                    │
│  No wxWidgets dependencies (testable with mock views).    │
├──────────────────────────────────────────────────────────┤
│          │                              │                 │
│    ┌─────▼─────┐                ┌───────▼───────┐        │
│    │   Model    │                │  View (I*)    │        │
│    │            │                │               │        │
│    │ Rainman    │                │ wxWidgets     │        │
│    │ data via   │                │ controls.     │        │
│    │ services.  │                │ Implements    │        │
│    │ No wx.     │                │ IView.        │        │
│    └────────────┘                └───────────────┘        │
└──────────────────────────────────────────────────────────┘
```

**Model** — Wraps Rainman data. No wxWidgets dependencies. Accessed via the existing service layer (`ModuleService`, `FileService`, `FormatService`, `HashService`). Holds data state and provides domain operations.

**View** — Defines an abstract interface (`IFooView`) that the presenter calls. The concrete implementation (`frmFoo`) inherits from both `wxWindow` and `IFooView`. View interfaces use `wxString`, `std::vector`, and simple types — no Rainman types.

**Presenter** — Contains all business logic. Holds a reference to the model and the view interface. Owns a `WxTaskRunner` for async operations. **Has no wxWidgets compile dependency** — it uses the view interface and standard C++ types only.

### 6.2 View Interface Design

Each editor gets a view interface that abstracts away wxWidgets:

```cpp
// views/IFileTreeView.h
class IFileTreeView {
public:
    virtual ~IFileTreeView() = default;

    // Tree operations
    virtual void ClearTree() = 0;
    virtual TreeNodeId AddNode(TreeNodeId parent, const wxString& name,
                               int iconIndex) = 0;
    virtual void ExpandNode(TreeNodeId node) = 0;

    // Progress
    virtual void ShowBusyIndicator(const wxString& message) = 0;
    virtual void HideBusyIndicator() = 0;

    // Notifications from presenter
    virtual void OnModuleLoaded(const wxString& moduleName) = 0;
    virtual void OnModuleClosed() = 0;
    virtual void ShowError(const wxString& message) = 0;
};
```

```cpp
// views/IRgdEditorView.h
class IRgdEditorView {
public:
    virtual ~IRgdEditorView() = default;

    // Tree
    virtual TreeNodeId AddTableNode(TreeNodeId parent, const wxString& name) = 0;
    virtual void ClearPropertyGrid() = 0;

    // Properties
    virtual void AddProperty(const wxString& name, const wxString& value,
                              PropertyType type) = 0;
    virtual void SetPropertyValue(const wxString& name, const wxString& value) = 0;

    // Status
    virtual void SetDirtyFlag(bool dirty) = 0;
    virtual void ShowError(const wxString& message) = 0;
};
```

### 6.3 Presenter Design

Presenters coordinate between model and view, own async operations, and contain all business logic:

```cpp
// presenters/ModuleLoadPresenter.h
class ModuleLoadPresenter {
public:
    ModuleLoadPresenter(ModuleModel& model, IMainFrameView& view);

    void LoadMod(const wxString& path, ConstructFrame::eLoadModGames game);
    void LoadSga(const wxString& path);
    void CloseMod();
    void Cancel();

private:
    ModuleModel& m_model;
    IMainFrameView& m_view;
    WxTaskRunner m_taskRunner;
};
```

```cpp
// presenters/RgdEditorPresenter.h
class RgdEditorPresenter {
public:
    RgdEditorPresenter(IRgdEditorView& view, FileService& fileService,
                        HashService& hashService);

    void LoadFile(const wxString& path);
    void OnPropertyChanged(const wxString& name, const wxString& value);
    void OnTreeNodeSelected(TreeNodeId node);
    void Save();
    void CopyNode();
    void PasteNode();

private:
    IRgdEditorView& m_view;
    FileService& m_fileService;
    HashService& m_hashService;
    std::unique_ptr<CRgdFile> m_pRgdFile;
    WxTaskRunner m_taskRunner;
};
```

### 6.4 Model Layer

Models wrap Rainman data and provide domain operations. They live below the presenter and above the service layer:

```cpp
// models/ModuleModel.h
class ModuleModel {
public:
    void SetModule(CModuleFile* pMod, const wxString& path);
    void ClearModule();

    bool HasModule() const;
    CModuleFile* GetModule() const;
    const wxString& GetModulePath() const;

    // Service accessors
    ModuleService& GetModuleService();
    FileService& GetFileService();
    HashService& GetHashService();

private:
    ModuleManager m_manager;  // Existing class, reused
};
```

The `ModuleModel` is thin — it delegates to the existing `ModuleManager`. The key difference is that presenters access data through the model, not through `TheConstruct` global.

### 6.5 MVP Mapping for Existing Views

| Current Class | Model | View Interface | Presenter | Async Operations |
|---------------|-------|----------------|-----------|-----------------|
| `ConstructFrame` | `ModuleModel` | `IMainFrameView` | `ModuleLoadPresenter` | Module loading, SGA loading |
| `frmFiles` | File tree data (via `FileService`) | `IFileTreeView` | `FileTreePresenter` | Tree population |
| `frmRGDEditor` | `CRgdFile` | `IRgdEditorView` | `RgdEditorPresenter` | File load/save |
| `frmScarEditor` | Script text + function list | `IScarEditorView` | `ScarEditorPresenter` | Lua syntax check |
| `frmUCSEditor` | `CUcsTransaction` | `IUcsEditorView` | `UcsEditorPresenter` | UCS load/save |
| `frmModule` | Module properties (via `ModuleService`) | `IModuleSettingsView` | `ModuleSettingsPresenter` | FS enumeration |
| `frmImage` | `CRgtFile` | `IImageView` | `ImagePresenter` | Format conversion |
| `frmMassExtract` | Extraction source list | `IMassExtractView` | `MassExtractPresenter` | File extraction |
| `frmSgaMake` | SGA creation params | `ISgaMakeView` | `SgaMakePresenter` | SGA compression |
| `frmRgdMacro` | Macro script + results | `IRgdMacroView` | `RgdMacroPresenter` | Macro execution |

### 6.6 Eliminating TheConstruct Global

Currently, most CDMS code accesses global state via `TheConstruct->GetModule()`, `TheConstruct->GetFileService()`, etc. The MVP pattern eliminates this:

- **Presenters** receive their dependencies via constructor injection (model references, service references)
- **Views** communicate only through their presenter, not through globals
- **`TheConstruct` macro** remains for backward compatibility during migration but new code never uses it

---

## 7. CDMS Directory Restructuring

### 7.1 Target Directory Layout

```
src/cdms/
├── async/                        # Threading infrastructure
│   ├── TaskRunner.h/cpp          # std::thread-based task runner
│   ├── WxTaskRunner.h/cpp        # wx CallAfter bridge
│   ├── ProgressChannel.h/cpp     # Thread-safe progress reporting
│   └── CancellationToken.h       # Atomic cancellation flag
│
├── models/                       # Data models (no wx dependency)
│   ├── ModuleModel.h/cpp         # Module lifecycle (wraps ModuleManager)
│   └── ... (future models)
│
├── views/                        # View interfaces + implementations
│   ├── interfaces/               # Pure abstract interfaces (no wx)
│   │   ├── IMainFrameView.h
│   │   ├── IFileTreeView.h
│   │   ├── IRgdEditorView.h
│   │   ├── IScarEditorView.h
│   │   ├── IUcsEditorView.h
│   │   ├── IModuleSettingsView.h
│   │   ├── IImageView.h
│   │   ├── IMassExtractView.h
│   │   ├── ISgaMakeView.h
│   │   └── IRgdMacroView.h
│   │
│   ├── frmFiles.h/cpp            # IFileTreeView implementation
│   ├── frmRgdEditor.h/cpp        # IRgdEditorView implementation
│   ├── frmScarEditor.h/cpp       # IScarEditorView implementation
│   ├── frmUCSEditor.h/cpp        # IUcsEditorView implementation
│   ├── frmModule.h/cpp           # IModuleSettingsView implementation
│   ├── frmImage.h/cpp            # IImageView implementation
│   ├── frmMassExtract.h/cpp      # IMassExtractView implementation
│   ├── frmSgaMake.h/cpp          # ISgaMakeView implementation
│   ├── frmRgdMacro.h/cpp         # IRgdMacroView implementation
│   ├── frmLoading.h/cpp          # Progress dialog (simple, no MVP)
│   ├── frmWelcome.h/cpp          # Welcome panel (static, no MVP)
│   ├── frmCredits.h/cpp          # Credits dialog (static, no MVP)
│   ├── frmMessage.h/cpp          # Message display (static, no MVP)
│   ├── frmNewMod.h/cpp           # New mod dialog (simple, no MVP)
│   ├── frmFileSelector.h/cpp     # File picker dialog (simple, no MVP)
│   ├── frmLocaleSelector.h/cpp   # Locale picker dialog (simple, no MVP)
│   ├── frmUCSSelector.h/cpp      # UCS picker dialog (simple, no MVP)
│   ├── frmUCSOutRange.h/cpp      # UCS range dialog (simple, no MVP)
│   ├── frmLuaInheritTree.h/cpp   # Lua inherit tree panel
│   └── CtrlStatusText.h/cpp      # Custom status text control
│
├── presenters/                   # Business logic (no wx dependency)
│   ├── ModuleLoadPresenter.h/cpp
│   ├── FileTreePresenter.h/cpp
│   ├── RgdEditorPresenter.h/cpp
│   ├── ScarEditorPresenter.h/cpp
│   ├── UcsEditorPresenter.h/cpp
│   ├── ModuleSettingsPresenter.h/cpp
│   ├── ImagePresenter.h/cpp
│   ├── MassExtractPresenter.h/cpp
│   ├── SgaMakePresenter.h/cpp
│   └── RgdMacroPresenter.h/cpp
│
├── services/                     # Service layer (unchanged from v1)
│   ├── Result.h
│   ├── ModuleService.h/cpp
│   ├── FileService.h/cpp
│   ├── FormatService.h/cpp
│   └── HashService.h/cpp
│
├── actions/                      # File action handlers (unchanged from v1)
│   ├── ActionUtil.h
│   ├── CRGDAction.h
│   ├── CTextViewAction.h
│   └── ... (32 total)
│
├── common/                       # Shared utilities
│   ├── Common.h                  # Backward compat header
│   ├── Application.h/cpp         # wx app entry point
│   ├── config.h/cpp              # Configuration wrappers
│   ├── strconv.h/cpp             # String conversion
│   ├── strings.h/cpp             # UI string constants
│   ├── Utility.h/cpp             # Shared helpers
│   ├── ITool.h                   # Tool plugin interface
│   └── TabDialog.h/cpp           # Tab dialog base
│
├── frame/                        # Main frame infrastructure
│   ├── Construct.h/cpp           # Main frame (delegates to helpers)
│   ├── ModuleManager.h/cpp       # Module lifecycle
│   ├── TabManager.h/cpp          # Tab notebook management
│   ├── MenuController.h/cpp      # Menu bar construction
│   └── ToolRegistry.h/cpp        # Tool dispatch
│
├── tools/                        # ITool implementations
│   ├── Tools.h/cpp               # Tool class definitions
│   ├── Tool_AESetup.h/cpp        # AE setup tool
│   └── Tool_AutoDPS.h/cpp        # DPS calculator
│
├── res/                          # Resources (unchanged)
├── resource.h                    # Resource IDs
├── resource.rc                   # Resource script
└── CMakeLists.txt                # Build configuration
```

### 7.2 CMakeLists.txt Update

```cmake
file(GLOB CDMS_SOURCES
    "common/*.cpp"
    "frame/*.cpp"
    "models/*.cpp"
    "views/*.cpp"
    "presenters/*.cpp"
    "services/*.cpp"
    "actions/*.cpp"
    "async/*.cpp"
    "tools/*.cpp"
)
```

### 7.3 Migration Strategy

Files are moved incrementally — one directory at a time. The GLOB pattern auto-discovers new locations. Steps:

1. Create the new directories
2. Move files one group at a time (common → frame → tools → views → ...)
3. Update `#include` paths in each moved file
4. Build and test after each group
5. Extract view interfaces and create presenters for one editor at a time

---

## 8. Migration Roadmap

Each phase is a standalone unit of work. Complete one before starting the next. Every phase ends with all tests passing.

### Phase G: Async Infrastructure ✅

> **Status**: Complete — 23 tests, `CTaskRunner`, `CWxTaskRunner`, `CProgressChannel`, `CCancellationToken` in `src/cdms/async/`.

Create the `TaskRunner`, `ProgressChannel`, `CancellationToken`, and `WxTaskRunner` classes. No existing code changes — purely additive.

1. Create `src/cdms/async/` directory
2. Implement `CancellationToken` (header-only, atomic bool)
3. Implement `ProgressChannel` with `RainmanCallback` bridge
4. Implement `TaskRunner` using `std::thread` + `std::future`
5. Implement `WxTaskRunner` with `CallAfter()` integration
6. Write unit tests for `TaskRunner` (using mock callbacks)
7. Write integration test proving background work + main-thread delivery

### Phase H: Module Loading Async ✅

> **Status**: Complete — 366 total tests (5 new presenter tests). `DoLoadMod()` and `DoLoadSga()` now use `CModuleLoadPresenter` with `CWxTaskRunner` for async background loading. `frmLoading` enhanced with cancel button. `IMainFrameView` interface and `CModuleLoadPresenter` in `src/cdms/views/` and `src/cdms/presenters/`.
>
> **Key decisions**: Enhanced `frmLoading` (not `wxProgressDialog`); minimal `IMainFrameView` with loading-related methods only; menu items disabled during load; `LoadModCallback` retained for `Tools.cpp` usage; `CModuleFile` leak-safe via `catch (...) { delete pMod; throw; }` pattern.

Move module loading off the UI thread using the `TaskRunner`. This is the highest-impact change.

1. Create `ModuleLoadPresenter` with `LoadMod()` and `LoadSga()` methods
2. Create `IMainFrameView` interface with loading/error/module-loaded callbacks
3. Have `ConstructFrame` implement `IMainFrameView`
4. Refactor `DoLoadMod()` to use `ModuleLoadPresenter::LoadMod()` (async)
5. Refactor `DoLoadSga()` to use `ModuleLoadPresenter::LoadSga()` (async)
6. Remove `wxSafeYield()` from `LoadModCallback` — replaced by `ProgressChannel`
7. Replace `frmLoading` text-only UI with `wxProgressDialog` (or enhanced `frmLoading` with gauge)
8. Test: verify module loads correctly with responsive UI

### Phase I: Background Operations for Tools & Dialogs

Move remaining blocking operations to background threads.

1. `frmMassExtract` — background extraction with progress gauge updates via `WxTaskRunner`
2. `frmSgaMake` — background SGA creation with progress
3. `frmRgdMacro` — background macro execution with cancellation
4. `frmLuaInheritTree` — background tree building
5. `Tools.cpp` DPS calculator — background scan + analysis
6. `Tool_AESetup` UCS→DAT — background conversion
7. File tree population (`FillFromIDirectoryTraverser`) — background with progressive loading

### Phase J: Rainman Internal Parallelism

Add parallelism within Rainman for archive loading and SGA compression.

1. Add `std::mutex` to `CFileMap::RegisterSource()`
2. Parallelize archive loading loop in `CResourceLoader::Load()` using `std::async`
3. Add thread pool to `CSgaCreator::CreateSga()` for parallel per-file compression
4. Make `CLuaFileCache` thread-safe (mutex on public methods) or thread-local
5. Test: verify parallel loading produces identical results to sequential
6. Benchmark: measure speedup on mods with 5+ archives

### Phase K: CDMS Directory Restructuring

Reorganize the CDMS directory structure.

1. Create directories: `async/`, `models/`, `views/`, `views/interfaces/`, `presenters/`, `common/`, `frame/`, `tools/`
2. Move utility files to `common/` (Application, config, strconv, strings, Utility, ITool, TabDialog)
3. Move frame infrastructure to `frame/` (Construct, ModuleManager, TabManager, MenuController, ToolRegistry)
4. Move tool implementations to `tools/` (Tools, Tool_AESetup, Tool_AutoDPS)
5. Move view files to `views/` (all frm* files, CtrlStatusText)
6. Update CMakeLists.txt glob patterns
7. Update all `#include` paths
8. Build and test after each group

### Phase L: MVP Extraction (Per-Editor)

Extract view interfaces and presenters for each editor. Do one editor at a time.

1. **L.1 — RGD Editor MVP**: Extract `IRgdEditorView` interface, create `RgdEditorPresenter`, move business logic out of `frmRgdEditor`
2. **L.2 — File Browser MVP**: Extract `IFileTreeView` interface, create `FileTreePresenter`
3. **L.3 — SCAR Editor MVP**: Extract `IScarEditorView` interface, create `ScarEditorPresenter`
4. **L.4 — UCS Editor MVP**: Extract `IUcsEditorView` interface, create `UcsEditorPresenter`
5. **L.5 — Module Settings MVP**: Extract `IModuleSettingsView`, create `ModuleSettingsPresenter`
6. **L.6 — Image Viewer MVP**: Extract `IImageView`, create `ImagePresenter`
7. **L.7 — Remaining dialogs**: Mass extract, SGA make, RGD macro — extract as needed

Each sub-phase:
- Define the view interface
- Create the presenter with constructor-injected dependencies
- Move business logic from frm* class to presenter
- frm* class implements the view interface
- Add presenter unit tests using mock view
- Build and test

---

## 9. Constraints & Non-Goals

### Hard Constraints

- **`CDMSSrc_055/`** is read-only — never modify
- **Lua 5.0.2 and 5.1.2** must not be upgraded (game data compatibility)
- **Lua states are NOT thread-safe** — never share `lua_State*` across threads
- **Build stays green** — `ctest --preset debug` must pass after every commit
- **LGPL v2.1 headers** preserved on all Rainman files
- **wxWidgets main-thread rule** — all GUI updates happen on the main thread
- **Hungarian notation** — preserve `C`/`I`/`V`/`m_` conventions in existing code
- **Existing Rainman API surface** — function signatures unchanged; thread safety added internally

### Non-Goals

- **Rewriting Rainman in modern C++** — existing API preserved; changes are additive (mutexes, parallel loops)
- **Cross-platform threading** — Windows-only is acceptable (std::thread works on MSVC)
- **Lock-free data structures** — `std::mutex` is sufficient; no need for atomic-based lock-free designs
- **GPU acceleration** — texture decompression stays CPU-based
- **Plugin/extension system** — the ITool interface is sufficient for now
- **Full dependency injection framework** — manual constructor injection is adequate for MVP
