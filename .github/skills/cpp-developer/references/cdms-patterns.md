# CDMS GUI Patterns

## Architecture

CDMS (`src/cdms/`) is a wxWidgets GUI application built on top of the Rainman library. It provides the modding IDE for Dawn of War and Company of Heroes.

## wxWidgets 3.x Conventions

### Event Tables

CDMS uses the macro-based event table system:

```cpp
// In the header:
class frmMyWindow : public wxFrame
{
    DECLARE_EVENT_TABLE()
public:
    frmMyWindow(wxWindow* parent);

private:
    void OnButtonClick(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
};

// In the implementation:
BEGIN_EVENT_TABLE(frmMyWindow, wxFrame)
    EVT_BUTTON(ID_MY_BUTTON, frmMyWindow::OnButtonClick)
    EVT_CLOSE(frmMyWindow::OnClose)
END_EVENT_TABLE()
```

### Unicode Strings

CDMS uses Unicode throughout. String literals use `wxT()` or `_T()` macros:

```cpp
wxString sTitle = wxT("Corsix's Mod Studio");
wxMessageBox(_T("Operation complete"), _T("Info"));
```

Both macros still work in wxWidgets 3.x. Use whichever is already dominant in the file you're editing.

### Property Grid

wxPropertyGrid is used in RGD and material editors. Key types:

```cpp
// Properties return wxPGProperty* (not the old wxPGId)
wxPGProperty* pProp = m_pGrid->Append(new wxStringProperty(wxT("Name"), wxPG_LABEL, value));

// Check validity with nullptr, not wxPGIdIsOk()
if (pProp != nullptr) { ... }

// Get selected property
wxPGProperty* pSelected = m_pGrid->GetSelectedProperty();
```

## String Conversion Utilities

`strconv.h` provides conversion between `char*`, `wchar_t*`, and `wxString`:

```cpp
#include "strconv.h"

// ASCII → Unicode — wrap in unique_ptr immediately
auto wsResult = std::unique_ptr<wchar_t[]>(AsciiToUnicode("hello"));

// Unicode → ASCII — wrap in unique_ptr immediately
auto sResult = std::unique_ptr<char[]>(UnicodeToAscii(L"hello"));

// ASCII → wxString (returns by value, no ownership concern)
wxString wxResult = AsciiTowxString("hello");

// wxString → ASCII — wrap in unique_ptr immediately
auto sFromWx = std::unique_ptr<char[]>(wxStringToAscii(myWxString));
```

**Important**: All pointer-returning functions allocate with `new[]`. **Always** wrap returns
in `std::unique_ptr<T[]>` — never hold a raw owning `char*`/`wchar_t*`.

## Common.h

Most CDMS source files include `Common.h` which pulls in:
- wxWidgets core headers
- Debug memory tracking (`DEBUG_NEW` macro in debug builds)
- Standard type definitions

When writing new CDMS files, include `Common.h` first, then Rainman headers.

## Naming in CDMS

CDMS uses a slightly different naming style than Rainman:
- Window classes: `frm` prefix (`frmRgdEditor`, `frmSgaMake`, `frmWelcome`)
- Dialog classes: standard wx naming or `frm` prefix
- Event IDs: `ID_` prefix with uppercase (`ID_MY_BUTTON`)
- Application globals: accessed via `AppStr(name)` macro from `strings.h`

## Adding a New CDMS Source File

1. Create the `.h` and `.cpp` in `src/cdms/` (or the appropriate subdirectory: `services/`, `async/`, `actions/`).
2. Include `Common.h` first in the `.cpp`.
3. Use the LGPL file header.
4. Files are auto-discovered by `file(GLOB)` — just rebuild.
5. If adding a new window, register it in the appropriate menu handler in `Construct.cpp`.
6. **Boy-scout rule**: When modifying existing CDMS code, apply localized modern C++ improvements
   (e.g., `NULL` → `nullptr`, add `override`, use range-for, add `const`).
   **Ownership boy-scout**: migrate raw `new`/`delete` to `std::unique_ptr` or `std::shared_ptr`
   when the change is safe and localized within the files being modified.

## Service Layer (`src/cdms/services/`)

Services wrap Rainman calls with `Result<T>` error handling, insulating the GUI from raw exceptions:

```cpp
#include "services/Result.h"

Result<wxString> MyService::DoSomething()
{
    try {
        // Rainman operation...
        return Result<wxString>::Ok(wxT("success"));
    }
    catch (CRainmanException* pE) {
        return ResultFromExceptionT<wxString>(pE);
    }
}
```

Access services via `GetModuleService()`, `GetFileService()`, `GetFormatService()`, `GetHashService()` on the `ConstructFrame`.

## Async Infrastructure (`src/cdms/async/`)

- `CCancellationToken` — cooperative cancellation (header-only)
- `CProgressChannel` — thread-safe progress reporting with RainmanCallback bridge
- `CTaskRunner` — runs work on `std::thread`, returns results via callback
- `CWxTaskRunner` — extends `CTaskRunner`, posts results to the main thread via `CallAfter()`
