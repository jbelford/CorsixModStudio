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

#include "Construct.h"
#include "frmLoading.h"
#include "frmWelcome.h"
#include "frmNewMod.h"
#include "frmFiles.h"
#include "frmModule.h"
#include "frmCredits.h"
#include "frmLocaleSelector.h"
#include "frmRgdEditor.h"
#include "strconv.h"
#include <memory>
#include <rainman/io/CFileSystemStore.h>
#include <rainman/module/CFileMap.h>
#include "Tools.h"
extern "C"
{
#include "rainman/util/md5.h"
}
#include <wx/filedlg.h>
#include "Utility.h"
#include "strings.h"
#include "config.h"
#include "strconv.h"
// For ShellExecute :(
#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shobjidl.h>
#ifdef _DEBUG
#include <crtdbg.h>
#endif
#include "Common.h"

BEGIN_EVENT_TABLE(ConstructFrame, wxFrame)
EVT_MENU(IDM_LoadModDoWWA, ConstructFrame::OnOpenModDoW)
EVT_MENU(IDM_LoadModDC, ConstructFrame::OnOpenModDC)
EVT_MENU(IDM_LoadModSS, ConstructFrame::OnOpenModSS)
EVT_MENU(IDM_LoadModCoH, ConstructFrame::OnOpenModCoH)
EVT_MENU(IDM_LoadSga, ConstructFrame::OnOpenSga)
EVT_MENU(wxID_EXIT, ConstructFrame::OnQuit)
EVT_MENU(wxID_CLOSE, ConstructFrame::OnCloseMod)

EVT_MENU(IDM_AttributeEditor, ConstructFrame::LaunchRelicAttributeEditor)
EVT_MENU(IDM_AudioEditor, ConstructFrame::LaunchRelicAudioEditor)
EVT_MENU(IDM_ChunkyViewer, ConstructFrame::LaunchRelicChunkyViewer)
EVT_MENU(IDM_FXTool, ConstructFrame::LaunchRelicFXTool)
EVT_MENU(IDM_MissionEditor, ConstructFrame::LaunchRelicMisionEditor)
EVT_MENU(IDM_ModPackager, ConstructFrame::LaunchRelicModPackager)
EVT_MENU(IDM_ObjectEditor, ConstructFrame::LaunchRelicObjectEditor)
EVT_MENU(IDM_PlayCOH, ConstructFrame::LaunchCOH)
EVT_MENU(IDM_PlayW40k, ConstructFrame::LaunchW40k)
EVT_MENU(IDM_PlayWXP, ConstructFrame::LaunchW40kWA)
EVT_MENU(IDM_PlayDC, ConstructFrame::LaunchDC)
EVT_MENU(IDM_PlaySS, ConstructFrame::LaunchSS)
EVT_MENU(IDM_PlayWarn, ConstructFrame::LaunchWarnings)
EVT_MENU(wxID_HELP_CONTENTS, ConstructFrame::LaunchHelp)
EVT_MENU(IDM_Credits, ConstructFrame::LaunchCredits)
EVT_MENU(IDM_HideDonate, ConstructFrame::HideDonate)

EVT_MENU(IDM_ForumDoW, ConstructFrame::LaunchForumDoW)
EVT_MENU(IDM_ForumCoH, ConstructFrame::LaunchForumCoH)
EVT_MENU(IDM_RDNWikiNew, ConstructFrame::LaunchNewRDNWiki)

EVT_MENU(IDM_LuaRef, ConstructFrame::LaunchLuaRef)
EVT_MENU(IDM_RDNWiki, ConstructFrame::LaunchRDNWiki)
EVT_MENU(IDM_KresjahWiki, ConstructFrame::LaunchKresjahWiki)
EVT_SPLITTER_SASH_POS_CHANGED(IDC_Splitter, ConstructFrame::OnSashMove)

EVT_CLOSE(ConstructFrame::OnCloseWindow)
END_EVENT_TABLE()

frmFiles *ConstructFrame::GetFilesList()
{
    if (!m_pSplitter->IsSplit())
        return 0;
    return (frmFiles *)m_pSplitter->GetWindow1();
}

void ConstructFrame::OnSashMove(wxSplitterEvent &event)
{
    TheConfig->Write(AppStr(config_sashposition), event.GetSashPosition());
}

void ConstructFrame::OnOpenSga(wxCommandEvent &event)
{
    UNUSED(event);
    DoLoadSga();
}

void ConstructFrame::LaunchTool(wxString sExeName, bool bIsInModTools)
{
    wxString sCommand, sFolder;
    sFolder = m_sModuleFile.BeforeLast('\\');
    if (bIsInModTools)
        sFolder.Append(wxT("\\ModTools"));
    sFolder.Append(wxT("\\"));
    sCommand = sFolder;
    sCommand.Append(sExeName);

    ShellExecute((HWND)TheConstruct->GetHandle(), wxT("open"), sCommand, wxT(""), sFolder, 3);
}

CRgdHashTable *ConstructFrame::GetRgdHashTable()
{
    if (m_pRgdHashTable)
        return m_pRgdHashTable;

    auto result = m_hashService.Initialize(AppStr(app_dictionariespath));
    if (!result.ok())
    {
        throw new CModStudioException(__FILE__, __LINE__, wxStringToAscii(result.error()));
    }

    m_pRgdHashTable = m_hashService.GetHashTable();
    if (m_hashService.GetCustomOutPath())
        m_sRgdHashCustomOut = strdup(m_hashService.GetCustomOutPath());

    return m_pRgdHashTable;
}

void ConstructFrame::LaunchCredits(wxCommandEvent &event)
{
    UNUSED(event);
    frmCredits oCredits;
    oCredits.ShowModal();
}

void ConstructFrame::LaunchURL(wxString sURL)
{
    ShellExecute((HWND)TheConstruct->GetHandle(), wxT("open"), sURL, 0, 0, 3);
}

void ConstructFrame::LaunchHelp(wxCommandEvent &event)
{
    UNUSED(event);
    LaunchURL(AppStr(app_helpurl));
}

void ConstructFrame::StaticLaunchHelp() { LaunchURL(AppStr(app_helpurl)); }

void ConstructFrame::LaunchDonate(wxCommandEvent &event)
{
    UNUSED(event);
    LaunchURL(wxT("https://www.paypal.com/cgi-bin/"
                  "webscr?cmd=_xclick&business=corsix%40gmail%2ecom&item_name=Donation%20toward%20Corsix%27s%%20Mod%"
                  "20Studio&no_shipping=1&return=http%3a%2f%2fwww%2ecorsix%2eorg%2fgo%2ephp%3fdonate_thankyou&cancel_"
                  "return=http%3a%2f%2fwww%2ecorsix%2eorg%2f&no_note=1&cn=Optional%20Message&tax=0&currency_code=GBP&"
                  "bn=PP%2dDonationsBF&charset=UTF%2d8"));
}

void ConstructFrame::LaunchLuaRef(wxCommandEvent &event)
{
    UNUSED(event);
    LaunchURL(wxT("http://dowmodstudio.gamemod.net/luaref/index.php"));
}

void ConstructFrame::LaunchForumDoW(wxCommandEvent &event)
{
    UNUSED(event);
    LaunchURL(wxT("http://forums.relicnews.com/forumdisplay.php?f=96"));
}

void ConstructFrame::LaunchForumCoH(wxCommandEvent &event)
{
    UNUSED(event);
    LaunchURL(wxT("http://forums.relicnews.com/forumdisplay.php?f=189"));
}

void ConstructFrame::LaunchNewRDNWiki(wxCommandEvent &event)
{
    UNUSED(event);
    LaunchURL(wxT("http://wiki.relicrank.com/"));
}

void ConstructFrame::LaunchKresjahWiki(wxCommandEvent &event)
{
    UNUSED(event);
    LaunchURL(wxT("http://astartes.vorbereitungstagung.de/ModdingDocs/doku.php?id=wiki:start"));
}

void ConstructFrame::LaunchRDNWiki(wxCommandEvent &event)
{
    UNUSED(event);
    LaunchURL(wxT("http://www.relic.com/rdn/wiki/RDNWikiHome"));
}

void ConstructFrame::LaunchRelicAttributeEditor(wxCommandEvent &event)
{
    UNUSED(event);
    LaunchTool(wxT("AttributeEditor.exe"));
}

void ConstructFrame::LaunchRelicAudioEditor(wxCommandEvent &event)
{
    UNUSED(event);
    LaunchTool(wxT("AudioEditor.exe"));
}

void ConstructFrame::LaunchRelicChunkyViewer(wxCommandEvent &event)
{
    UNUSED(event);
    LaunchTool(wxT("ChunkyViewer.exe"));
}

void ConstructFrame::LaunchRelicFXTool(wxCommandEvent &event)
{
    UNUSED(event);
    LaunchTool(wxT("FxTool.exe"));
}

void ConstructFrame::LaunchRelicMisionEditor(wxCommandEvent &event)
{
    UNUSED(event);
    LaunchTool(wxT("W40kME.exe"), false);
}

void ConstructFrame::LaunchRelicModPackager(wxCommandEvent &event)
{
    UNUSED(event);
    LaunchTool(wxT("ModPackager.exe"));
}

void ConstructFrame::LaunchRelicObjectEditor(wxCommandEvent &event)
{
    UNUSED(event);
    LaunchTool(wxT("ObjectEditor.exe"));
}

wxAuiNotebook *ConstructFrame::GetTabs() { return m_pTabs; }

void ConstructFrame::LaunchW40k(wxCommandEvent &event)
{
    UNUSED(event);
    wxString sCommand, sFolder, sCmdLine;
    try
    {
        sFolder = ConfGetDoWFolder();
    }
    catch (CRainmanException *pE)
    {
        ErrorBoxE(pE);
        return;
    }
    sCommand = sFolder;
    sCommand.Append(wxT("W40k.exe"));
    sCmdLine = wxT("");
    if (m_pModule)
    {
        sCmdLine = m_sModuleFile;
        sCmdLine = sCmdLine.BeforeLast('.');
        sCmdLine = sCmdLine.AfterLast('\\');
        sCmdLine.Prepend(wxT("-modname "));
    }

    if (GetMenuBar()->GetMenu(3)->FindItem(IDM_PlayDev)->IsChecked())
        sCmdLine.Append(wxT(" -dev"));
    if (GetMenuBar()->GetMenu(3)->FindItem(IDM_PlayNoMovies)->IsChecked())
        sCmdLine.Append(wxT(" -nomovies"));

    ShellExecute((HWND)TheConstruct->GetHandle(), wxT("open"), sCommand, sCmdLine, sFolder, 3);
}

void ConstructFrame::LaunchDC(wxCommandEvent &event)
{
    UNUSED(event);
    wxString sCommand, sFolder, sCmdLine;
    try
    {
        sFolder = ConfGetDCFolder();
    }
    catch (CRainmanException *pE)
    {
        ErrorBoxE(pE);
        return;
    }
    sCommand = sFolder;
    sCommand.Append(wxT("DarkCrusade.exe"));
    sCmdLine = wxT("");
    if (m_pModule)
    {
        sCmdLine = m_sModuleFile;
        sCmdLine = sCmdLine.BeforeLast('.');
        sCmdLine = sCmdLine.AfterLast('\\');
        sCmdLine.Prepend(wxT("-modname "));
    }

    if (GetMenuBar()->GetMenu(3)->FindItem(IDM_PlayDev)->IsChecked())
        sCmdLine.Append(wxT(" -dev"));
    if (GetMenuBar()->GetMenu(3)->FindItem(IDM_PlayNoMovies)->IsChecked())
        sCmdLine.Append(wxT(" -nomovies"));

    ShellExecute((HWND)TheConstruct->GetHandle(), wxT("open"), sCommand, sCmdLine, sFolder, 3);
}

void ConstructFrame::LaunchSS(wxCommandEvent &event)
{
    UNUSED(event);
    wxString sCommand, sFolder, sCmdLine;
    try
    {
        sFolder = ConfGetDCFolder();
    }
    catch (CRainmanException *pE)
    {
        ErrorBoxE(pE);
        return;
    }
    sCommand = sFolder;
    sCommand.Append(wxT("Soulstorm.exe"));
    sCmdLine = wxT("");
    if (m_pModule)
    {
        sCmdLine = m_sModuleFile;
        sCmdLine = sCmdLine.BeforeLast('.');
        sCmdLine = sCmdLine.AfterLast('\\');
        sCmdLine.Prepend(wxT("-modname "));
    }

    if (GetMenuBar()->GetMenu(3)->FindItem(IDM_PlayDev)->IsChecked())
        sCmdLine.Append(wxT(" -dev"));
    if (GetMenuBar()->GetMenu(3)->FindItem(IDM_PlayNoMovies)->IsChecked())
        sCmdLine.Append(wxT(" -nomovies"));

    ShellExecute((HWND)TheConstruct->GetHandle(), wxT("open"), sCommand, sCmdLine, sFolder, 3);
}

void ConstructFrame::LaunchCOH(wxCommandEvent &event)
{
    UNUSED(event);
    wxString sCommand, sFolder, sCmdLine;
    try
    {
        sFolder = ConfGetCoHFolder();
    }
    catch (CRainmanException *pE)
    {
        ErrorBoxE(pE);
        return;
    }
    sCommand = sFolder;
    if (sCommand.Last() != wxChar('\\'))
        sCommand.Append(wxT("\\"));
    sCommand.Append(wxT("RelicCOH.exe"));
    sCmdLine = wxT("");
    if (m_pModule)
    {
        sCmdLine = AsciiTowxString(m_pModule->GetName());
        sCmdLine.Prepend(wxT("-modname "));
    }

    if (GetMenuBar()->GetMenu(3)->FindItem(IDM_PlayDev)->IsChecked())
        sCmdLine.Append(wxT(" -dev"));
    if (GetMenuBar()->GetMenu(3)->FindItem(IDM_PlayNoMovies)->IsChecked())
        sCmdLine.Append(wxT(" -nomovies"));

    ShellExecute((HWND)TheConstruct->GetHandle(), wxT("open"), sCommand, sCmdLine, sFolder, 3);
}

void ConstructFrame::LaunchW40kWA(wxCommandEvent &event)
{
    UNUSED(event);
    wxString sCommand, sFolder, sCmdLine;
    try
    {
        sFolder = ConfGetDoWFolder();
    }
    catch (CRainmanException *pE)
    {
        ErrorBoxE(pE);
        return;
    }
    sCommand = sFolder;
    sCommand.Append(wxT("W40kWA.exe"));
    sCmdLine = wxT("");
    if (m_pModule)
    {
        sCmdLine = m_sModuleFile;
        sCmdLine = sCmdLine.BeforeLast('.');
        sCmdLine = sCmdLine.AfterLast('\\');
        sCmdLine.Prepend(wxT("-modname "));
    }

    if (GetMenuBar()->GetMenu(3)->FindItem(IDM_PlayDev)->IsChecked())
        sCmdLine.Append(wxT(" -dev"));
    if (GetMenuBar()->GetMenu(3)->FindItem(IDM_PlayNoMovies)->IsChecked())
        sCmdLine.Append(wxT(" -nomovies"));

    ShellExecute((HWND)TheConstruct->GetHandle(), wxT("open"), sCommand, sCmdLine, sFolder, 3);
}

void ConstructFrame::LaunchWarnings(wxCommandEvent &event)
{
    UNUSED(event);
    wxString sCommand, sFolder;
    if (GetModule() && GetModuleService().GetModuleType() != CModuleFile::MT_DawnOfWar)
    {
        wchar_t sDocumentsFolder[MAX_PATH];
        SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, sDocumentsFolder);
        sCommand = wxT("\"");
        sCommand.Append(sDocumentsFolder);
        sCommand.Append(wxT("\\My Games\\Company of Heroes\\warnings.log\""));
    }
    else
    {
        try
        {
            sFolder = ConfGetDoWFolder();
        }
        catch (CRainmanException *pE)
        {
            ErrorBoxE(pE);
            return;
        }
        sCommand = wxT("\"");
        sCommand.Append(sFolder);
        sCommand.Append(wxT("warnings.log\""));
    }

    wchar_t sWinFolder[MAX_PATH];
    SHGetFolderPath(NULL, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, sWinFolder);
    wxString sNotepad = sWinFolder;
    sNotepad.Append(wxT("\\notepad.exe"));

    ShellExecute((HWND)TheConstruct->GetHandle(), wxT("open"), sNotepad, sCommand, sFolder, 3);
}

ConstructFrame::ConstructFrame(const wxString &sTitle, const wxPoint &oPos, const wxSize &oSize)
    : wxFrame((wxFrame *)NULL, -1, sTitle, oPos, oSize, wxDEFAULT_FRAME_STYLE | wxMAXIMIZE)
{
    m_pSplitter = 0;
    m_pTabs = 0;
    m_pModule = 0;
    m_sModuleFile = wxT("");
    m_pRgdHashTable = 0;
    m_sRgdHashCustomOut = 0;

    // Initiate tools
    m_toolRegistry.Register(new CLocaleTool);
    m_toolRegistry.Register(new CUcsTool);
    m_toolRegistry.Register(new CAttribSnapshotTool);
    m_toolRegistry.Register(new CSgaPackerTool);
    m_toolRegistry.Register(new CExtractAllTool);
    m_toolRegistry.Register(new CDpsCalculatorTool);
    m_toolRegistry.Register(new CRefreshFilesTool);
    m_toolRegistry.Register(new CMakeLuaInheritTree);
    m_toolRegistry.Register(new CAESetupTool);
    m_toolRegistry.Register(new CRedButtonTool);

    // Make Splitter Window
    m_pSplitter = new wxSplitterWindow(this, IDC_Splitter);
    m_pSplitter->SetSashGravity(0.5);
    m_pSplitter->SetMinimumPaneSize(48);

    // Make Tabs Window
    m_pTabs = new wxAuiNotebook(m_pSplitter, -1, wxPoint(0, 0), wxDefaultSize,
                                wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxAUI_NB_WINDOWLIST_BUTTON |
                                    wxNO_BORDER | wxAUI_NB_CLOSE_BUTTON);
    m_pSplitter->Initialize(m_pTabs);
    m_pTabs->AddPage(new frmWelcome(m_pTabs, -1), AppStr(welcome_tabname));

    // Build menus
    m_menuController.Build(this, m_toolRegistry);

    // Make Statusbar
    CreateStatusBar();
    SetStatusText(AppStr(statusbar_message_default));
}

void ConstructFrame::DoNewMod()
{
    frmNewMod oNewMod;
    if (oNewMod.ShowModal() == wxID_NEW)
    {
        DoLoadMod(oNewMod.GetPath());
    }
}
void ConstructFrame::OnToolMenuCommand(wxCommandEvent &event)
{
    size_t index = static_cast<size_t>(event.GetId() - IDM_ModToolBase);
    m_toolRegistry.LaunchTool(index);
}

size_t ConstructFrame::GetToolCount() { return m_toolRegistry.GetCount(); }

ConstructFrame::ITool *ConstructFrame::GetTool(size_t i) { return m_toolRegistry.Get(i); }

void ConstructFrame::DoTool(wxString sName) { m_toolRegistry.DoTool(sName); }

ConstructFrame::~ConstructFrame()
{
    frmRGDEditor::FreeNodeHelp();
    if (m_pModule)
        delete m_pModule;
    // HashService owns the hash table lifecycle (save + delete)
    m_hashService.Shutdown();
    m_pRgdHashTable = nullptr;
    if (m_sRgdHashCustomOut)
        free(m_sRgdHashCustomOut);
    // ToolRegistry destructor handles tool cleanup
}

CModuleFile *ConstructFrame::GetModule() const { return m_pModule; }

const wxString &ConstructFrame::GetModuleFile() const { return m_sModuleFile; }

void ConstructFrame::OnCloseMod(wxCommandEvent &event) { SetModule(0); }

void ConstructFrame::SetModule(CModuleFile *pMod)
{
    if (m_pModule)
        delete m_pModule;
    m_pModule = pMod;
    m_moduleService.SetModule(pMod);
    if (pMod)
    {
        // CModuleFile implements both IFileStore and IDirectoryTraverser
        m_fileService.SetStore(pMod);
        m_fileService.SetTraverser(pMod);
    }
    else
    {
        m_fileService.SetStore(nullptr);
        m_fileService.SetTraverser(nullptr);
    }
    if (pMod != 0)
    {
        if (!m_pSplitter->IsSplit())
        {
            GetMenuBar()->GetMenu(0)->Enable(wxID_CLOSE, true);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModDoWWA, false);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModDC, false);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModSS, false);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModCoH, false);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadSga, false);
            GetMenuBar()->GetMenu(0)->Enable(wxID_NEW, false);
            GetMenuBar()->EnableTop(1, true);
            frmFiles *pFiles = new frmFiles(m_pSplitter, -1);
            m_pSplitter->SplitVertically(pFiles, m_pTabs);
            m_pSplitter->SetSashPosition(TheConfig->Read(AppStr(config_sashposition), 189));
            pFiles->FillFromIDirectoryTraverser(pMod);
            m_pTabs->AddPage(
                new frmModule(m_pTabs, -1),
                AppStr(mod_tabname).Append(wxString(wxT(" ["))).Append(m_sModuleFile.AfterLast('\\')).Append(wxT("]")),
                true);
            while (m_pTabs->GetPageCount() > 1)
                m_pTabs->DeletePage(0);

            GetMenuBar()->GetMenu(3)->Enable(IDM_PlayCOH, false);
            GetMenuBar()->GetMenu(3)->Enable(IDM_PlayW40k, false);
            GetMenuBar()->GetMenu(3)->Enable(IDM_PlayWXP, false);
            GetMenuBar()->GetMenu(3)->Enable(IDM_PlayDC, false);
            GetMenuBar()->GetMenu(3)->Enable(IDM_PlaySS, false);

            switch (pMod->GetModuleType())
            {
            case CModuleFile::MT_DawnOfWar:
                GetMenuBar()->GetMenu(3)->Enable(IDM_PlayW40k, true);
                GetMenuBar()->GetMenu(3)->Enable(IDM_PlayWXP, true);
                GetMenuBar()->GetMenu(3)->Enable(IDM_PlayDC, true);
                GetMenuBar()->GetMenu(3)->Enable(IDM_PlaySS, true);
                break;

            case CModuleFile::MT_CompanyOfHeroes:
            case CModuleFile::MT_CompanyOfHeroesEarly:
                GetMenuBar()->GetMenu(3)->Enable(IDM_PlayCOH, true);
                break;
            }
        }
    }
    else
    {
        GetMenuBar()->GetMenu(3)->Enable(IDM_PlayCOH, false);
        GetMenuBar()->GetMenu(3)->Enable(IDM_PlayW40k, false);
        GetMenuBar()->GetMenu(3)->Enable(IDM_PlayWXP, false);
        GetMenuBar()->GetMenu(3)->Enable(IDM_PlayDC, false);
        GetMenuBar()->GetMenu(3)->Enable(IDM_PlaySS, false);

        m_sModuleFile = wxT("");
        if (m_pSplitter->IsSplit())
        {
            GetMenuBar()->GetMenu(0)->Enable(wxID_CLOSE, false);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModDoWWA, true);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModDC, true);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModSS, true);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModCoH, true);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadSga, true);
            GetMenuBar()->GetMenu(0)->Enable(wxID_NEW, true);
            GetMenuBar()->EnableTop(1, false);

            m_pTabs->AddPage(new frmWelcome(m_pTabs, -1), AppStr(welcome_tabname), true);
            while (m_pTabs->GetPageCount() > 1)
                m_pTabs->DeletePage(0);

            frmFiles *pFiles = GetFilesList();
            m_pSplitter->Unsplit(pFiles);
            delete pFiles;
        }
    }
}

void __cdecl ConstructFrame::LoadModCallback(const char *sMsg, void *pTag)
{
    UNUSED(pTag);
    TheConstruct->GetLoadingForm()->SetMessage(AsciiTowxString(sMsg));
    wxSafeYield(TheConstruct->GetLoadingForm());
}

void ConstructFrame::SetLoadingForm(frmLoading *pLoading) { m_pLoadingForm = pLoading; }

frmLoading *ConstructFrame::GetLoadingForm() { return m_pLoadingForm; }

void ConstructFrame::OnOpenModDoW(wxCommandEvent &event)
{
    UNUSED(event);
    DoLoadMod(wxT(""), LM_DoW_WA);
}

void ConstructFrame::OnOpenModDC(wxCommandEvent &event)
{
    UNUSED(event);
    DoLoadMod(wxT(""), LM_DC);
}

void ConstructFrame::OnOpenModSS(wxCommandEvent &event)
{
    UNUSED(event);
    DoLoadMod(wxT(""), LM_SS);
}

void ConstructFrame::OnOpenModCoH(wxCommandEvent &event)
{
    UNUSED(event);
    DoLoadMod(wxT(""), LM_CoH_OF);
}

void ConstructFrame::HideDonate(wxCommandEvent &event)
{
    UNUSED(event);
    ::wxMessageBox(AppStr(hidedonate_text), AppStr(hidedonate_menu), wxICON_INFORMATION, wxTheApp->GetTopWindow());
    TheConfig->Write(AppStr(config_donate), (int)0);
}

IFileStore::IOutputStream *ConstructFrame::SaveFileCallback(const char *sFile, bool bEraseIfPresent, void *pTag)
{
    ConstructFrame *pConstruct = (ConstructFrame *)pTag;

    const char *sFilenamePart = strrchr(sFile, '\\') + 1;
    const char *sFileextPart = strrchr(sFilenamePart, '.') + 1;

    wxString sSavename =
        ::wxFileSelector(wxT("Where do you want to save to?"), wxT(""), AsciiTowxString(sFilenamePart),
                         AsciiTowxString(sFileextPart), wxT("*.*"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT, pConstruct);

    if (sSavename.IsEmpty())
        return 0;

    return pConstruct->GetModuleService().GetModule()->GetFileSystem()->OpenOutputStreamW(sSavename, bEraseIfPresent);
}

void ConstructFrame::DoLoadSga()
{
    std::unique_ptr<wxFileDialog> pFileDialog(
        new wxFileDialog(this, AppStr(mod_select_sga), wxT(""), wxT(""), AppStr(mod_sga_filter), wxFD_OPEN));

    if (pFileDialog->ShowModal() == wxID_OK)
    {
        m_pLoadingForm = new frmLoading(AppStr(mod_loading));
        m_pLoadingForm->Show(true);
        m_pLoadingForm->SetMessage(wxString(wxT("Initializing")));
        wxSafeYield(m_pLoadingForm);

        auto result = ModuleService::LoadSgaAsMod(pFileDialog->GetPath(), LoadModCallback);
        if (!result.ok())
        {
            ErrorBoxS(result.error());
            m_pLoadingForm->Close(true);
            delete m_pLoadingForm;
            m_pLoadingForm = 0;
            return;
        }
        CModuleFile *pMod = result.value();
        pMod->GetFileMap()->SetAuxOutputSupply(SaveFileCallback, (void *)this);

        m_pLoadingForm->SetMessage(wxString(wxT("Initializing GUI")));
        wxSafeYield(m_pLoadingForm);
        m_sModuleFile = pFileDialog->GetPath();
        SetModule(pMod);

        GetMenuBar()->EnableTop(3, false);

        m_pLoadingForm->Close(true);
        delete m_pLoadingForm;
        m_pLoadingForm = 0;
        Refresh();
    }
}

void ConstructFrame::DoLoadMod(wxString sPath, eLoadModGames eGame)
{
    wxFileDialog *pFileDialog = 0;
    if (sPath == wxT(""))
    {
        wxString sDoWPath;
        try
        {
            if (eGame == LM_CoH_OF)
                sDoWPath = ConfGetCoHFolder();
            else if (eGame == LM_DC)
                sDoWPath = ConfGetDCFolder();
            else if (eGame == LM_SS)
                sDoWPath = ConfGetSSFolder();
            else if (eGame == LM_DoW_WA || eGame == LM_Any)
                sDoWPath = ConfGetDoWFolder();
        }
        catch (CRainmanException *pE)
        {
            ErrorBoxE(pE);
            return;
        }
        pFileDialog =
            new wxFileDialog(this, AppStr(mod_select_file), sDoWPath, wxT(""), AppStr(mod_file_filter), wxFD_OPEN);
        if (!pFileDialog)
        {
            return;
        }
    }
    if (sPath != wxT("") || pFileDialog->ShowModal() == wxID_OK)
    {
        if (sPath == wxT(""))
        {
            wxString sAppFolder = pFileDialog->GetPath();
            sAppFolder = sAppFolder.BeforeLast('\\');
            sAppFolder.Append(wxT("\\"));
            if (eGame == LM_CoH_OF)
                TheConfig->Write(AppStr(config_cohfolder), sAppFolder);
            else if (eGame == LM_DC)
                TheConfig->Write(AppStr(config_dcfolder), sAppFolder);
            else if (eGame == LM_SS)
                TheConfig->Write(AppStr(config_ssfolder), sAppFolder);
            else if (eGame == LM_DoW_WA)
                TheConfig->Write(AppStr(config_dowfolder), sAppFolder);
        }

        m_pLoadingForm = new frmLoading(AppStr(mod_loading));
        m_pLoadingForm->Show(true);
        m_pLoadingForm->SetMessage(wxString(wxT("Initializing")));
        wxSafeYield(m_pLoadingForm);

        char *sFile = UnicodeToAscii(pFileDialog ? pFileDialog->GetPath() : sPath);
        if (!sFile)
        {
            ErrorBox("Memory allocation error");
            delete pFileDialog;
            m_pLoadingForm->Close(true);
            delete m_pLoadingForm;
            m_pLoadingForm = 0;
            return;
        }

        CModuleFile *pMod = new CModuleFile;
        if (!pMod)
        {
            ErrorBox("Memory allocation error");
            delete[] sFile;
            delete pFileDialog;
            m_pLoadingForm->Close(true);
            delete m_pLoadingForm;
            m_pLoadingForm = 0;
            return;
        }

        MD5Context md5ModName;
        MD5InitKey(&md5ModName, sFile);
        unsigned char sModConfigKey[17];
        wchar_t sModConfigKeyHex[33];
        sModConfigKey[16] = 0;
        sModConfigKeyHex[32] = 0;
        MD5Final(sModConfigKey, &md5ModName);
        for (int i = 0; i < 16; ++i)
        {
            sModConfigKeyHex[i * 2] = "0123456789ABCDEF"[sModConfigKey[i] >> 4];
            sModConfigKeyHex[(i * 2) + 1] = "0123456789ABCDEF"[sModConfigKey[i] & 15];
        }
        TheConfig->SetPath(wxString(wxT("/")).Append(sModConfigKeyHex));

        bool bSkipLocale;
        TheConfig->Read(AppStr(config_mod_localeremember), &bSkipLocale, false);
        if (!bSkipLocale)
        {
            frmLocaleSelector *pLocaleSelect = new frmLocaleSelector(AppStr(localeselect_title), eGame);
            pLocaleSelect->ShowModal();
            delete pLocaleSelect;
        }

        char *sLocale = wxStringToAscii(TheConfig->Read(AppStr(config_mod_locale), AppStr(localeselect_default)));
        {
            ModuleService tmpSvc(pMod);
            auto locResult = tmpSvc.SetLocale(wxString::FromUTF8(sLocale));
            delete[] sLocale;
            if (!locResult.ok())
            {
                ErrorBoxS(locResult.error());
                delete pMod;
                delete[] sFile;
                delete pFileDialog;
                m_pLoadingForm->Close(true);
                delete m_pLoadingForm;
                m_pLoadingForm = 0;
                return;
            }
        }

        try
        {
            pMod->LoadModuleFile(sFile, LoadModCallback);

            if (pMod->GetModuleType() == CModuleFile::MT_CompanyOfHeroes)
            {
                wchar_t sMapPackDir[MAX_PATH];
                SHGetFolderPathW(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, sMapPackDir);
                wcscat(sMapPackDir, L"\\My Games\\Company of Heroes");
                pMod->SetMapPackRootFolder(sMapPackDir);
            }

            pMod->ReloadResources(CModuleFile::RR_All, CModuleFile::RR_All, CModuleFile::RR_All, LoadModCallback);
        }
        catch (CRainmanException *pE)
        {
            ErrorBoxE(pE);
            delete pMod;
            delete[] sFile;
            delete pFileDialog;
            m_pLoadingForm->Close(true);
            delete m_pLoadingForm;
            m_pLoadingForm = 0;
            return;
        }
        m_pLoadingForm->SetMessage(wxString(wxT("Initializing GUI")));
        wxSafeYield(m_pLoadingForm);
        m_sModuleFile = pFileDialog ? pFileDialog->GetPath() : sPath;
        SetModule(pMod);

        delete[] sFile;
        m_pLoadingForm->Close(true);
        delete m_pLoadingForm;
        m_pLoadingForm = 0;
        Refresh();
    }
    delete pFileDialog;
}

void ConstructFrame::OnQuit(wxCommandEvent &event)
{
    UNUSED(event);
    Close(true);
}

void ConstructFrame::OnCloseWindow(wxCloseEvent &event)
{
    for (size_t i = 0; i < m_pTabs->GetPageCount(); ++i)
    {
        wxCloseEvent e2(wxEVT_CLOSE_WINDOW);
        e2.SetCanVeto(event.CanVeto());
        m_pTabs->GetPage(i)->GetEventHandler()->ProcessEvent(e2);
        if (e2.GetVeto())
        {
            event.Veto();
            return;
        }
    }
    this->Destroy();
}
