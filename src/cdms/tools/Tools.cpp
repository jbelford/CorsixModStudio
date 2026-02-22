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

#include "Tools.h"
#include "common/strings.h"

#include "views/frmUCSEditor.h"
#include "views/frmFileSelector.h"
#include "views/frmLocaleSelector.h"
#include "common/strconv.h"
#include "views/frmSgaMake.h"
#include "Tool_AESetup.h"
#include "Tool_AutoDPS.h"
#include "presenters/CDpsCalculatorPresenter.h"
#include "presenters/CRefreshFilesPresenter.h"
#include "common/Utility.h"
#include "views/frmMassExtract.h"
#include <memory>
// Rainman_RGDDump.h removed; RgdDump usage is commented-out dead code
#include <rainman/util/Util.h>
#include <rainman/io/CFileSystemStore.h>
#include <rainman/io/IFileStore.h>
#include "common/Common.h"
#include "common/ThemeColours.h"
#include "rainman/core/RainmanLog.h"

wxString CLocaleTool::GetName() { return AppStr(locale); }
wxString CLocaleTool::GetHelpString() { return wxT(""); }
wxString CLocaleTool::GetBitmapName() { return wxT("IDB_TOOL_LOC"); }
void CLocaleTool::DoAction()
{
    ConstructFrame::eLoadModGames eGame;
    switch (TheConstruct->GetModuleService().GetModuleType())
    {
    case CModuleFile::MT_CompanyOfHeroesEarly:
    case CModuleFile::MT_CompanyOfHeroes:
        eGame = ConstructFrame::LM_CoH_OF;
        break;

    case CModuleFile::MT_DawnOfWar:
        eGame = ConstructFrame::LM_DoW_WA;
        break;

    default:
        eGame = ConstructFrame::LM_Any;
        break;
    };

    frmLocaleSelector *pLocaleSelect = new frmLocaleSelector(AppStr(localeselect_title), eGame);
    pLocaleSelect->ShowModal();
    delete pLocaleSelect;

    TheConstruct->DoTool(AppStr(refreshfiles_name));
}

wxString CAESetupTool::GetName() { return AppStr(aesetup_name); }
wxString CAESetupTool::GetHelpString() { return AppStr(aesetup_help); }
wxString CAESetupTool::GetBitmapName() { return wxT("IDB_TOOL_AESETUP"); }
void CAESetupTool::DoAction()
{
    frmUCSToDAT oMake;
    oMake.ShowModal();
}

wxString CUcsTool::GetName() { return AppStr(ucs_editor); }
wxString CUcsTool::GetHelpString() { return wxT(""); }
wxString CUcsTool::GetBitmapName() { return wxT("IDB_TOOL_UCS"); }

void CUcsTool::HandleSelectorResponse(frmUCSSelector *pSelector, wxAuiNotebook *pTabsDestination,
                                      unsigned long *pResult, bool bRegisterTabStrip)
{
    frmUCSEditor *pForm;

    switch (pSelector->ShowModal())
    {
    case wxID_NEW:
    {
        size_t iUCSCount = TheConstruct->GetModuleService().GetUcsCount();
        for (size_t i = 0; i < iUCSCount; ++i)
        {
            CModuleFile::CUcsHandler *pUcs = TheConstruct->GetModuleService().GetUcs(i);
            if (AsciiTowxString(pUcs->GetFileName()) == pSelector->GetAnswer())
            {
                ThemeColours::ShowMessageBox(AppStr(file_newucs_dup_caption), AppStr(file_newucs_title), wxICON_ERROR,
                                             TheConstruct);
                delete pSelector;
                return;
            }
        }
        wxString sNewFile = TheConstruct->GetModuleFile().BeforeLast('\\');
        sNewFile.Append(wxT("\\"));
        if (TheConstruct->GetModuleService().GetModuleType() == CModuleFile::MT_CompanyOfHeroes)
        {
            sNewFile.Append(wxT("Engine\\Locale\\"));
        }
        else
        {
            wxString sLocFolder = TheConstruct->GetModuleService().GetLocaleFolder();
            if (!sLocFolder.IsEmpty())
            {
                sNewFile.Append(sLocFolder);
                sNewFile.Append(wxT("\\"));
            }
            else
            {
                sNewFile.Append(TheConstruct->GetModuleService().GetModFolder());
                sNewFile.Append(wxT("\\Locale\\"));
            }
        }
        sNewFile.Append(TheConstruct->GetModuleService().GetLocale());
        sNewFile.Append(wxT("\\"));
        sNewFile.Append(pSelector->GetAnswer());

        auto saNewFile = wxStringToAscii(sNewFile);
        auto pNewUcs = std::make_shared<CUcsFile>();
        try
        {
            pNewUcs->Save(saNewFile.get());
        }
        catch (const CRainmanException &e)
        {
            delete pSelector;
            ErrorBoxE(e);
            return;
        }
        {
            auto result = TheConstruct->GetModuleService().NewUCS(pSelector->GetAnswer(), pNewUcs);
            if (!result)
            {
                ErrorBoxS(result.error());
                return;
            }
        }
        pTabsDestination->AddPage(
            pForm = new frmUCSEditor(pTabsDestination, -1, false, wxDefaultPosition, wxDefaultSize, pResult),
            wxString()
                .Append(AppStr(ucsedit_tabname))
                .Append(wxT(" ["))
                .Append(pSelector->GetAnswer())
                .Append(wxT("]")),
            true);
        pForm->FillFromCUcsFile(pNewUcs);
        break;
    }
    case wxID_OPEN:
    {
        pTabsDestination->AddPage(pForm = new frmUCSEditor(pTabsDestination, -1, pSelector->IsAnswerUcsReadOnly(),
                                                           wxDefaultPosition, wxDefaultSize, pResult),
                                  wxString()
                                      .Append(AppStr(ucsedit_tabname))
                                      .Append(wxT(" ["))
                                      .Append(pSelector->GetAnswer())
                                      .Append(wxT("]")),
                                  true);
        pForm->FillFromCUcsFile(pSelector->GetAnswerUcs());
        /*
        size_t iUCSCount = TheConstruct->GetModule()->GetUcsCount();
        for(size_t i = 0; i < iUCSCount; ++i)
        {
            CModuleFile::CUcsHandler* pUcs = TheConstruct->GetModule()->GetUcs(i);
            if(AsciiTowxString(pUcs->GetFileName()) == pSelector->GetAnswer())
            {
                pForm->FillFromCUcsFile(pUcs->GetUcsHandle());
            }
        }
        */
        break;
    }
    default:
        return; // should never happen?
    };

    if (bRegisterTabStrip)
    {
        pForm->SetTabStripForLoad(pTabsDestination);
    }

    delete pSelector;
}

void CUcsTool::DoAction()
{
    HandleSelectorResponse(new frmUCSSelector(AppStr(ucsselect_title)), TheConstruct->GetTabs());
}

wxString CSgaPackerTool::GetName() { return AppStr(sgapack_title); }
wxString CSgaPackerTool::GetHelpString() { return wxT(""); }
wxString CSgaPackerTool::GetBitmapName() { return wxT("IDB_SGAPACK"); }
void CSgaPackerTool::DoAction()
{
    frmSgaMake oMake;
    oMake.ShowModal();
}

wxString CExtractAllTool::GetName() { return AppStr(massext_toolname); }
wxString CExtractAllTool::GetHelpString() { return wxT(""); }
wxString CExtractAllTool::GetBitmapName() { return wxT("IDB_TOOL_EXTALL"); }
void CExtractAllTool::DoAction()
{
    frmMassExtract oMassExtract(wxT(""), TheConstruct->GetFilesList()->GetTree()->GetRootItem(), true);
    oMassExtract.ShowModal();
}

wxString CDpsCalculatorTool::GetName() { return AppStr(dpscalculator_title); }
wxString CDpsCalculatorTool::GetHelpString() { return wxT(""); }
wxString CDpsCalculatorTool::GetBitmapName() { return wxT("IDB_TOOL_CALCULATOR"); }
void CDpsCalculatorTool::DoAction()
{
    CDMS_LOG_INFO("Running Auto DPS tool");
    wxString sOutFile = wxFileSelector(AppStr(dpscalculator_outselect), wxT(""), wxT(""), wxT("html"),
                                       AppStr(dpscalculator_filter), wxFD_SAVE | wxFD_OVERWRITE_PROMPT, TheConstruct);

    if (sOutFile.Len() == 0)
    {
        return;
    }

    auto itrResult = TheConstruct->GetFileService().Iterate(wxT("data\\attrib\\weapon"));
    if (!itrResult)
    {
        ErrorBoxE(CModStudioException(__FILE__, __LINE__, "Cannot iterate over weapon files"));
        return;
    }
    auto *pItr = itrResult.value().release();

    if (!m_pPresenter)
    {
        m_pPresenter =
            std::make_unique<CDpsCalculatorPresenter>(static_cast<IDpsCalculatorView &>(*this), TheConstruct);
    }

    if (!m_pPresenter->CalculateDps(sOutFile, pItr, TheConstruct->GetModuleService().GetModule()))
    {
        delete pItr;
        ThemeColours::ShowMessageBox(wxT("DPS calculation is already running."), AppStr(dpscalculator_title),
                                     wxICON_WARNING, TheConstruct);
    }
}

void CDpsCalculatorTool::ShowProgress(const wxString & /*sMessage*/) {}
void CDpsCalculatorTool::HideProgress() {}
void CDpsCalculatorTool::ShowError(const wxString &sMessage)
{
    ThemeColours::ShowMessageBox(sMessage, AppStr(dpscalculator_title), wxICON_ERROR, TheConstruct);
}
void CDpsCalculatorTool::OnDpsComplete()
{
    ThemeColours::ShowMessageBox(AppStr(dpscalculator_done), AppStr(dpscalculator_title), wxICON_INFORMATION,
                                 TheConstruct);
}
void CDpsCalculatorTool::DisableControls() {}
void CDpsCalculatorTool::EnableControls() {}

wxString CMakeLuaInheritTree::GetName() { return wxT("Make Lua Inheritance Tree"); }
wxString CMakeLuaInheritTree::GetHelpString() { return wxT(""); }
wxString CMakeLuaInheritTree::GetBitmapName() { return wxT("IDB_REDBUTTON"); }
bool CMakeLuaInheritTree::_DoesExist(const char *sFol)
{
    auto result = TheConstruct->GetFileService().DirectoryExists(AsciiTowxString(sFol));
    return result.ok() && result.value();
}

void CMakeLuaInheritTree::_ForEach(IDirectoryTraverser::IIterator *pItr, void *pTag)
{
    const char *sN = pItr->VGetName();
    sN = strrchr(sN, '.');
    if (sN)
    {
        if ((strnicmp(sN, ".lua", 4) == 0) || (strnicmp(sN, ".nil", 4) == 0))
        {
            CMakeLuaInheritTree *pC = (CMakeLuaInheritTree *)pTag;
            pC->_DoLua(pItr);
        }
    }
}

void CMakeLuaInheritTree::_DoLua(IDirectoryTraverser::IIterator *pItr)
{
    std::unique_ptr<IFileStore::IStream> pStream(pItr->VOpenFile());
    char *sRef = 0;

    CLuaFile oLua;
    try
    {
        sRef = oLua.GetReferencedFile(pStream.get());
    }
    catch (const CRainmanException &e)
    {
        sRef = 0;
    }

    if (sRef == 0)
    {
        unsigned long iChildCount;
        try
        {
            pStream->VSeek(0, IFileStore::IStream::SL_Root);
            oLua.Load(pStream.get(), TheConstruct->GetModuleService().GetModule(), pItr->VGetFullPath());
            iChildCount = oLua.VGetChildCount();
        }
        catch (const CRainmanException &e)
        {
            ErrorBoxE(e);
            return;
        }

        for (unsigned long i = 0; i < iChildCount; ++i)
        {
            IMetaNode *pChild = 0;
            IMetaNode::IMetaTable *pTable = 0;

            try
            {
                pChild = oLua.VGetChild(i);
                if (strcmp(pChild->VGetName(), "GameData") != 0)
                {
                    delete pChild;
                    continue;
                }
                pTable = pChild->VGetValueMetatable();
                if (pTable->VGetReferenceType() == IMetaNode::DT_String)
                {
                    sRef = strdup(pTable->VGetReferenceString());
                }
                else if (pTable->VGetReferenceType() == IMetaNode::DT_WString)
                {
                    const wchar_t *pS = pTable->VGetReferenceWString();
                    signed long iS = -1;
                    sRef = (char *)malloc(wcslen(pS) + 1);
                    do
                    {
                        ++iS;
                        sRef[iS] = pS[iS];
                    } while (pS[iS]);
                }
                else
                {
                    sRef = strdup("");
                }
            }
            catch (const CRainmanException &e)
            {
                delete pChild;
                delete pTable;
                ErrorBoxE(e);
                return;
            }
            delete pChild;
            delete pTable;
            break;
        }
        if (sRef == 0)
        {
            QUICK_THROW("Cannot find Reference for file");
        }
    }
    CInheritTable::CNode *pThis = pTable->findOrMake(pItr->VGetFullPath() + iAttribL);
    CInheritTable::CNode *pReff = pTable->findOrMake(sRef);
    pThis->setParent(pReff);

    free(sRef);
}

CMakeLuaInheritTree::CMakeLuaInheritTree() { pTable = 0; }

void CMakeLuaInheritTree::Do(const char *sAttrib)
{
    iAttribL = strlen(sAttrib);

    IDirectoryTraverser::IIterator *pItr = 0;

    {
        auto itrResult = TheConstruct->GetFileService().Iterate(AsciiTowxString(sAttrib));
        if (!itrResult)
        {
            throw CModStudioException(__FILE__, __LINE__, "Cannot iterate attrib");
        }
        pItr = itrResult.value().release();
    }

    try
    {
        Rainman_ForEach(pItr, _ForEach, (void *)this, true);
    }
    catch (const CRainmanException &e)
    {
        delete pItr;
        throw CModStudioException(e, __FILE__, __LINE__, "Error processing luas");
    }

    pTable->assignOrphansTo(pTable->getRoot());
    delete pItr;
}

void CMakeLuaInheritTree::DoAction()
{
    pTable = new CInheritTable;

    const char *sAttrib = _DoesExist("attrib\\") ? "attrib\\attrib\\" : "data\\attrib\\";
    try
    {
        Do(sAttrib);
    }
    catch (const CRainmanException &e)
    {
        ErrorBoxE(e);
        delete pTable;
        return;
    }

    delete pTable;
}

wxString CRedButtonTool::GetName() { return AppStr(redbutton_toolname); }
wxString CRedButtonTool::GetHelpString() { return wxT(""); }
wxString CRedButtonTool::GetBitmapName() { return wxT("IDB_REDBUTTON"); }
void CRedButtonTool::DoAction()
{
    CFileSystemStore oFSO;
    oFSO.VInit();
    try
    {
        std::unique_ptr<IFileStore::IOutputStream> guard(
            oFSO.VOpenOutputStream("I:\\j\\k\\l\\m\\n\\o\\p\\q.test", true));
    }
    catch (const CRainmanException &e)
    {
        ErrorBoxE(e);
    }
    /*
    CIniConfigFile oConfig;
    oConfig.loadFile("E:\\Company of Heroes\\RelicCOH.module");
    oConfig.saveFile("E:\\Company of Heroes\\RelicCOH_.module");
    */

    /*
    wxString sOutFile = wxFileSelector(wxT("Select strings list"), wxT(""), wxT(""), wxT("txt"), wxT("Text File
    (*.txt)|*.txt") , wxFD_OPEN, TheConstruct);

    if(sOutFile.Len() == 0) return;

    char* sOut = wxStringToAscii(sOutFile);

    TheConstruct->GetRgdHashTable()->XRefWithStringList(sOut);

    delete[] sOut;
    */
    /*
    char* sOut = wxStringToAscii(oF.GetBaseFolder() + oF.GetFile());
    IFileStore::IStream* pS = TheConstruct->GetModule()->VOpenStream(sOut);
    delete[] sOut;
    RgdDump::CRgd oRgd;

    oRgd.loadFromStream(pS);
    delete pS;
    */
}

CRefreshFilesTool::~CRefreshFilesTool() { HideLoading(); }

wxString CRefreshFilesTool::GetName() { return AppStr(refreshfiles_name); }
wxString CRefreshFilesTool::GetHelpString() { return wxT(""); }
wxString CRefreshFilesTool::GetBitmapName() { return wxT("IDB_REFRESH"); }
void CRefreshFilesTool::DoAction()
{
    CModuleFile *pMod = TheConstruct->GetModuleService().GetModule();

    if (!m_pPresenter)
    {
        m_pPresenter = std::make_unique<CRefreshFilesPresenter>(static_cast<IRefreshFilesView &>(*this), TheConstruct);
    }

    if (!m_pPresenter->Refresh(pMod))
    {
        ThemeColours::ShowMessageBox(wxT("A refresh operation is already running."), AppStr(refreshfiles_name),
                                     wxICON_WARNING, TheConstruct);
    }
}

// IRefreshFilesView implementation

void CRefreshFilesTool::ShowLoading(const wxString &sMessage)
{
    m_pLoadingForm = new frmLoading(AppStr(mod_loading));
    m_pLoadingForm->SetMessage(sMessage);
    m_pLoadingForm->Show(true);
}

void CRefreshFilesTool::HideLoading()
{
    if (m_pLoadingForm)
    {
        m_pLoadingForm->Close(true);
        delete m_pLoadingForm;
        m_pLoadingForm = nullptr;
    }
}

void CRefreshFilesTool::UpdateProgress(const wxString &sMessage)
{
    if (m_pLoadingForm)
    {
        m_pLoadingForm->SetMessage(sMessage);
    }
}

void CRefreshFilesTool::RefreshFileTree()
{
    CModuleFile *pMod = TheConstruct->GetModuleService().GetModule();
    TheConstruct->GetFilesList()->FillFromIDirectoryTraverser(pMod);
}

void CRefreshFilesTool::ShowError(const wxString &sMessage)
{
    ThemeColours::ShowMessageBox(sMessage, AppStr(refreshfiles_name), wxICON_ERROR, TheConstruct);
}
