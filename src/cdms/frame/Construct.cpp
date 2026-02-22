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
#include "views/interfaces/ISaveable.h"
#include "views/frmLoading.h"
#include "views/frmWelcome.h"
#include "views/frmNewMod.h"
#include "views/frmFiles.h"
#include "views/frmModule.h"
#include "views/frmCredits.h"
#include "views/frmLocaleSelector.h"
#include "views/frmRgdEditor.h"
#include "common/strconv.h"
#include <memory>
#include <rainman/io/CFileSystemStore.h>
#include <rainman/module/CFileMap.h>
#include "tools/Tools.h"
extern "C"
{
#include "rainman/util/md5.h"
}
#include <wx/filedlg.h>
#include "common/Utility.h"
#include "common/strings.h"
#include "common/config.h"
#include "common/strconv.h"
#include "common/ThemeColours.h"
// For ShellExecute :(
#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shobjidl.h>
#ifdef _DEBUG
#include <crtdbg.h>
#endif
#include "common/Common.h"
#include <rainman/core/RainmanLog.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

BEGIN_EVENT_TABLE(ConstructFrame, wxFrame)
EVT_MENU(IDM_LoadModDoWWA, ConstructFrame::OnOpenModDoW)
EVT_MENU(IDM_LoadModDC, ConstructFrame::OnOpenModDC)
EVT_MENU(IDM_LoadModSS, ConstructFrame::OnOpenModSS)
EVT_MENU(IDM_LoadModDE, ConstructFrame::OnOpenModDE)
EVT_MENU(IDM_LoadModCoH, ConstructFrame::OnOpenModCoH)
EVT_MENU(IDM_LoadSga, ConstructFrame::OnOpenSga)
EVT_MENU(wxID_EXIT, ConstructFrame::OnQuit)
EVT_MENU(wxID_CLOSE, ConstructFrame::OnCloseMod)
EVT_MENU(wxID_PROPERTIES, ConstructFrame::OnModProperties)
EVT_MENU(IDM_SaveActive, ConstructFrame::OnSaveActive)
EVT_MENU(IDM_CloseActiveTab, ConstructFrame::OnCloseActiveTab)
EVT_MENU(IDM_NextTab, ConstructFrame::OnNextTab)

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
EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, ConstructFrame::OnTabClosing)
END_EVENT_TABLE()

frmFiles *ConstructFrame::GetFilesList() { return m_tabManager.GetFilesList(); }

void ConstructFrame::OnSashMove(wxSplitterEvent &event) { m_tabManager.OnSashMoved(event.GetSashPosition()); }

void ConstructFrame::OnTabClosing(wxAuiNotebookEvent &event)
{
    int idx = event.GetSelection();
    if (idx == wxNOT_FOUND)
    {
        event.Skip();
        return;
    }

    wxWindow *page = m_tabManager.GetTabs()->GetPage(idx);

    // If the page has unsaved changes, prompt the user
    auto *pSaveable = dynamic_cast<ISaveable *>(page);
    if (pSaveable && pSaveable->IsModified())
    {
        int ans = ThemeColours::ShowMessageBox(wxT("This file has unsaved changes. Save before closing?"),
                                               m_tabManager.GetTabs()->GetPageText(idx),
                                               wxYES_NO | wxCANCEL | wxICON_WARNING, this);

        switch (ans)
        {
        case wxYES:
            pSaveable->DoSave();
            break;
        case wxCANCEL:
            event.Veto();
            return;
        case wxNO:
        default:
            break;
        }
    }

    m_tabManager.OnPageClosed(page);
    event.Skip();
}

void ConstructFrame::OnSaveActive(wxCommandEvent &event)
{
    UNUSED(event);
    wxAuiNotebook *pTabs = m_tabManager.GetTabs();
    int sel = pTabs->GetSelection();
    if (sel == wxNOT_FOUND)
    {
        return;
    }

    wxWindow *page = pTabs->GetPage(sel);
    auto *pSaveable = dynamic_cast<ISaveable *>(page);
    if (pSaveable && pSaveable->IsModified())
    {
        pSaveable->DoSave();
    }
}

void ConstructFrame::OnCloseActiveTab(wxCommandEvent &event)
{
    UNUSED(event);
    wxAuiNotebook *pTabs = m_tabManager.GetTabs();
    int sel = pTabs->GetSelection();
    if (sel == wxNOT_FOUND)
    {
        return;
    }

    // Fire the same notebook page-close event so OnTabClosing handles
    // unsaved-changes prompts and cleanup.
    wxAuiNotebookEvent closeEvent(wxEVT_AUINOTEBOOK_PAGE_CLOSE, pTabs->GetId());
    closeEvent.SetSelection(sel);
    closeEvent.SetEventObject(pTabs);
    pTabs->GetEventHandler()->ProcessEvent(closeEvent);

    if (!closeEvent.IsAllowed())
    {
        return;
    }

    pTabs->DeletePage(sel);
}

void ConstructFrame::OnNextTab(wxCommandEvent &event)
{
    UNUSED(event);
    wxAuiNotebook *pTabs = m_tabManager.GetTabs();
    int count = static_cast<int>(pTabs->GetPageCount());
    if (count < 2)
    {
        return;
    }

    int sel = pTabs->GetSelection();
    pTabs->SetSelection((sel + 1) % count);
}

void ConstructFrame::OnOpenSga(wxCommandEvent &event)
{
    UNUSED(event);
    DoLoadSga();
}

void ConstructFrame::OnModProperties(wxCommandEvent &event)
{
    UNUSED(event);
    if (!m_moduleManager.HasModule())
    {
        return;
    }

    // If the frmModule tab is already open, select it
    wxAuiNotebook *pTabs = m_tabManager.GetTabs();
    for (size_t i = 0; i < pTabs->GetPageCount(); ++i)
    {
        if (dynamic_cast<frmModule *>(pTabs->GetPage(i)))
        {
            pTabs->SetSelection(i);
            return;
        }
    }

    // Otherwise create a new frmModule tab
    m_tabManager.AddPage(new frmModule(pTabs, -1),
                         AppStr(mod_tabname)
                             .Append(wxString(wxT(" [")))
                             .Append(m_moduleManager.GetModuleFile().AfterLast('\\'))
                             .Append(wxT("]")),
                         true);
}

void ConstructFrame::OnRelicToolCommand(wxCommandEvent &event)
{
    size_t index = static_cast<size_t>(event.GetId() - IDM_RelicToolBase);
    if (index >= m_relicToolResolver.GetToolCount())
    {
        return;
    }

    const auto &tool = m_relicToolResolver.GetTool(index);
    if (!tool.bFound)
    {
        ThemeColours::ShowMessageBox(wxT("Tool not found: ") + tool.sExeName, wxT("Error"), wxOK | wxICON_ERROR);
        return;
    }

    wxString sFolder = tool.sResolvedPath.BeforeLast('\\');
    auto result = reinterpret_cast<INT_PTR>(
        ShellExecute((HWND)GetHandle(), wxT("open"), tool.sResolvedPath, wxT(""), sFolder, SW_SHOWNORMAL));
    if (result <= 32)
    {
        ThemeColours::ShowMessageBox(wxT("Failed to launch ") + tool.sExeName, wxT("Error"), wxOK | wxICON_ERROR);
    }
}

void ConstructFrame::UpdateRelicToolsState()
{
    if (m_moduleManager.HasModule())
    {
        m_relicToolResolver.ScanWithModule(m_moduleManager.GetModuleFile(),
                                           m_moduleManager.GetModule()->GetModuleType());
    }
    else
    {
        m_relicToolResolver.ScanNoModule();
    }

    m_menuController.UpdateRelicToolsState(this, m_relicToolResolver);
}

CRgdHashTable *ConstructFrame::GetRgdHashTable()
{
    return m_moduleManager.GetRgdHashTable(AppStr(app_dictionariespath));
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

void ConstructFrame::OnThemeChange(wxCommandEvent &event)
{
    int iAppearance = 0; // system

    if (event.GetId() == IDM_ThemeLight)
    {
        iAppearance = 1;
    }
    else if (event.GetId() == IDM_ThemeDark)
    {
        iAppearance = 2;
    }

    TheConfig->Write(AppStr(config_appearance), iAppearance);
    ThemeColours::ShowMessageBox(wxT("The theme change will take effect when you restart the application."),
                                 wxT("Theme Changed"), wxOK | wxICON_INFORMATION, this);
}

wxAuiNotebook *ConstructFrame::GetTabs() { return m_tabManager.GetTabs(); }

void ConstructFrame::LaunchW40k(wxCommandEvent &event)
{
    UNUSED(event);
    wxString sCommand, sFolder, sCmdLine;
    try
    {
        sFolder = ConfGetDoWFolder();
    }
    catch (const CRainmanException &e)
    {
        ErrorBoxE(e);
        return;
    }
    sCommand = sFolder;
    sCommand.Append(wxT("W40k.exe"));
    sCmdLine = wxT("");
    if (m_moduleManager.GetModule())
    {
        sCmdLine = m_moduleManager.GetModuleFile();
        sCmdLine = sCmdLine.BeforeLast('.');
        sCmdLine = sCmdLine.AfterLast('\\');
        sCmdLine.Prepend(wxT("-modname "));
    }

    if (GetMenuBar()->GetMenu(4)->FindItem(IDM_PlayDev)->IsChecked())
    {
        sCmdLine.Append(wxT(" -dev"));
    }
    if (GetMenuBar()->GetMenu(4)->FindItem(IDM_PlayNoMovies)->IsChecked())
    {
        sCmdLine.Append(wxT(" -nomovies"));
    }

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
    catch (const CRainmanException &e)
    {
        ErrorBoxE(e);
        return;
    }
    sCommand = sFolder;
    sCommand.Append(wxT("DarkCrusade.exe"));
    sCmdLine = wxT("");
    if (m_moduleManager.GetModule())
    {
        sCmdLine = m_moduleManager.GetModuleFile();
        sCmdLine = sCmdLine.BeforeLast('.');
        sCmdLine = sCmdLine.AfterLast('\\');
        sCmdLine.Prepend(wxT("-modname "));
    }

    if (GetMenuBar()->GetMenu(4)->FindItem(IDM_PlayDev)->IsChecked())
    {
        sCmdLine.Append(wxT(" -dev"));
    }
    if (GetMenuBar()->GetMenu(4)->FindItem(IDM_PlayNoMovies)->IsChecked())
    {
        sCmdLine.Append(wxT(" -nomovies"));
    }

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
    catch (const CRainmanException &e)
    {
        ErrorBoxE(e);
        return;
    }
    sCommand = sFolder;
    sCommand.Append(wxT("Soulstorm.exe"));
    sCmdLine = wxT("");
    if (m_moduleManager.GetModule())
    {
        sCmdLine = m_moduleManager.GetModuleFile();
        sCmdLine = sCmdLine.BeforeLast('.');
        sCmdLine = sCmdLine.AfterLast('\\');
        sCmdLine.Prepend(wxT("-modname "));
    }

    if (GetMenuBar()->GetMenu(4)->FindItem(IDM_PlayDev)->IsChecked())
    {
        sCmdLine.Append(wxT(" -dev"));
    }
    if (GetMenuBar()->GetMenu(4)->FindItem(IDM_PlayNoMovies)->IsChecked())
    {
        sCmdLine.Append(wxT(" -nomovies"));
    }

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
    catch (const CRainmanException &e)
    {
        ErrorBoxE(e);
        return;
    }
    sCommand = sFolder;
    if (sCommand.Last() != wxChar('\\'))
    {
        sCommand.Append(wxT("\\"));
    }
    sCommand.Append(wxT("RelicCOH.exe"));
    sCmdLine = wxT("");
    if (m_moduleManager.GetModule())
    {
        sCmdLine = AsciiTowxString(m_moduleManager.GetModule()->GetName());
        sCmdLine.Prepend(wxT("-modname "));
    }

    if (GetMenuBar()->GetMenu(4)->FindItem(IDM_PlayDev)->IsChecked())
    {
        sCmdLine.Append(wxT(" -dev"));
    }
    if (GetMenuBar()->GetMenu(4)->FindItem(IDM_PlayNoMovies)->IsChecked())
    {
        sCmdLine.Append(wxT(" -nomovies"));
    }

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
    catch (const CRainmanException &e)
    {
        ErrorBoxE(e);
        return;
    }
    sCommand = sFolder;
    sCommand.Append(wxT("W40kWA.exe"));
    sCmdLine = wxT("");
    if (m_moduleManager.GetModule())
    {
        sCmdLine = m_moduleManager.GetModuleFile();
        sCmdLine = sCmdLine.BeforeLast('.');
        sCmdLine = sCmdLine.AfterLast('\\');
        sCmdLine.Prepend(wxT("-modname "));
    }

    if (GetMenuBar()->GetMenu(4)->FindItem(IDM_PlayDev)->IsChecked())
    {
        sCmdLine.Append(wxT(" -dev"));
    }
    if (GetMenuBar()->GetMenu(4)->FindItem(IDM_PlayNoMovies)->IsChecked())
    {
        sCmdLine.Append(wxT(" -nomovies"));
    }

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
        catch (const CRainmanException &e)
        {
            ErrorBoxE(e);
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
    : wxFrame((wxFrame *)NULL, -1, sTitle, oPos, oSize, wxDEFAULT_FRAME_STYLE | wxMAXIMIZE),
      m_moduleLoadPresenter(*this, this)
{
    // Initiate tools
    m_toolRegistry.Register(new CLocaleTool);
    m_toolRegistry.Register(new CUcsTool);
    m_toolRegistry.Register(new CSgaPackerTool);
    m_toolRegistry.Register(new CExtractAllTool);
    m_toolRegistry.Register(new CDpsCalculatorTool);
    m_toolRegistry.Register(new CRefreshFilesTool);
    m_toolRegistry.Register(new CMakeLuaInheritTree);
    m_toolRegistry.Register(new CAESetupTool);

    // Make Splitter+ Tabs
    m_tabManager.Init(this, IDC_Splitter);
    m_tabManager.AddPage(new frmWelcome(m_tabManager.GetTabs(), -1), AppStr(welcome_tabname), false);

    // Build menus
    m_menuController.Build(this, m_toolRegistry, m_relicToolResolver);

    // Initial scan for Relic tools (no module loaded yet)
    UpdateRelicToolsState();

    // Make Statusbar
    CreateStatusBar();
    SetStatusText(AppStr(statusbar_message_default));

    // Keyboard accelerators
    wxAcceleratorEntry accel[] = {
        wxAcceleratorEntry(wxACCEL_CTRL, (int)'S', IDM_SaveActive),
        wxAcceleratorEntry(wxACCEL_CTRL, (int)'W', IDM_CloseActiveTab),
        wxAcceleratorEntry(wxACCEL_CTRL, WXK_TAB, IDM_NextTab),
    };
    SetAcceleratorTable(wxAcceleratorTable(3, accel));
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
    // Shut down LSP before children are destroyed, so that
    // frmScarEditor::~frmScarEditor can safely skip the close notification.
    m_bLspShutDown = true;
    m_pLspClient.reset();

    frmRGDEditor::FreeNodeHelp();
    // ModuleManager destructor handles m_pModule, hash service, etc.
    // ToolRegistry destructor handles tool cleanup
}

CModuleFile *ConstructFrame::GetModule() const { return m_moduleManager.GetModule(); }

lsp::CLspClient *ConstructFrame::GetLspClient()
{
    if (m_bLspShutDown)
    {
        return nullptr;
    }

    if (m_pLspClient && m_pLspClient->IsRunning())
    {
        return m_pLspClient.get();
    }

    // Locate LuaLS binary next to the executable
    wxFileName exeDir(wxStandardPaths::Get().GetExecutablePath());
    wxString lspBinDir = exeDir.GetPath() + wxT("\\Mod_Studio_Files\\lsp\\luals\\bin");
    wxString serverPath = lspBinDir + wxT("\\lua-language-server.exe");

    if (!wxFileExists(serverPath))
    {
        CDMS_LOG_WARN("LSP: LuaLS binary not found at: {}", serverPath.ToStdString());
        return nullptr;
    }

    CDMS_LOG_INFO("LSP: Found LuaLS at: {}", serverPath.ToStdString());

    // Build workspace root and library paths
    std::string workspaceRoot;
    std::vector<std::string> libraryPaths;

    wxString lspDir = exeDir.GetPath() + wxT("\\Mod_Studio_Files\\lsp");
    libraryPaths.push_back(wxStringToAscii(lspDir + wxT("\\scar-types.lua")).get());

    bool isDow =
        m_moduleManager.HasModule() && m_moduleManager.GetModuleService().GetModuleType() == CModuleFile::MT_DawnOfWar;
    if (isDow)
    {
        libraryPaths.push_back(wxStringToAscii(lspDir + wxT("\\lua502-compat.lua")).get());
        libraryPaths.push_back(wxStringToAscii(lspDir + wxT("\\scar-dow.lua")).get());
    }
    else
    {
        libraryPaths.push_back(wxStringToAscii(lspDir + wxT("\\scar-coh.lua")).get());
    }

    // LuaLS settings
    nlohmann::json settings;
    settings["Lua"]["runtime"]["version"] = "Lua 5.1";
    settings["Lua"]["runtime"]["builtin"]["io"] = "disable";
    settings["Lua"]["runtime"]["builtin"]["debug"] = "disable";
    settings["Lua"]["runtime"]["builtin"]["os"] = "disable";
    settings["Lua"]["runtime"]["builtin"]["package"] = "disable";
    settings["Lua"]["diagnostics"]["globals"] = {"GameData", "MetaData", "Reference", "InheritMeta", "import"};
    settings["Lua"]["diagnostics"]["disable"] = {"lowercase-global", "trailing-space"};
    settings["Lua"]["files"]["associations"]["*.scar"] = "lua";
    settings["Lua"]["workspace"]["library"] = libraryPaths;

    m_pLspClient = std::make_unique<lsp::CLspClient>();
    if (!m_pLspClient->Start(serverPath.ToStdWstring(), workspaceRoot, libraryPaths, settings))
    {
        CDMS_LOG_WARN("Failed to start Lua Language Server");
        m_pLspClient.reset();
        return nullptr;
    }

    return m_pLspClient.get();
}

const wxString &ConstructFrame::GetModuleFile() const { return m_moduleManager.GetModuleFile(); }

void ConstructFrame::OnCloseMod(wxCommandEvent &event) { SetModule(nullptr); }

void ConstructFrame::SetModule(CModuleFile *pMod, const wxString &sModuleFile)
{
    if (pMod != 0)
    {
        m_moduleManager.SetModule(pMod, sModuleFile);
        if (!m_tabManager.IsSplit())
        {
            GetMenuBar()->GetMenu(0)->Enable(wxID_CLOSE, true);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModDoWWA, false);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModDC, false);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModSS, false);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModDE, false);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModCoH, false);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadSga, false);
            GetMenuBar()->GetMenu(0)->Enable(wxID_NEW, false);
            GetMenuBar()->EnableTop(2, true);
            frmFiles *pFiles = new frmFiles(m_tabManager.GetSplitter(), -1);
            m_tabManager.SplitWithFileTree(pFiles, TheConfig->Read(AppStr(config_sashposition), 189));
            pFiles->FillFromIDirectoryTraverser(pMod);
            m_tabManager.AddPage(new frmModule(m_tabManager.GetTabs(), -1),
                                 AppStr(mod_tabname)
                                     .Append(wxString(wxT(" [")))
                                     .Append(m_moduleManager.GetModuleFile().AfterLast('\\'))
                                     .Append(wxT("]")),
                                 true);
            while (m_tabManager.GetTabs()->GetPageCount() > 1)
            {
                m_tabManager.GetTabs()->DeletePage(0);
            }

            GetMenuBar()->GetMenu(4)->Enable(IDM_PlayCOH, false);
            GetMenuBar()->GetMenu(4)->Enable(IDM_PlayW40k, false);
            GetMenuBar()->GetMenu(4)->Enable(IDM_PlayWXP, false);
            GetMenuBar()->GetMenu(4)->Enable(IDM_PlayDC, false);
            GetMenuBar()->GetMenu(4)->Enable(IDM_PlaySS, false);

            switch (pMod->GetModuleType())
            {
            case CModuleFile::MT_DawnOfWar:
                GetMenuBar()->GetMenu(4)->Enable(IDM_PlayW40k, true);
                GetMenuBar()->GetMenu(4)->Enable(IDM_PlayWXP, true);
                GetMenuBar()->GetMenu(4)->Enable(IDM_PlayDC, true);
                GetMenuBar()->GetMenu(4)->Enable(IDM_PlaySS, true);
                break;

            case CModuleFile::MT_CompanyOfHeroes:
            case CModuleFile::MT_CompanyOfHeroesEarly:
                GetMenuBar()->GetMenu(4)->Enable(IDM_PlayCOH, true);
                break;
            }
        }

        UpdateRelicToolsState();
    }
    else
    {
        GetMenuBar()->GetMenu(4)->Enable(IDM_PlayCOH, false);
        GetMenuBar()->GetMenu(4)->Enable(IDM_PlayW40k, false);
        GetMenuBar()->GetMenu(4)->Enable(IDM_PlayWXP, false);
        GetMenuBar()->GetMenu(4)->Enable(IDM_PlayDC, false);
        GetMenuBar()->GetMenu(4)->Enable(IDM_PlaySS, false);

        if (m_tabManager.IsSplit())
        {
            GetMenuBar()->GetMenu(0)->Enable(wxID_CLOSE, false);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModDoWWA, true);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModDC, true);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModSS, true);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModDE, true);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModCoH, true);
            GetMenuBar()->GetMenu(0)->Enable(IDM_LoadSga, true);
            GetMenuBar()->GetMenu(0)->Enable(wxID_NEW, true);
            GetMenuBar()->EnableTop(2, false);

            // Destroy the tree before the module — tree items hold borrowed
            // pointers (sMod, sSource) into CFileMap data owned by the module.
            m_tabManager.ShowWelcomePage();
            m_tabManager.UnsplitFileTree();
        }

        m_moduleManager.SetModule(nullptr, sModuleFile);

        UpdateRelicToolsState();
    }
}

void ConstructFrame::SetLoadingForm(frmLoading *pLoading) { m_pLoadingForm = pLoading; }

frmLoading *ConstructFrame::GetLoadingForm() { return m_pLoadingForm; }

// --- IMainFrameView implementation ---

void ConstructFrame::ShowLoadingDialog(const wxString &sMessage)
{
    m_pLoadingForm = new frmLoading(AppStr(mod_loading));
    m_pLoadingForm->SetCancelCallback([this]() { m_moduleLoadPresenter.Cancel(); });
    m_pLoadingForm->SetMessage(sMessage);
    m_pLoadingForm->Show(true);
}

void ConstructFrame::HideLoadingDialog()
{
    if (m_pLoadingForm)
    {
        m_pLoadingForm->Close(true);
        delete m_pLoadingForm;
        m_pLoadingForm = nullptr;
    }
}

void ConstructFrame::UpdateLoadingProgress(const wxString &sMessage)
{
    if (m_pLoadingForm)
    {
        m_pLoadingForm->SetMessage(sMessage);
    }
}

void ConstructFrame::ShowError(const wxString &sMessage) { ErrorBoxS(sMessage); }

void ConstructFrame::OnModuleLoaded(CModuleFile *pMod, const wxString &sPath, bool bIsSga)
{
    if (bIsSga)
    {
        pMod->GetFileMap()->SetAuxOutputSupply(SaveFileCallback, (void *)this);
    }

    SetModule(pMod, sPath);

    if (bIsSga)
    {
        GetMenuBar()->EnableTop(4, false);
    }

    Refresh();
}

void ConstructFrame::DisableLoadMenuItems()
{
    GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModDoWWA, false);
    GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModDC, false);
    GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModSS, false);
    GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModDE, false);
    GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModCoH, false);
    GetMenuBar()->GetMenu(0)->Enable(IDM_LoadSga, false);
    GetMenuBar()->GetMenu(0)->Enable(wxID_NEW, false);
}

void ConstructFrame::EnableLoadMenuItems()
{
    // Only re-enable if no module is loaded (SetModule disables them when a mod is loaded)
    if (!GetModule())
    {
        GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModDoWWA, true);
        GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModDC, true);
        GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModSS, true);
        GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModDE, true);
        GetMenuBar()->GetMenu(0)->Enable(IDM_LoadModCoH, true);
        GetMenuBar()->GetMenu(0)->Enable(IDM_LoadSga, true);
        GetMenuBar()->GetMenu(0)->Enable(wxID_NEW, true);
    }
}

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

void ConstructFrame::OnOpenModDE(wxCommandEvent &event)
{
    UNUSED(event);
    DoLoadMod(wxT(""), LM_DE);
}

void ConstructFrame::OnOpenModCoH(wxCommandEvent &event)
{
    UNUSED(event);
    DoLoadMod(wxT(""), LM_CoH_OF);
}

void ConstructFrame::HideDonate(wxCommandEvent &event)
{
    UNUSED(event);
    ThemeColours::ShowMessageBox(AppStr(hidedonate_text), AppStr(hidedonate_menu), wxICON_INFORMATION,
                                 wxTheApp->GetTopWindow());
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
    {
        return 0;
    }

    return pConstruct->GetModuleService().GetModule()->GetFileSystem()->OpenOutputStreamW(sSavename, bEraseIfPresent);
}

void ConstructFrame::DoLoadSga()
{
    if (m_moduleLoadPresenter.IsLoading())
    {
        return;
    }

    std::unique_ptr<wxFileDialog> pFileDialog(
        new wxFileDialog(this, AppStr(mod_select_sga), wxT(""), wxT(""), AppStr(mod_sga_filter), wxFD_OPEN));

    if (pFileDialog->ShowModal() == wxID_OK)
    {
        m_moduleLoadPresenter.LoadSga(pFileDialog->GetPath());
    }
}

void ConstructFrame::DoLoadMod(wxString sPath, eLoadModGames eGame)
{
    if (m_moduleLoadPresenter.IsLoading())
    {
        return;
    }

    wxFileDialog *pFileDialog = 0;
    if (sPath == wxT(""))
    {
        wxString sDoWPath;
        try
        {
            if (eGame == LM_CoH_OF)
            {
                sDoWPath = ConfGetCoHFolder();
            }
            else if (eGame == LM_DC)
            {
                sDoWPath = ConfGetDCFolder();
            }
            else if (eGame == LM_SS)
            {
                sDoWPath = ConfGetSSFolder();
            }
            else if (eGame == LM_DE)
            {
                sDoWPath = ConfGetDEFolder();
            }
            else if (eGame == LM_DoW_WA || eGame == LM_Any)
            {
                sDoWPath = ConfGetDoWFolder();
            }
        }
        catch (const CRainmanException &e)
        {
            ErrorBoxE(e);
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
        wxString sFilePath = pFileDialog ? pFileDialog->GetPath() : sPath;

        if (sPath == wxT(""))
        {
            wxString sAppFolder = sFilePath.BeforeLast('\\');
            sAppFolder.Append(wxT("\\"));
            if (eGame == LM_CoH_OF)
            {
                TheConfig->Write(AppStr(config_cohfolder), sAppFolder);
            }
            else if (eGame == LM_DC)
            {
                TheConfig->Write(AppStr(config_dcfolder), sAppFolder);
            }
            else if (eGame == LM_SS)
            {
                TheConfig->Write(AppStr(config_ssfolder), sAppFolder);
            }
            else if (eGame == LM_DE)
            {
                TheConfig->Write(AppStr(config_defolder), sAppFolder);
            }
            else if (eGame == LM_DoW_WA)
            {
                TheConfig->Write(AppStr(config_dowfolder), sAppFolder);
            }
        }

        // Compute MD5-based config key for per-mod settings
        auto sFileAscii = wxStringToAscii(sFilePath);
        if (!sFileAscii)
        {
            ErrorBox("Memory allocation error");
            delete pFileDialog;
            return;
        }

        MD5Context md5ModName;
        MD5InitKey(&md5ModName, sFileAscii.get());
        unsigned char sModConfigKey[17];
        wchar_t sModConfigKeyHex[33];
        sModConfigKey[16] = 0;
        sModConfigKeyHex[32] = 0;
        MD5Final(sModConfigKey, &md5ModName);
        for (int i = 0; i < 16; ++i)
        {
            sModConfigKeyHex[static_cast<size_t>(i) * 2] = "0123456789ABCDEF"[sModConfigKey[i] >> 4];
            sModConfigKeyHex[(static_cast<size_t>(i) * 2) + 1] = "0123456789ABCDEF"[sModConfigKey[i] & 15];
        }
        TheConfig->SetPath(wxString(wxT("/")).Append(sModConfigKeyHex));

        // Locale selection (main thread — shows UI dialog)
        bool bSkipLocale;
        TheConfig->Read(AppStr(config_mod_localeremember), &bSkipLocale, false);
        if (!bSkipLocale)
        {
            frmLocaleSelector *pLocaleSelect = new frmLocaleSelector(AppStr(localeselect_title), eGame);
            pLocaleSelect->ShowModal();
            delete pLocaleSelect;
        }

        wxString sLocale = TheConfig->Read(AppStr(config_mod_locale), AppStr(localeselect_default));

        // Resolve CoH My Documents path on the main thread
        bool bIsCoH = (eGame == LM_CoH_OF);
        wxString sMyDocumentsPath;
        if (bIsCoH)
        {
            wchar_t sMapPackDir[MAX_PATH];
            SHGetFolderPathW(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, sMapPackDir);
            wcscat(sMapPackDir, L"\\My Games\\Company of Heroes");
            sMyDocumentsPath = sMapPackDir;
        }

        // Hand off to presenter for async background loading
        m_moduleLoadPresenter.LoadMod(sFilePath, sLocale, bIsCoH, sMyDocumentsPath);
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
    if (!m_tabManager.CanCloseAll(event.CanVeto()))
    {
        event.Veto();
        return;
    }

    // Shut down the language server before destroying the frame
    m_bLspShutDown = true;
    if (m_pLspClient)
    {
        m_pLspClient->Stop();
        m_pLspClient.reset();
    }

    this->Destroy();
}
