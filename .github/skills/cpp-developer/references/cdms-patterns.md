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

// ASCII → Unicode (caller must delete[] result)
wchar_t* wsResult = AsciiToUnicode("hello");
delete[] wsResult;

// Unicode → ASCII (caller must delete[] result)
char* sResult = UnicodeToAscii(L"hello");
delete[] sResult;

// ASCII → wxString
wxString wxResult = AsciiTowxString("hello");

// wxString → ASCII (caller must delete[] result)
char* sFromWx = wxStringToAscii(myWxString);
delete[] sFromWx;
```

**Important**: All pointer-returning functions allocate with `new[]`. Callers must `delete[]` the result.

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

1. Create the `.h` and `.cpp` in `src/cdms/`.
2. Include `Common.h` first in the `.cpp`.
3. Use the LGPL file header.
4. Files are auto-discovered by `file(GLOB)` — just rebuild.
5. If adding a new window, register it in the appropriate menu handler in `Construct.cpp`.
