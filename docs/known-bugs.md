# Corsix Mod Studio — Known Bugs & Issues Tracker

A comprehensive catalogue of known bugs, quirks, missing features, and
user-reported problems in Corsix Mod Studio 0.5.5 (and earlier).  Items here
form the backlog of things to address in the modernised CorsixModStudio
codebase.

> **Living document** — add new entries as they are discovered during
> development.  Reference bug IDs (e.g. `BUG-C03`) in commit messages and
> code comments when a fix is made.

---

## Severity Legend

| Level | Meaning |
|-------|---------|
| **Critical** | Crash, data loss, or security vulnerability |
| **High** | Feature completely broken or unusable |
| **Medium** | Incorrect behaviour; workaround exists |
| **Low** | Cosmetic, minor UX annoyance, or nice-to-have |

---

## 1 — Crashes & Stability

These were reported as fixed in various releases, but the underlying code
patterns (unchecked pointers, raw `new`/`delete`, no input validation) suggest
similar issues may lurk elsewhere.

| ID | Summary | Severity | Status in 0.5.5 | Source |
|----|---------|----------|------------------|--------|
| BUG-C01 | Crash when adding entries to a UCS file | Critical | Fixed 0.5.5 | [Changelog](https://modstudio.corsix.org/) |
| BUG-C02 | Crashes with DoW DPS calculator tool | Critical | Fixed 0.5.5 | [Changelog](https://modstudio.corsix.org/) |
| BUG-C03 | Crash when loading or burning Lua files | Critical | Fixed 0.5.5 | [Changelog](https://modstudio.corsix.org/) |
| BUG-C04 | Crash during mod loading for CoH mods with unconventional directory structures | Critical | Fixed 0.5.4 | [Changelog](https://modstudio.corsix.org/) |
| BUG-C05 | Crash from "Extract All" tool | Critical | Fixed 0.5.1 | [Changelog](https://modstudio.corsix.org/) |
| BUG-C06 | Crash when right-clicking whitespace in RGD editor | Critical | Fixed 0.5.0 | [Changelog](https://modstudio.corsix.org/) |
| BUG-C07 | Crash when right-clicking tree items | Critical | Fixed 0.5.0b | [Changelog](https://modstudio.corsix.org/) |
| BUG-C08 | Possible crash when saving a copy of an image | Critical | Fixed 0.5.2 | [Changelog](https://modstudio.corsix.org/) |
| BUG-C09 | **Buffer overflow in SGA loading code** — potential security vulnerability | Critical | Fixed 0.5.3 | [Changelog](https://modstudio.corsix.org/) |
| BUG-C10 | Non-SGA files in a CoH scenario pack folder cause errors during mod loading | Medium | Fixed 0.5.4 | [Changelog](https://modstudio.corsix.org/) |

**Pattern note:** Many of these were described as "possible crash" or "fixed
potential crash", indicating defensive checks were added after the fact rather
than the root cause being addressed.  The modernised codebase should audit all
similar code paths.

---

## 2 — Data Corruption & Incorrect Behaviour

| ID | Summary | Severity | Status in 0.5.5 | Source |
|----|---------|----------|------------------|--------|
| BUG-D01 | RGD editor required excessively frequent manual saves — unsaved data silently lost if the user navigated away | High | Improved 0.5.2 ("you shouldn't have to press Save so regularly any more") | [Changelog](https://modstudio.corsix.org/) |
| BUG-D02 | UCS editor retained unsaved changes in memory after closing; stale data could be written on next save | High | Fixed 0.5.1 | [Changelog](https://modstudio.corsix.org/) |
| BUG-D03 | Extracting a folder did not extract all contained files | High | Fixed 0.5.0 | [Changelog](https://modstudio.corsix.org/) |
| BUG-D04 | Extracting from SGA archives with more than 32,767 files failed (integer overflow — `short` used for file count) | High | Fixed 0.5.3 | [Changelog](https://modstudio.corsix.org/) |
| BUG-D05 | Converting RGT files to TGA produced incorrect output | Medium | Fixed 0.5.1 | [Changelog](https://modstudio.corsix.org/) |
| BUG-D06 | When Lua files were burnt from DataGeneric, the resulting RGD files were saved into DataGeneric instead of Data | Medium | Fixed 0.5.5 | [Changelog](https://modstudio.corsix.org/) |

---

## 3 — Broken Features (still present in 0.5.5)

These are features that shipped but stopped working (or never fully worked)
and remain broken in the final 0.5.5 release.

| ID | Summary | Severity | Details | Source |
|----|---------|----------|---------|--------|
| BUG-B01 | **"Launch mod from Corsix" button always launches vanilla CoH** instead of the selected mod | High | Confirmed broken since at least CoH patch 2.6.  Clicking the launch button ignores the loaded module and starts vanilla Company of Heroes.  Workaround: create a Windows shortcut with `-mod ModName` launch parameter. | [Eastern Front forums](https://www.easternfront.org/forums/index.php?topic=12365.0), [Steam Community](https://steamcommunity.com/app/228200/discussions/0/1470841715933316262/) |
| BUG-B02 | **File renames not detected** — renaming a file externally (via Windows Explorer) is not reflected in Corsix until the user manually selects Mod → Refresh File List | Medium | Noted as a known limitation in community tutorials.  Users must close the file tab, rename in Explorer, then refresh. | [Revora tutorial](https://forums.revora.net/topic/118733-tutorial-corsix-mod-studio/) |
| BUG-B03 | **Module file sometimes protected from saving** — users report being unable to save changes to `.module` files after editing | Medium | May be related to file permissions or the module being loaded from a read-only SGA.  Exact reproduction unclear. | [Eastern Front forums](https://www.easternfront.org/forums/index.php?topic=12365.0) |
| BUG-B04 | **Soulstorm support is incomplete** — 0.5.5 added only "initial" Soulstorm support; some Soulstorm-specific file formats or module structures may not be handled | Medium | The changelog explicitly says "Initial DoW:Soulstorm support", implying known gaps. | [Changelog](https://modstudio.corsix.org/) |
| BUG-B05 | **Corsix outdated since CoH patch 2.400** — community members note that game patches broke Corsix compatibility; a patched module file was once provided but the download link is dead | High | Reported by multiple users.  Exact breakage unclear but affects mod creation workflow. | [Eastern Front forums](https://www.easternfront.org/forums/index.php?topic=12365.0) |

---

## 4 — Modern OS Compatibility

These are fundamental compatibility issues that prevent the original 0.5.5
binary from running on modern systems.

| ID | Summary | Severity | Details | Source |
|----|---------|----------|---------|--------|
| BUG-O01 | **MSVCR71.dll not found** — the 0.5.5 binary was compiled with Visual C++ 7.1 (VS 2003); modern Windows 10/11 does not ship this runtime | Critical | Users cannot launch the program at all.  Manually placing the DLL in System32/SysWOW64 sometimes works but is unreliable. | [Reddit r/dawnofwar (2023)](https://www.reddit.com/r/dawnofwar/comments/16avclm/corsixs_mod_studio_help/) |
| BUG-O02 | **32-bit only (x86)** — no 64-bit build exists; relies on WoW64 subsystem | Medium | Not a crash issue on current Windows, but limits addressable memory and causes DLL search-path issues.  Our modernised build targets x64. | Codebase analysis |
| BUG-O03 | **Windows-only** — no Linux or macOS support | Low | Feature request from 2008 on Revora forums.  The Rainman library is being modernised with platform-neutral I/O; full cross-platform is a future goal. | [Revora forums](https://forums.revora.net/topic/61772-corsix-and-linux/) |

---

## 5 — Unimplemented Features (TODO stubs in source code)

These are features that Corsix started but never finished, indicated by `TODO`
comments or `QUICK_THROW("TODO")` in the source.

| ID | Summary | Severity | Location | Source |
|----|---------|----------|----------|--------|
| BUG-U01 | `CDoWFileView` has an unimplemented method that throws "TODO: Need to implement this :D" | Medium | `src/rainman/CDoWFileView.cpp:139` | Codebase |
| BUG-U02 | `CCohUcsFile` has an unfinished TODO block in its implementation | Medium | `src/rainman/CCohUcsFile.cpp:147` | Codebase |
| BUG-U03 | `CLuaFile2` reference handling is entirely stubbed out — `VSetReferenceType`, `VSetReferenceString`, `VSetReferenceWString`, and `SGetNodeFromRainmanRgd` all throw "TODO" | High | `src/rainman/CLuaFile2.cpp:854-856, 1093, 1162, 1164` | Codebase |
| BUG-U04 | `frmLuaInheritTree` has an unfinished TODO in the UI code | Low | `src/cdms/frmLuaInheritTree.cpp:210` | Codebase |

---

## 6 — UX & Quality-of-Life Issues

Problems that don't cause crashes or data loss but make the tool frustrating
to use day-to-day.

| ID | Summary | Severity | Details | Source |
|----|---------|----------|---------|--------|
| BUG-X01 | **No undo/redo** in RGD editor, UCS editor, or SCAR editor | Medium | Users must manually revert changes by re-opening files.  Especially painful given BUG-D01 (data loss on navigation). | Codebase analysis |
| BUG-X02 | **No automatic file-system watching** — external changes to mod files are invisible until manual refresh | Low | Related to BUG-B02.  Modern editors use `ReadDirectoryChangesW` or similar to auto-detect file changes. | Community tutorials |
| BUG-X03 | **No search/find across files** — users cannot search for a string across all files in a mod | Low | Common feature request; users must know exact file paths. | Codebase analysis |
| BUG-X04 | **No input validation on RGD values** — entering invalid data (e.g. leaving `false` in a text field) can crash the game at runtime with no warning from the tool | Medium | Noted in Revora tutorial: "make sure to remove false from the value, as that will crash the game if you leave it there." | [Revora tutorial](https://forums.revora.net/topic/118733-tutorial-corsix-mod-studio/) |
| BUG-X05 | **Limited error messages** — many error conditions result in a crash or silent failure rather than a user-friendly error dialog | Medium | Observed in the exception pattern: `throw new CRainmanException(...)` without catch handlers in many code paths. | Codebase analysis |
| BUG-X06 | **No batch editing** — users cannot apply changes across multiple RGD files simultaneously (the RGD macro feature in 0.5.2 partially addresses this but is limited) | Low | Feature gap noted in community discussions. | Community observations |
| BUG-X07 | **Icons on Tools tab arranged incorrectly on startup** | Low | Fixed in 0.5.0 but historically fragile. | [Changelog](https://modstudio.corsix.org/) |

---

## 7 — Architectural & Technical Debt

Structural issues in the codebase that make bugs more likely and maintenance
harder.  These are not user-visible bugs per se but are root causes of many
items above.

| ID | Summary | Severity | Details | Source |
|----|---------|----------|---------|--------|
| BUG-T01 | **Raw `new`/`delete` memory management** — no RAII, no smart pointers; memory leaks are likely throughout | Medium | Root cause of many crash bugs.  Stream ownership semantics (`VOpenStream` returns heap-allocated stream, caller must `delete`) are error-prone. | Codebase analysis |
| BUG-T02 | **Non-standard exception pattern** — `throw new CRainmanException(...)` with manual `->destroy()` cleanup; easy to leak exception objects | Medium | C++ best practice is stack-based exceptions.  This pattern exists for legacy reasons (DLL boundary compatibility). | Codebase analysis |
| BUG-T03 | **`unsigned long` used instead of `size_t`/`uint32_t`** — assumes 32-bit throughout; root cause of BUG-D04 (>32767 files bug) | Medium | Changing this would cascade through the entire API surface.  The modernised build should gradually migrate. | Codebase analysis |
| BUG-T04 | **Vendored zLib** — ships a private copy of zLib rather than using the system or vcpkg version | Low | Our build now uses vcpkg zlib for new code, but the vendored copy remains for legacy compatibility. | `src/rainman/zLib/` |
| BUG-T05 | **wxWidgets 2.8 era UI** — the CDMS GUI targets wxWidgets 2.8; migration to 3.x is required for modern OS support and High-DPI | High | Phase 4 of the modernisation plan.  wx 3.x has breaking API changes (sizer flags, event macros). | Codebase analysis, `src/cdms/` |
| BUG-T06 | **DLL/EXE split design** — original shipped as `Rainman.dll` + `ModStudio.exe` but the exception pattern and memory management make DLL boundaries fragile | Low | Currently built as a static lib.  If DLL builds are ever needed, the API boundary needs redesign. | Codebase analysis |
| BUG-T07 | **No logging in original code** — no diagnostics framework; failures are silent or crash | Medium | We have since added spdlog-based `RainmanLog`, but much legacy code doesn't use it yet. | Codebase analysis |
| BUG-T08 | **Entirely single-threaded** — no concurrency; large mod loading blocks the UI | Low | Modern UI frameworks expect background work.  The Rainman library should be made thread-safe before any async loading is added. | Codebase analysis |
| BUG-T09 | **No automated tests in original** — zero test coverage; all testing was manual | Medium | We have since added Google Test infrastructure and initial tests for core classes. | Codebase analysis |

---

## Sources

| Source | URL / Location |
|--------|----------------|
| Official Corsix Mod Studio website & changelog | https://modstudio.corsix.org/ |
| Reddit r/dawnofwar — MSVCR71.dll issue (2023) | https://www.reddit.com/r/dawnofwar/comments/16avclm/corsixs_mod_studio_help/ |
| Steam Community — can't run mods (2017) | https://steamcommunity.com/app/228200/discussions/0/1470841715933316262/ |
| Eastern Front forums — launch button broken (2017) | https://www.easternfront.org/forums/index.php?topic=12365.0 |
| Revora Forums — Corsix Mod Studio Forum | https://forums.revora.net/forum/2010-corsix-mod-studio-forum/ |
| Revora Forums — tutorial noting quirks | https://forums.revora.net/topic/118733-tutorial-corsix-mod-studio/ |
| Codebase TODO/FIXME analysis | `src/rainman/`, `src/cdms/` |
