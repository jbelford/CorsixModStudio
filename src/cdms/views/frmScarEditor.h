/*
    This file is part of Corsix's Mod Studio.

    Corsix's Mod Studio is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Corsix's Mod Studio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Corsix's Mod Studio; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
// ----------------------------
#include <rainman/io/IFileStore.h>
#include <wx/stc/stc.h>
#include <wx/timer.h>
#include <list>
#include <stack>
#include <wx/treectrl.h>
#include <wx/popupwin.h>
#include "common/strings.h"
#include "interfaces/ISaveable.h"
#include <lsp/Protocol.h>

class frmScarEditor : public wxWindow, public ISaveable
{
  protected:
    wxStyledTextCtrl *m_pSTC;
    wxString m_sFilename;
    wxChoice *m_pFunctionDropdown;
    wxTreeItemId m_oFileParent;
    bool m_bNeedsSaving;

    // --- LSP integration ---
    wxTimer m_lspTimer;
    std::string m_sLspUri;
    bool m_bLspOpen = false;
    int m_iLspVersion = 0;
    bool m_bLspNeedsSync = false;
    std::vector<lsp::Diagnostic> m_vDiagnostics;
    wxPopupTransientWindow *m_pHoverPopup = nullptr;
    static constexpr int INDICATOR_LSP_ERROR = 8;
    static constexpr int INDICATOR_LSP_WARNING = 9;
    static constexpr int INDICATOR_LSP_INFO = 10;

    void OnLspTimer(wxTimerEvent &event);
    void OnDwellStart(wxStyledTextEvent &event);
    void OnDwellEnd(wxStyledTextEvent &event);
    void DismissHoverPopup();
    void LspOpenDocument();
    void LspSyncDocument();
    void LspCloseDocument();
    void ApplyDiagnostics(const std::string &uri, std::vector<lsp::Diagnostic> diagnostics);

    struct _ScarFunction
    {
        char *sReturn;
        char *sName;
        std::list<char *> sArguments;
        char *sDesc;
        int iType;
    };

    struct _CCalltipPop
    {
        int iPos;
        wxString sTip;
    };

    _CCalltipPop m_oThisCalltip;
    std::stack<_CCalltipPop> m_stkCalltips;
    std::list<_ScarFunction> m_lstScarFunctions;
    static char *_ReadNiceString(FILE *f);
    void _RestorePreviousCalltip();
    void _PushThisCalltip();
    void ShowAutoComplete();
    int FillFunctionDrop(const wxString &sNameTarget);

  public:
    frmScarEditor(const wxTreeItemId &oFileParent, wxString sFilename, wxWindow *parent, wxWindowID id,
                  const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize,
                  const wchar_t *pLangRef = AppStr(app_scarreffile));
    ~frmScarEditor();

    void OnSize(wxSizeEvent &event);

    void OnCharAdded(wxStyledTextEvent &event);
    void OnStyleNeeded(wxStyledTextEvent &event);
    void OnSavePointLeave(wxStyledTextEvent &event);
    void OnSavePointReach(wxStyledTextEvent &event);

    // ISaveable
    bool IsModified() const override { return m_bNeedsSaving; }
    void DoSave() override;
    void OnSave(wxCommandEvent &event);
    void OnCheckLua(wxCommandEvent &event);
    void OnAutoCompChoose(wxStyledTextEvent &event);
    void OnKeyDown(wxKeyEvent &event);
    void OnFuncListChoose(wxCommandEvent &event);

    void Load(IFileStore::IStream *pFile);

    void OnCloseWindow(wxCloseEvent &event);

    enum
    {
        IDC_Text = wxID_HIGHEST + 1,
        IDC_Compile,
        IDC_ToolSave,
        IDC_ToolCheck,
        IDC_FunctionDrop
    };

    DECLARE_EVENT_TABLE()
};
