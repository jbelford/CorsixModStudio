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
#include "common/TabDialog.h"
#include "frmUCSEditor.h"
#include "frmUCSOutRange.h"
#include "frame/Construct.h"
#include "tools/Tools.h"
#include "common/strings.h"
#include "common/config.h"
#include "common/Utility.h"
#include "presenters/CUcsEditorPresenter.h"
#include <wx/msgdlg.h>
#include <wx/toolbar.h>
#include <wx/tbarbase.h>
#include <memory>
#include <algorithm>
#include "common/Common.h"
#include "common/ThemeColours.h"
#include <wx/generic/msgdlgg.h>
#include "rainman/localization/CUcsMap.h"

BEGIN_EVENT_TABLE(frmUCSEditor, wxWindow)
EVT_SIZE(frmUCSEditor::OnSize)
EVT_PG_CHANGED(IDC_PropertyGrid, frmUCSEditor::OnPropertyChange)
EVT_BUTTON(wxID_NEW, frmUCSEditor::OnNewEntry)
EVT_TOOL(IDC_ToolAdd, frmUCSEditor::OnNewEntry)
EVT_BUTTON(wxID_SAVE, frmUCSEditor::OnSaveFile)
EVT_TOOL(IDC_ToolSave, frmUCSEditor::OnSaveFile)
EVT_BUTTON(wxID_OPEN, frmUCSEditor::OnLoad)

EVT_BUTTON(wxID_APPLY, frmUCSEditor::OnApply)
EVT_TOOL(IDC_ToolApply, frmUCSEditor::OnApply)
EVT_BUTTON(wxID_CANCEL, frmUCSEditor::OnClose)
EVT_TOOL(IDC_ToolClose, frmUCSEditor::OnClose)

EVT_CLOSE(frmUCSEditor::OnCloseWindow)
END_EVENT_TABLE()

void frmUCSEditor::OnClose(wxCommandEvent &event)
{
    UNUSED(event);
    wxCloseEvent oClose;
    oClose.SetCanVeto(true);
    OnCloseWindow(oClose);
    if (oClose.GetVeto())
    {
        return;
    }

    auto *pParent = (frmTabDialog *)GetParent()->GetParent();
    pParent->EndModal(wxID_OK);
}

void frmUCSEditor::OnApply(wxCommandEvent &event)
{
    wxPGProperty *oSelected = m_pPropertyGrid->GetSelectedProperty();
    if (oSelected == nullptr)
    {
        return;
    }

    if (m_pResultVal)
    {
        wxString sStr = m_pPropertyGrid->GetPropertyLabel(oSelected);
        *m_pResultVal = wcstoul(sStr.c_str() + 1, nullptr, 10);
    }
    OnClose(event);
}

frmUCSEditor::frmUCSEditor(wxWindow *parent, wxWindowID id, bool bReadOnly, const wxPoint &pos, const wxSize &size,
                           unsigned long *pResult)
    : wxWindow(parent, id, pos, size), m_taskRunner(this)
{
    m_pTabStripForLoad = nullptr;
    m_pUCS = nullptr;
    m_pResultVal = pResult;
    m_bNeedsSave = false;
    m_bReadOnly = bReadOnly;
    auto *pTopSizer = new wxBoxSizer(wxVERTICAL);

    wxToolBar *pToolbar;
    pTopSizer->Add(pToolbar = new wxToolBar(this, -1, wxDefaultPosition, wxDefaultSize,
                                            wxTB_NODIVIDER | wxTB_HORIZONTAL | wxNO_BORDER),
                   0, wxEXPAND | wxALL, 3);
    wxBitmap oSaveBmp(wxT("IDB_32SAVE"), wxBITMAP_TYPE_BMP_RESOURCE),
        oAddBmp(wxT("IDB_32ADD"), wxBITMAP_TYPE_BMP_RESOURCE);
    wxBitmap oApplyBmp(wxT("IDB_32APPLY"), wxBITMAP_TYPE_BMP_RESOURCE),
        oCancelBmp(wxT("IDB_32CANCEL"), wxBITMAP_TYPE_BMP_RESOURCE);
    oSaveBmp.SetMask(new wxMask(oSaveBmp, wxColour(128, 128, 128)));
    oAddBmp.SetMask(new wxMask(oAddBmp, wxColour(128, 128, 128)));
    oApplyBmp.SetMask(new wxMask(oApplyBmp, wxColour(128, 128, 128)));
    oCancelBmp.SetMask(new wxMask(oCancelBmp, wxColour(128, 128, 128)));
    pToolbar->SetToolBitmapSize(wxSize(32, 32));
    pToolbar->AddTool(IDC_ToolSave, AppStr(ucsedit_save), oSaveBmp, AppStr(ucsedit_save));
    pToolbar->AddTool(IDC_ToolAdd, AppStr(ucsedit_newentry), oAddBmp, AppStr(ucsedit_newentry));
    if (m_pResultVal)
    {
        pToolbar->AddTool(IDC_ToolClose, AppStr(ucsedit_rgdcancel), oCancelBmp, AppStr(ucsedit_rgdcancel));
        pToolbar->AddTool(IDC_ToolApply, AppStr(ucsedit_rgdapply), oApplyBmp, AppStr(ucsedit_rgdapply));
    }
    pToolbar->Realize();

    pTopSizer->Add(m_pPropertyGrid = new wxPropertyGrid(this, IDC_PropertyGrid, wxDefaultPosition, wxDefaultSize,
                                                        wxPG_DEFAULT_STYLE | wxPG_HIDE_MARGIN),
                   1, wxEXPAND | wxALL, 0);
    ThemeColours::ApplyPropertyGridTheme(m_pPropertyGrid);

    m_pLoadingLabel = new wxStaticText(this, wxID_ANY, wxT("Loading UCS entries..."), wxDefaultPosition, wxDefaultSize,
                                       wxALIGN_CENTRE_HORIZONTAL);
    m_pLoadingLabel->Hide();

    auto *pButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxWindow *pBgTemp;

    pButtonSizer->Add(m_pLoadButton = new wxButton(this, wxID_OPEN, AppStr(ucsedit_load_title)), 0, wxEXPAND | wxALL,
                      3);
    if (m_pResultVal)
    {
        pButtonSizer->Add(new wxButton(this, wxID_APPLY, AppStr(ucsedit_rgdapply)), 0, wxEXPAND | wxALL, 3);
        pButtonSizer->Add(new wxButton(this, wxID_CANCEL, AppStr(ucsedit_rgdcancel)), 0, wxEXPAND | wxALL, 3);
    }
    pButtonSizer->Add(pBgTemp = new wxButton(this, wxID_NEW, AppStr(ucsedit_newentry)), 0, wxEXPAND | wxALL, 3);
    pButtonSizer->Add(new wxButton(this, wxID_SAVE, AppStr(ucsedit_save)), 0, wxEXPAND | wxALL, 3);

    pTopSizer->Add(pButtonSizer, 0, wxALIGN_RIGHT);

    SetBackgroundColour(pBgTemp->GetBackgroundColour());

    m_pLoadButton->Show(false);

    SetSizer(pTopSizer);
    pTopSizer->SetSizeHints(this);
}

void frmUCSEditor::SetTabStripForLoad(wxAuiNotebook *pTabStrib)
{
    m_pTabStripForLoad = pTabStrib;

    m_pLoadButton->Show(m_pTabStripForLoad != nullptr);

    Layout();
}

void frmUCSEditor::OnLoad(wxCommandEvent &event)
{
    UNUSED(event);
    auto *pSelector = new frmUCSSelector(wxT("Select UCS to get value from"));
    CUcsTool::HandleSelectorResponse(&*pSelector, m_pTabStripForLoad, m_pResultVal, true);
}

frmUCSEditor::~frmUCSEditor() {}

void frmUCSEditor::FillFromCUcsFile(std::shared_ptr<CUcsFile> pUcs, unsigned long iSelect)
{
    m_pUCS = std::make_unique<CUcsTransaction>(pUcs);

    // Show loading state
    m_pPropertyGrid->Disable();
    m_pLoadingLabel->Show();
    Layout();

    // Prepare the sorted snapshot on a background thread
    m_taskRunner.RunAsync<UcsEntryVec>(
        [pUcs = std::move(pUcs)](CProgressChannel &progress, CCancellationToken &cancel) -> UcsEntryVec
        {
            UNUSED(progress);
            UNUSED(cancel);
            auto &sortedMap = pUcs->GetRawMap().GetSortedMap();
            UcsEntryVec entries;
            entries.reserve(sortedMap.size());
            for (auto &entry : sortedMap)
            {
                if (entry.second)
                {
                    entries.emplace_back(entry.first, entry.second);
                }
            }
            return entries;
        },
        [](const std::string &) {},
        [this, iSelect](Result<UcsEntryVec> result)
        {
            m_pLoadingLabel->Hide();
            m_pPropertyGrid->Enable();
            if (!result.ok())
            {
                ThemeColours::ShowMessageBox(result.error(), wxT("UCS Load Error"), wxICON_ERROR, this);
                return;
            }
            PopulateGrid(std::move(result).value(), iSelect);
        });
}

void frmUCSEditor::PopulateGrid(UcsEntryVec entries, unsigned long iSelect)
{
    wxPGProperty *oSelectMe = nullptr;

    m_pPropertyGrid->Freeze();
    for (auto &entry : entries)
    {
        auto sNumberBuffer = L"$" + std::to_wstring(entry.first);
        auto stringProp = std::make_unique<wxStringProperty>(sNumberBuffer, sNumberBuffer, entry.second.get());
        if (m_bReadOnly)
        {
            stringProp->Enable(false);
        }
        if (iSelect == entry.first)
        {
            oSelectMe = stringProp.get();
        }

        m_pPropertyGrid->Append(stringProp.release());
    }
    m_pPropertyGrid->Thaw();

    m_pPropertyGrid->SetSplitterLeft();

    if (oSelectMe != nullptr)
    {
        m_pPropertyGrid->EnsureVisible(oSelectMe);
        m_pPropertyGrid->SelectProperty(oSelectMe, true);
    }
}

void frmUCSEditor::OnSize(wxSizeEvent &event)
{
    UNUSED(event);
    Layout();
}

void frmUCSEditor::OnPropertyChange(wxPropertyGridEvent &event)
{
    unsigned long iID = wcstoul(((const wchar_t *)event.GetProperty()->GetLabel()) + 1, nullptr, 10);
    try
    {
        m_pUCS->SetString(iID, event.GetPropertyValue().GetString());
    }
    catch (const CRainmanException &e)
    {
        ErrorBoxE(e);
    }
    m_bNeedsSave = true;
}

void frmUCSEditor::OnNewEntry(wxCommandEvent &event)
{
    UNUSED(event);
    if (m_bReadOnly)
    {
        ThemeColours::ShowMessageBox(AppStr(ucsedit_readonlyerror), AppStr(ucsedit_newentry), wxICON_ERROR, this);
        return;
    }

    wxString sDefaultId;
    wchar_t sNumberBuffer[34];
    sNumberBuffer[0] = '$';
    try
    {
        const CUcsMap &pMap = m_pUCS->GetRawMap();
        if (!pMap.Empty())
        {
            unsigned long iNext = CUcsEditorPresenter::SuggestNextId(pMap);
            _ultow(iNext, sNumberBuffer + 1, 10);
        }
        else
        {
            wcscpy(sNumberBuffer, AppStr(ucsedit_newentrydefault));
        }
    }
    catch (const CRainmanException &e)
    {
        ErrorBoxE(e);
        return;
    }

    wxString sNewID;
    sNewID = wxGetTextFromUser(AppStr(ucsedit_newentrycaption), AppStr(ucsedit_newentry), sNumberBuffer, this,
                               wxDefaultCoord, wxDefaultCoord, false);
    if (sNewID.IsEmpty())
    {
        return;
    }

    unsigned long iNewID = 0;
    if (!CUcsEditorPresenter::ParseIdFromInput(sNewID, iNewID))
    {
        return;
    }

    if (!CUcsEditorPresenter::IsIdInRecommendedRange(iNewID))
    {
        bool bDontAsk;
        TheConfig->Read(AppStr(config_mod_ucsrangeremember), &bDontAsk, false);
        if (!bDontAsk)
        {
            auto *pQuestion = new frmUCSOutOfRange(AppStr(ucsrange_title), iNewID);
            if (pQuestion->ShowModal() == wxID_NO)
            {
                delete pQuestion;
                return;
            }
            delete pQuestion;
        }
    }

    const CUcsMap &entries = m_pUCS->GetRawMap();

    if (CUcsEditorPresenter::ValidateNewId(iNewID, entries) == CUcsEditorPresenter::IdValidation::Duplicate)
    {
        ThemeColours::ShowMessageBox(AppStr(ucsedit_newentrydupcaption), AppStr(ucsedit_newentryduptitle), wxICON_ERROR,
                                     this);
        return;
    }

    // Find insertion point (sorted order)
    for (auto itr = entries.begin(); itr != entries.end(); ++itr)
    {
        if (itr->second && itr->first >= iNewID)
        {
            sNumberBuffer[0] = '$';
            _ultow(itr->first, sNumberBuffer + 1, 10);
            wxPGProperty *oEntry = m_pPropertyGrid->GetPropertyByLabel(sNumberBuffer);
            _ultow(iNewID, sNumberBuffer + 1, 10);
            try
            {
                m_pUCS->SetString(iNewID, L"");
                m_bNeedsSave = true;
            }
            catch (const CRainmanException &e)
            {
                ErrorBoxE(e);
                return;
            }
            oEntry = m_pPropertyGrid->Insert(oEntry, new wxStringProperty(sNumberBuffer, sNumberBuffer, wxT("")));
            m_pPropertyGrid->Refresh();
            m_pPropertyGrid->EnsureVisible(oEntry);
            m_pPropertyGrid->SelectProperty(oEntry, true);
            return;
        }
    }

    // Append at end
    sNumberBuffer[0] = '$';
    _ultow(iNewID, sNumberBuffer + 1, 10);
    try
    {
        m_pUCS->SetString(iNewID, L"");
        m_bNeedsSave = true;
    }
    catch (const CRainmanException &e)
    {
        ErrorBoxE(e);
        return;
    }
    wxPGProperty *oEntry = m_pPropertyGrid->Append(new wxStringProperty(sNumberBuffer, sNumberBuffer, wxT("")));
    m_pPropertyGrid->Refresh();
    m_pPropertyGrid->EnsureVisible(oEntry);
    m_pPropertyGrid->SelectProperty(oEntry, true);
    if (m_pPropertyGrid->GetRoot()->GetChildCount() == 1)
    {
        m_pPropertyGrid->SetSplitterLeft();
    }
}

void frmUCSEditor::OnSaveFile(wxCommandEvent &event)
{
    UNUSED(event);
    if (m_bReadOnly)
    {
        ThemeColours::ShowMessageBox(AppStr(ucsedit_readonlyerror), AppStr(ucsedit_save), wxICON_ERROR, this);
        return;
    }

    DoSave();
}

void frmUCSEditor::DoSave()
{
    wxString sModuleDir = TheConstruct->GetModuleFile().BeforeLast('\\');
    wxString sNewFile;
    try
    {
        auto &modSvc = TheConstruct->GetModuleService();
        sNewFile = CUcsEditorPresenter::BuildLocaleBasePath(
            sModuleDir, modSvc.GetModuleType(), modSvc.GetLocaleFolder(), modSvc.GetModFolder(), modSvc.GetLocale());
    }
    catch (const CRainmanException &e)
    {
        ErrorBoxE(e);
        return;
    }

    try
    {
        size_t iUCSCount = TheConstruct->GetModuleService().GetUcsCount();
        for (size_t i = 0; i < iUCSCount; ++i)
        {
            CModuleFile::CUcsHandler *pUcs = TheConstruct->GetModuleService().GetUcs(i);
            if (pUcs->GetUcsHandle().get() == m_pUCS->GetRawObject())
            {
                sNewFile.Append(AsciiTowxString(pUcs->GetFileName()));
                goto found_ucs_file;
            }
        }

        if (TheConstruct->GetModuleService().GetModuleType() == CModuleFile::MT_CompanyOfHeroes)
        {
            size_t iModCount = TheConstruct->GetModuleService().GetRequiredCount();
            for (size_t i = 0; i < iModCount; ++i)
            {
                const CModuleFile *pMod = TheConstruct->GetModuleService().GetRequired(i)->GetModHandle();
                iUCSCount = pMod->GetUcsCount();
                for (size_t i = 0; i < iUCSCount; ++i)
                {
                    const CModuleFile::CUcsHandler *pUcs = pMod->GetUcs(i);
                    if (pUcs->GetUcsHandle().get() == m_pUCS->GetRawObject())
                    {
                        sNewFile.Append(AsciiTowxString(pUcs->GetFileName()));
                        goto found_ucs_file;
                    }
                }
            }

            iModCount = TheConstruct->GetModuleService().GetEngineCount();
            for (size_t i = 0; i < iModCount; ++i)
            {
                const CModuleFile *pMod = TheConstruct->GetModuleService().GetEngine(i);
                iUCSCount = pMod->GetUcsCount();
                for (size_t i = 0; i < iUCSCount; ++i)
                {
                    const CModuleFile::CUcsHandler *pUcs = pMod->GetUcs(i);
                    if (pUcs->GetUcsHandle().get() == m_pUCS->GetRawObject())
                    {
                        sNewFile.Append(AsciiTowxString(pUcs->GetFileName()));
                        goto found_ucs_file;
                    }
                }
            }
        }
        throw CModStudioException(__FILE__, __LINE__, "Unable to match UCS handle (should never happen; o.O)");
    }
    catch (const CRainmanException &e)
    {
        ErrorBoxE(e);
        return;
    }
found_ucs_file:

    BackupFile(sNewFile);
    auto saNewFile = wxStringToAscii(sNewFile);
    try
    {
        m_pUCS->Save(saNewFile.get());
    }
    catch (const CRainmanException &e)
    {
        ErrorBoxE(e);
        RestoreBackupFile(sNewFile);
        return;
    }

    m_bNeedsSave = false;
    ThemeColours::ShowMessageBox(AppStr(ucsedit_save_caption), AppStr(ucsedit_save_title), wxICON_INFORMATION, this);
}

void frmUCSEditor::OnCloseWindow(wxCloseEvent &event)
{
    if (m_bNeedsSave)
    {
        auto *dialog = new wxGenericMessageDialog(this, wxT("UCS has been modified. Save file?"), wxT("UCS Editor"),
                                                  wxYES_NO | wxCANCEL);

        int ans = dialog->ShowModal();
        dialog->Destroy();

        switch (ans)
        {
        case wxID_YES:
            DoSave();
            break;

        case wxID_CANCEL:
        default:
            if (event.CanVeto())
            {
                event.Veto();
            }
        case wxID_NO:
            break;
        }
    }
}
