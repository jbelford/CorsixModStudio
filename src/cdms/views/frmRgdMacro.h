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
#include "presenters/CRgdMacroPresenter.h"
#include "views/interfaces/IRgdMacroView.h"
#include <memory>
#include <rainman/formats/CRgdFileMacro.h>
#include <wx/stc/stc.h>
#include <wx/treectrl.h>

class frmRgdMacro : public wxDialog, public IRgdMacroView
{
  protected:
    wxStyledTextCtrl *m_pSTC;
    wxTextCtrl *m_pTextbox;

    wxStaticText *m_pCaption;
    wxButton *m_pSaveBtn, *m_pLoadBtn, *m_pRunBtn, *m_pModeBtn;
    wxBoxSizer *m_pFormMainSizer;

    bool m_bShowingOutput;

    unsigned long m_iPathHash;
    unsigned long m_iPathLen;

    wxString m_sPath;
    wxTreeItemId &m_oFolder;

    std::unique_ptr<CRgdMacroPresenter> m_pPresenter;

    void _populateFileList(std::vector<char *> *lstFiles);

  public:
    frmRgdMacro(wxString sFile, wxTreeItemId &oFolder);

    void OnRunClick(wxCommandEvent &event);
    void OnModeClick(wxCommandEvent &event);
    void OnLoadClick(wxCommandEvent &event);
    void OnSaveClick(wxCommandEvent &event);
    void OnCancelClick(wxCommandEvent &event);
    void OnStyleNeeded(wxStyledTextEvent &event);
    void OnCharAdded(wxStyledTextEvent &event);

    // IRgdMacroView implementation
    void ShowProgress(const wxString &sMessage) override;
    void HideProgress() override;
    void ShowError(const wxString &sMessage) override;
    void AppendOutput(const wxString &sText) override;
    bool RequestPermission(const wxString &sQuestion) override;
    void OnMacroComplete(const std::vector<wxString> &vFoldersToUpdate) override;
    void DisableControls() override;
    void EnableControls() override;

    enum
    {
        IDC_Run = wxID_HIGHEST + 1,
        IDC_Load,
        IDC_Save,
        IDC_Cancel,
        IDC_Mode,
    };

    DECLARE_EVENT_TABLE()
};
