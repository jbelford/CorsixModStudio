# CorsixModStudio — Comprehensive Manual Test Plan

> **Phase 6 Integration Testing** — Final validation of the Corsix 0.5.5 port.
>
> All code migration phases (A–F) are complete with 400+ automated unit tests
> passing. This document provides structured manual validation of all critical
> functionality that cannot be covered by automated tests: GUI interactions,
> real file I/O, end-to-end workflows, and visual correctness.

---

## Prerequisites

| Requirement | Details |
|-------------|---------|
| **Debug build** | `build\bin\Debug\CorsixModStudio.exe` exists |
| **Release build** | `build\bin\Release\CorsixModStudio.exe` exists |
| **Automated tests** | `ctest --preset debug` passes (400+ tests, 0 failures) |
| **Runtime data** | `Mod_Studio_Files\` directory with config.txt, dictionaries, etc. |
| **Game install** *(optional)* | Dawn of War, Dark Crusade, Soulstorm, or Company of Heroes |

## Priority Levels

| Priority | Meaning | Areas |
|----------|---------|-------|
| **P0** | Must pass before any other testing | Build Verification, Application Startup |
| **P1** | Critical functionality — core workflows | Module Loading, SGA Archives, Editors |
| **P2** | Important features — tools and actions | File Conversion, Tools Menu, Context Menus |
| **P3** | Nice to have — config, edge cases, polish | Configuration, Play Menu, New Mod, Edge Cases, Release Build |

## Test Result Template

For each test, record:

```
Test ID: T_._ 
Result:  [ ] PASS  [ ] FAIL  [ ] SKIP  [ ] BLOCKED
Tester:  _______________
Date:    _______________
Notes:   _______________
```

---

## Area 1: Build Verification (P0)

*3 tests — Verify the project builds cleanly and all automated tests pass.*

### T1.1 — Clean Build (Debug)

**Steps:**
1. Delete the `build\` directory entirely.
2. Run `cmake --preset default`.
3. Run `cmake --build --preset debug`.

**Pass criteria:** Zero compilation errors. The following artifacts exist:
- `build\bin\Debug\CorsixModStudio.exe`
- `build\lib\Debug\rainman.lib`
- `build\lib\Debug\lua502.lib`
- `build\lib\Debug\lua512.lib`
- `build\bin\Debug\rainman_tests.exe`
- `build\bin\Debug\cdms_tests.exe`

### T1.2 — Clean Build (Release)

**Steps:**
1. Run `cmake --build --preset release`.

**Pass criteria:** Zero compilation errors. Same artifacts exist under `build\bin\Release\` and `build\lib\Release\`.

### T1.3 — Automated Test Suite

**Steps:**
1. Run `ctest --preset debug --output-on-failure`.

**Pass criteria:** All tests pass (400+ tests, 0 failures).

---

## Area 2: Application Startup & Basic UI (P0)

*8 tests — Verify the application launches, displays correctly, and exits cleanly.*

### T2.1 — Application Launches Without Crash

**Steps:**
1. Run `build\bin\Debug\CorsixModStudio.exe`.
2. Observe the application window.
3. Check console/debug output for errors.

**Pass criteria:** Main window appears. No crash dialog. No unhandled exception.

### T2.2 — Welcome Tab Displayed

**Steps:**
1. On launch, observe the main content area.

**Pass criteria:** Welcome tab (frmWelcome) is visible with welcome content.

### T2.3 — Menu Bar Present

**Steps:**
1. Inspect the menu bar at the top of the window.

**Pass criteria:** All top-level menus present and clickable: **File**, **Mod**, **Tools** (Relic Tools), **Play**, **Help**.

### T2.4 — File Menu Items

**Steps:**
1. Click the **File** menu.
2. Inspect available items.

**Pass criteria:**
- Contains: New Mod, Open Mod (with submenu for DoW/DC/SS/CoH), Open SGA, Close, Exit.
- Close is disabled when no mod is loaded.

### T2.5 — Help Menu Items

**Steps:**
1. Click the **Help** menu.
2. Inspect available items.

**Pass criteria:** Contains Credits, Help Contents, RDN Wiki links, forum links, Lua Reference, About.

### T2.6 — Help → Credits

**Steps:**
1. Click Help → Credits.
2. Observe the dialog. Close it.

**Pass criteria:** Credits dialog (frmCredits) opens, displays content, and closes without crash.

### T2.7 — Help → About

**Steps:**
1. Click Help → About (or equivalent menu item).
2. Observe the dialog. Close it.

**Pass criteria:** About dialog shows version/build information and closes without crash.

### T2.8 — Clean Exit

**Steps:**
1. Close the application via File → Exit.
2. Repeat by closing via the window's X button.

**Pass criteria:** Application exits cleanly both ways. No crash on shutdown. Process fully terminates (verify in Task Manager).

---

## Area 3: Configuration & Runtime Data (P3)

*4 tests — Verify config file handling and graceful degradation.*

### T3.1 — Config File Loading

**Steps:**
1. Ensure `Mod_Studio_Files\config.txt` exists in the working directory.
2. Launch the application.

**Pass criteria:** No config-related error dialogs. Sash position and file tree color settings from config.txt are respected.

### T3.2 — First-Run Detection

**Steps:**
1. Set `firstrun=1` in `config.txt`.
2. Launch the application.

**Pass criteria:** First-run behavior activates (welcome wizard or initial setup, if implemented).

### T3.3 — Missing Runtime Data — Graceful Handling

**Steps:**
1. Rename `Mod_Studio_Files\` to `Mod_Studio_Files_backup\`.
2. Launch the application.
3. Restore the directory after the test.

**Pass criteria:** Application launches without crash. May show warnings but does not abort.

### T3.4 — Sash Position Persistence

**Steps:**
1. Launch the application.
2. Drag the file tree / content splitter to a new position.
3. Close and relaunch.

**Pass criteria:** Splitter position is restored to the position set before closing.

---

## Area 4: Module Loading (P1)

*10 tests — Verify loading modules for all supported game types.*

> Tests T4.1–T4.9 require a game installation. If unavailable, use T4.10
> (synthetic module) as a substitute.

### T4.1 — Open Mod Dialog (Dawn of War)

**Steps:**
1. Click File → Open Mod → Dawn of War.

**Pass criteria:** A file/directory browser opens targeting the DoW install path.

### T4.2 — Open Mod Dialog (Dark Crusade)

**Steps:**
1. Click File → Open Mod → Dark Crusade.

**Pass criteria:** A file/directory browser opens targeting the DC install path.

### T4.3 — Open Mod Dialog (Soulstorm)

**Steps:**
1. Click File → Open Mod → Soulstorm.

**Pass criteria:** A file/directory browser opens targeting the SS install path.

### T4.4 — Open Mod Dialog (Company of Heroes)

**Steps:**
1. Click File → Open Mod → Company of Heroes.

**Pass criteria:** A file/directory browser opens targeting the CoH install path.

### T4.5 — Load a Real Module File

**Steps:**
1. Select a valid `.module` file from a game installation.
2. Observe the loading process.

**Pass criteria:**
- Loading progress dialog (frmLoading) appears with a progress bar.
- File tree populates after loading completes.
- No crash occurs.
- Module properties are accessible via Mod → Properties.

### T4.6 — Loading Progress & Cancel

**Steps:**
1. Start loading a large module (e.g., full CoH).
2. Click **Cancel** on the loading dialog before it completes.

**Pass criteria:** Loading aborts cleanly. No crash. Application returns to its previous state.

### T4.7 — File Tree Population

**Steps:**
1. Load a module successfully.
2. Examine the file tree panel (frmFiles).

**Pass criteria:**
- Tree shows the folder hierarchy from the module.
- Files are color-coded per `config.txt` settings:
  - **This mod's files**: dark red (RGB 128/64/64)
  - **Other mod's files**: dark blue (RGB 64/64/128)
  - **Engine files**: gray (RGB 128/128/128)

### T4.8 — Module Properties

**Steps:**
1. With a module loaded, click Mod → Properties.

**Pass criteria:** Module settings dialog opens showing mod name, version, dependencies, data folders, and archive list.

### T4.9 — Close Mod

**Steps:**
1. With a module loaded, click File → Close (or Mod → Close).

**Pass criteria:** Module unloaded. File tree cleared. No crash. Application returns to the welcome state.

### T4.10 — Load Synthetic Module (No Game Required)

**Steps:**
1. Create a file named `TestMod.module` with the following content:
   ```ini
   [global]
   ModFolder = TestMod
   UCSName = Test Mod
   Description = A test module
   DllName = TestMod
   ModVersion = 1
   [files]
   [folds]
   [incompatible]
   ```
2. Create a matching directory structure (`TestMod\`) with at least one data file.
3. Load via File → Open Mod.

**Pass criteria:** Module loads (possibly with warnings about missing resources). No crash.

---

## Area 5: SGA Archive Operations (P1)

*5 tests — Verify SGA reading, extraction, and creation.*

### T5.1 — Open SGA Archive

**Steps:**
1. Click File → Open SGA.
2. Select a valid `.sga` file.

**Pass criteria:** Archive contents shown in the file tree. Files and folders are browsable.

### T5.2 — Extract Single File from SGA

**Steps:**
1. With an SGA loaded, right-click a file → Extract.
2. Choose a destination folder.

**Pass criteria:** File extracted to disk. File contents match the original.

### T5.3 — Extract Folder from SGA

**Steps:**
1. Right-click a folder in the SGA tree → Extract.
2. Choose a destination folder.

**Pass criteria:** Entire folder extracted with correct directory structure preserved.

### T5.4 — SGA Packer Tool

**Steps:**
1. Load a module.
2. Click Mod → Tools → SGA Packer.
3. Configure output path and source folders.
4. Click Build/Pack.

**Pass criteria:** SGA file created at the specified path. The new SGA can be reopened with File → Open SGA.

### T5.5 — Extract All Tool

**Steps:**
1. Load a module.
2. Click Mod → Tools → Extract All.
3. Choose a destination folder.

**Pass criteria:** All files extracted. Progress indicator shown. No crash, even on large modules.

---

## Area 6: File Viewing & Editors (P1)

*14 tests — Verify all file viewers and editors open, display, and save correctly.*

### T6.1 — View Lua File

**Steps:**
1. In the file tree, double-click (or right-click → View) a `.lua` file.

**Pass criteria:** Text content displayed in a viewer/editor tab with readable formatting.

### T6.2 — View SCAR Script

**Steps:**
1. Double-click a `.scar` file in the file tree.

**Pass criteria:** SCAR editor (frmScarEditor) opens with syntax highlighting.

### T6.3 — Edit SCAR Script

**Steps:**
1. Open a SCAR file.
2. Make a text edit.
3. Save the file.

**Pass criteria:** Changes saved to disk. Re-opening confirms the edit persisted. No crash.

### T6.4 — View Image (TGA / DDS / RGT)

**Steps:**
1. Double-click a texture file (`.tga`, `.dds`, or `.rgt`) in the file tree.

**Pass criteria:** Image viewer (frmImage) opens and renders the texture correctly.

### T6.5 — RGD Editor — Open

**Steps:**
1. Right-click an `.rgd` file → Edit RGD.

**Pass criteria:** RGD editor (frmRgdEditor) opens displaying a property tree with keys and values.

### T6.6 — RGD Editor — Edit Value

**Steps:**
1. In the RGD editor, modify a value (change a number, string, or boolean).
2. Save the file.

**Pass criteria:** File saved. Re-opening shows the modified value.

### T6.7 — RGD Editor — Add/Delete Node

**Steps:**
1. Right-click in the RGD editor tree → Add child node.
2. Right-click a node → Delete.

**Pass criteria:** Node added/removed. Tree view updates accordingly.

### T6.8 — RGD → Lua Dump

**Steps:**
1. Right-click an `.rgd` file in the file tree → Dump to Lua.

**Pass criteria:** Lua representation generated and displayed in a text viewer tab.

### T6.9 — UCS Editor — Open

**Steps:**
1. Load a module with UCS files.
2. Click Mod → Tools → UCS Tool.

**Pass criteria:** UCS editor (frmUCSEditor) opens with a string table. Entries listed with numeric IDs and text values.

### T6.10 — UCS Editor — Edit Entry

**Steps:**
1. Select an existing string entry in the UCS editor.
2. Modify its text. Save.

**Pass criteria:** Changes saved. Re-opening shows the modified text.

### T6.11 — UCS Editor — Add Entry

**Steps:**
1. Add a new string entry with a unique numeric ID and text value.
2. Save.

**Pass criteria:** Entry added to the table. Save succeeds. Entry persists on re-open.

### T6.12 — RGM Material Editor

**Steps:**
1. Right-click an `.rgm` file in the file tree → Edit.

**Pass criteria:** Material editor (frmRgmMaterialEditor) opens and displays material properties.

### T6.13 — BFX → Lua Dump

**Steps:**
1. Right-click a `.bfx` file → Dump to Lua.

**Pass criteria:** Lua representation generated and displayed.

### T6.14 — Text File Viewer

**Steps:**
1. Right-click a plain text file (`.txt`, `.ini`, etc.) → View.

**Pass criteria:** Text viewer (frmText) opens and displays the file content.

---

## Area 7: File Conversion Operations (P2)

*5 tests — Verify format conversion pipelines.*

### T7.1 — Lua Burn (Lua → RGD)

**Steps:**
1. Right-click a Lua data file → Burn Lua.

**Pass criteria:** Corresponding `.rgd` file created in the correct output location (Data, not DataGeneric — see BUG-D06).

### T7.2 — DDS → RGT Conversion

**Steps:**
1. Right-click a `.dds` file → Convert to RGT.

**Pass criteria:** `.rgt` file created. Viewable in the image viewer (T6.4).

### T7.3 — RGT → TGA Conversion

**Steps:**
1. Right-click an `.rgt` file → Save as TGA (or equivalent export action).

**Pass criteria:** `.tga` file created with valid image data.

### T7.4 — RGO Burn

**Steps:**
1. Right-click an `.rgo` file → Burn.

**Pass criteria:** File processed without crash.

### T7.5 — BFX Deburn

**Steps:**
1. Right-click a `.bfx` file → Deburn.

**Pass criteria:** Lua source recovered from the binary BFX.

---

## Area 8: Tools Menu (P2)

*8 tests — Verify all tools accessible from the Mod menu.*

### T8.1 — Locale Selector

**Steps:**
1. Load a module with UCS localization files.
2. Click Mod → Tools → Locale Selector.
3. Select a locale (e.g., French, German).

**Pass criteria:** Locale selector (frmLocaleSelector) opens showing available locales. After selection, UCS strings switch to the chosen locale.

### T8.2 — Attribute Snapshot

**Steps:**
1. Load a module.
2. Click Mod → Tools → Attribute Snapshot.

**Pass criteria:** Tool runs without crash. Produces an output report or saves snapshot data.

### T8.3 — DPS Calculator

**Steps:**
1. Load a module.
2. Click Mod → Tools → DPS Calculator.
3. Select units and trigger calculation.

**Pass criteria:** Calculator opens. DPS values computed and displayed.

### T8.4 — Refresh Files

**Steps:**
1. Load a module.
2. Add or remove a file on disk within the module's data folders.
3. Click Mod → Tools → Refresh Files.

**Pass criteria:** File tree updates to reflect the filesystem changes.

### T8.5 — Make Lua Inherit Tree

**Steps:**
1. Load a module.
2. Click Mod → Tools → Make Lua Inherit Tree.

**Pass criteria:** Lua inheritance tree viewer (frmLuaInheritTree) opens and displays a class/table hierarchy.

### T8.6 — AE Setup Tool

**Steps:**
1. Load a module.
2. Click Mod → Tools → AE Setup.

**Pass criteria:** Tool opens and allows configuring Relic Attribute Editor paths.

### T8.7 — Red Button Tool

**Steps:**
1. Load a module.
2. Click Mod → Tools → Red Button.

**Pass criteria:** Tool runs its batch operation. No crash.

### T8.8 — RGD Macro Tool

**Steps:**
1. Open an `.rgd` file in the RGD editor.
2. Open the macro tool (frmRgdMacro).
3. Load a macro script from `Mod_Studio_Files\rgd_macros\` (e.g., `Find DoW DPS.lua`).
4. Execute the macro.

**Pass criteria:** Macro executes. Results visible in the RGD editor or output panel.

---

## Area 9: Context Menu Actions (P2)

*6 tests — Verify right-click actions on files and folders in the file tree.*

### T9.1 — Copy File Path

**Steps:**
1. Right-click a file in the tree → Copy Path.
2. Paste into a text editor.

**Pass criteria:** The file's path is on the clipboard.

### T9.2 — Extract File

**Steps:**
1. Right-click a file → Extract.
2. Choose a destination folder.

**Pass criteria:** File extracted correctly to the chosen location.

### T9.3 — MD5 Hash Scan

**Steps:**
1. Right-click a file → Scan MD5.

**Pass criteria:** MD5 hash displayed in a dialog or logged.

### T9.4 — File-Type-Specific Context Menus

**Steps:**
1. Right-click files of each type: `.lua`, `.rgd`, `.scar`, `.tga`, `.ucs`.
2. Inspect the context menu for each.

**Pass criteria:** Each file type shows appropriate actions:
- `.lua` → View, Burn
- `.rgd` → Edit RGD, Dump to Lua
- `.scar` → Edit SCAR
- `.tga` / `.dds` / `.rgt` → View Image, Convert
- `.ucs` → Edit UCS

### T9.5 — Folder Context Menu

**Steps:**
1. Right-click a folder in the file tree → Extract.

**Pass criteria:** Folder extraction dialog appears. All contained files are extracted with correct directory structure.

### T9.6 — File Color Coding

**Steps:**
1. Load a module that includes files from this mod, required mods, and the engine.
2. Observe the file tree colors.

**Pass criteria:** Colors match `config.txt` settings:
- This mod's files: dark red (RGB 128, 64, 64)
- Other mod's files: dark blue (RGB 64, 64, 128)
- Engine files: gray (RGB 128, 128, 128)

---

## Area 10: Play Menu & External Tools (P3)

*4 tests — Verify game launch and Relic tool integration.*

### T10.1 — Play Menu Items

**Steps:**
1. Click the **Play** menu.

**Pass criteria:** Shows game launch options (CoH, W40k, WXP, Dark Crusade, Soulstorm). **Dev Mode** and **No Movies** toggle items are present.

### T10.2 — Relic Tools Menu

**Steps:**
1. Click the **Tools** (Relic Tools) menu.

**Pass criteria:** Shows: Attribute Editor, Audio Editor, Chunky Viewer, FX Tool, Mission Editor, Mod Packager, Object Editor.

### T10.3 — Launch Game (If Installed)

**Steps:**
1. Click Play → [an installed game].

**Pass criteria:** Game executable launches.

> **Known issue (BUG-B01):** The launch button may start vanilla CoH instead
> of the loaded mod. This is a pre-existing bug from the original 0.5.5.

### T10.4 — View Warnings Log

**Steps:**
1. Click Play → View Warnings Log.

**Pass criteria:** Log viewer opens showing game warning output (or indicates no log exists).

---

## Area 11: New Mod Workflow (P3)

*4 tests — Verify creating a new mod from scratch.*

### T11.1 — File → New Mod

**Steps:**
1. Click File → New Mod.

**Pass criteria:** New mod dialog (frmNewMod) opens with fields for mod name, target game, and folder location.

### T11.2 — Create a New Mod

**Steps:**
1. Fill in mod name, target game, and folder location.
2. Click Create.

**Pass criteria:** `.module` file created on disk. Directory structure created. Mod auto-loaded into the application.

### T11.3 — Edit New Mod Properties

**Steps:**
1. With the new mod loaded, open Mod → Properties.
2. Change description, version, or dependencies.
3. Save.

**Pass criteria:** `.module` file updated on disk. Changes persist on re-open.

### T11.4 — Build New Mod to SGA

**Steps:**
1. Place data files in the new mod's data folder.
2. Use SGA Packer (Mod → Tools → SGA Packer) to build an archive.

**Pass criteria:** SGA archive created containing the new mod's files.

---

## Area 12: Edge Cases & Error Handling (P3)

*6 tests — Verify robustness under abnormal conditions.*

### T12.1 — Load Non-Existent Module

**Steps:**
1. Attempt to open a `.module` file path that does not exist.

**Pass criteria:** Error dialog shown with a meaningful message. No crash.

### T12.2 — Load Corrupt SGA

**Steps:**
1. Create a file with `.sga` extension containing garbage data.
2. Attempt to open it via File → Open SGA.

**Pass criteria:** Error message shown. No crash.

### T12.3 — Load Empty Module

**Steps:**
1. Create a `.module` file with zero content (empty file).
2. Attempt to open it.

**Pass criteria:** Error or warning dialog shown. No crash.

### T12.4 — Large Module Performance

**Steps:**
1. Load a large module (e.g., full CoH or DoW with all expansions).
2. Observe the loading process.

**Pass criteria:**
- Loading completes (may take time).
- UI remains responsive during load (progress dialog is interactive).
- Progress bar shows advancement.

### T12.5 — Multiple Close/Open Cycles

**Steps:**
1. Load a module, close it.
2. Load a different module, close it.
3. Repeat 5+ times.
4. Monitor process memory in Task Manager.

**Pass criteria:** No crash. No stale UI state. Memory usage does not grow unboundedly.

### T12.6 — Window Resize & Layout

**Steps:**
1. Resize the main window to very small dimensions.
2. Resize to very large dimensions.
3. Toggle maximize / restore.
4. Minimize and restore.
5. Drag the file tree / content splitter.

**Pass criteria:** Layout adapts gracefully. No rendering artifacts. Splitter remains functional.

---

## Area 13: Release Build Validation (P3)

*3 tests — Verify the Release configuration matches Debug behavior with better performance.*

### T13.1 — Release Build Launches

**Steps:**
1. Run `build\bin\Release\CorsixModStudio.exe`.

**Pass criteria:** Application launches identically to the Debug build (same UI, same menus).

### T13.2 — Release Build Core Workflow

**Steps:**
1. In the Release build: load a module → browse files → open an editor → close the module → exit.

**Pass criteria:** All functionality works. No assertions or crashes.

### T13.3 — Release Build Performance

**Steps:**
1. Time module loading in the Debug build.
2. Time module loading in the Release build with the same module.

**Pass criteria:** Release build is noticeably faster than Debug.

---

## Known Limitations

These are pre-existing issues from the original Corsix 0.5.5 that are **not**
expected to be fixed in this port. Tests touching these areas should be marked
SKIP or noted accordingly.

| Bug ID | Summary |
|--------|---------|
| BUG-B01 | Play menu launches vanilla game instead of the loaded mod |
| BUG-B04 | Soulstorm support is incomplete ("initial" only) |
| BUG-U01 | `CDoWFileView` has an unimplemented method |
| BUG-U03 | `CLuaFile2` reference handling is stubbed out |

See [docs/known-bugs.md](known-bugs.md) for the full catalogue.

## Test Environment Requirements

| Component | Version |
|-----------|---------|
| **OS** | Windows 10 or 11 (x64) |
| **Compiler** | Visual Studio 2022 or later (MSVC) |
| **Build system** | CMake 3.25+ with presets |
| **Package manager** | vcpkg (VCPKG_ROOT must be set) |
| **Dependencies** | wxWidgets 3.2, zlib, spdlog, libsquish, Google Test |
| **Game** *(optional)* | Dawn of War / Dark Crusade / Soulstorm / Company of Heroes |

## Summary

| Area | Priority | Tests | Description |
|------|----------|-------|-------------|
| 1. Build Verification | P0 | 3 | Clean build Debug + Release, automated test suite |
| 2. Application Startup | P0 | 8 | Launch, welcome tab, menus, credits, clean exit |
| 3. Configuration | P3 | 4 | Config loading, first-run, missing data, sash persistence |
| 4. Module Loading | P1 | 10 | Open mod dialogs, load/close, file tree, properties |
| 5. SGA Archives | P1 | 5 | Open, extract, pack, mass extract |
| 6. File Editors | P1 | 14 | RGD, UCS, SCAR, RGM, image, Lua, BFX, text viewers |
| 7. File Conversion | P2 | 5 | Lua burn, DDS↔RGT, RGT→TGA, RGO, BFX deburn |
| 8. Tools Menu | P2 | 8 | Locale, snapshot, DPS calc, refresh, inherit tree, macros |
| 9. Context Menus | P2 | 6 | Copy path, extract, MD5, file-type menus, color coding |
| 10. Play & Tools | P3 | 4 | Game launch, Relic tools, warnings log |
| 11. New Mod | P3 | 4 | Create mod, edit properties, build to SGA |
| 12. Edge Cases | P3 | 6 | Corrupt files, large mods, memory, window resize |
| 13. Release Build | P3 | 3 | Launch, core workflow, performance comparison |
| **Total** | | **80** | |
