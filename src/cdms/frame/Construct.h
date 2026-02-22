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
#include "views/frmLoading.h"
#include "TabManager.h"
#include "views/frmFiles.h"
#include <rainman/module/CModuleFile.h>
#include "ModuleManager.h"
#include "common/ITool.h"
#include "ToolRegistry.h"
#include "MenuController.h"
#include "views/interfaces/IMainFrameView.h"
#include "presenters/CModuleLoadPresenter.h"
#include "RelicToolResolver.h"
#include <lsp/LspClient.h>
#include <memory>

class ConstructFrame : public wxFrame, public IMainFrameView
{
  protected:
    TabManager m_tabManager;
    ModuleManager m_moduleManager;
    frmLoading *m_pLoadingForm;

  public:
    // Backward compatibility: ConstructFrame::ITool resolves to the standalone ITool
    using ITool = ::ITool;

  protected:
    ToolRegistry m_toolRegistry;
    MenuController m_menuController;
    CModuleLoadPresenter m_moduleLoadPresenter;
    RelicToolResolver m_relicToolResolver;
    std::unique_ptr<lsp::CLspClient> m_pLspClient;
    bool m_bLspShutDown = false;

  public:
    ConstructFrame(const wxString &sTitle, const wxPoint &oPos, const wxSize &oSize);
    ~ConstructFrame();

    void OnCloseMod(wxCommandEvent &event);
    void OnQuit(wxCommandEvent &event);
    void OnOpenModDoW(wxCommandEvent &event);
    void OnOpenModDC(wxCommandEvent &event);
    void OnOpenModSS(wxCommandEvent &event);
    void OnOpenModDE(wxCommandEvent &event);
    void OnOpenModCoH(wxCommandEvent &event);
    void OnOpenModOF(wxCommandEvent &event);
    void OnOpenSga(wxCommandEvent &event);
    void DoNewMod();

    enum eLoadModGames
    {
        LM_Any,
        LM_DoW_WA,
        LM_DC,
        LM_SS,
        LM_DE,
        LM_CoH_OF,
    };

    void DoLoadSga();
    void DoLoadMod(wxString sPath = wxT(""), eLoadModGames eGame = LM_Any);
    void OnModProperties(wxCommandEvent &event);
    void OnToolMenuCommand(wxCommandEvent &event);
    void OnRelicToolCommand(wxCommandEvent &event);
    void UpdateRelicToolsState();
    void LaunchCOH(wxCommandEvent &event);
    void LaunchW40k(wxCommandEvent &event);
    void LaunchW40kWA(wxCommandEvent &event);
    void LaunchDC(wxCommandEvent &event);
    void LaunchSS(wxCommandEvent &event);
    void LaunchWarnings(wxCommandEvent &event);
    void LaunchCredits(wxCommandEvent &event);
    void LaunchHelp(wxCommandEvent &event);
    static void StaticLaunchHelp();
    void LaunchDonate(wxCommandEvent &event);
    void LaunchLuaRef(wxCommandEvent &event);
    void LaunchKresjahWiki(wxCommandEvent &event);
    void LaunchForumDoW(wxCommandEvent &event);
    void LaunchForumCoH(wxCommandEvent &event);
    void LaunchNewRDNWiki(wxCommandEvent &event);
    void LaunchRDNWiki(wxCommandEvent &event);
    void HideDonate(wxCommandEvent &event);
    void OnThemeChange(wxCommandEvent &event);

    static void LaunchURL(wxString sURL);

    void OnCloseWindow(wxCloseEvent &event);

    void OnSashMove(wxSplitterEvent &event);
    void OnTabClosing(wxAuiNotebookEvent &event);
    void OnSaveActive(wxCommandEvent &event);
    void OnCloseActiveTab(wxCommandEvent &event);
    void OnNextTab(wxCommandEvent &event);

    CModuleFile *GetModule() const;
    const wxString &GetModuleFile() const;
    void SetModule(CModuleFile *pMod, const wxString &sModuleFile = wxT(""));
    wxAuiNotebook *GetTabs();
    TabManager &GetTabManager() { return m_tabManager; }
    frmLoading *GetLoadingForm();
    frmFiles *GetFilesList();

    ModuleService &GetModuleService() { return m_moduleManager.GetModuleService(); }
    FileService &GetFileService() { return m_moduleManager.GetFileService(); }
    HashService &GetHashService() { return m_moduleManager.GetHashService(); }

    /// Get or create the shared LSP client. Returns nullptr if LuaLS is not available.
    lsp::CLspClient *GetLspClient();

    size_t GetToolCount();
    ITool *GetTool(size_t i);
    void DoTool(wxString sName);
    ToolRegistry &GetToolRegistry() { return m_toolRegistry; }

    void SetLoadingForm(frmLoading *pLoading);

    static IFileStore::IOutputStream *SaveFileCallback(const char *sFile, bool bEraseIfPresent, void *pTag);

    // --- IMainFrameView overrides ---
    void ShowLoadingDialog(const wxString &sMessage) override;
    void HideLoadingDialog() override;
    void UpdateLoadingProgress(const wxString &sMessage) override;
    void ShowError(const wxString &sMessage) override;
    void OnModuleLoaded(CModuleFile *pMod, const wxString &sPath, bool bIsSga) override;
    void DisableLoadMenuItems() override;
    void EnableLoadMenuItems() override;

    /*!
        may throw a CRainmanException
    */
    CRgdHashTable *GetRgdHashTable();

    DECLARE_EVENT_TABLE()
};

inline ConstructFrame *GetConstruct() { return (ConstructFrame *)wxTheApp->GetTopWindow(); }
#define TheConstruct GetConstruct()

enum
{
    // File Menu
    IDM_NewMod = wxID_HIGHEST + 1,
    IDM_LoadModDoWWA,
    IDM_LoadModDC,
    IDM_LoadModSS,
    IDM_LoadModDE,
    IDM_LoadModCoH,
    IDM_LoadSga,
    IDM_Quit,
    // Mod Menu
    IDM_Settings,
    IDM_Tools,
    // Mod.Tools Menu
    IDM_ModToolBase,
    IDM_ModToolLast = IDM_ModToolBase + 99, // Reserve 100 dynamic tool slots
                                            // Relic's Tools Menu
    IDM_RelicToolBase,
    IDM_RelicToolLast = IDM_RelicToolBase + 19, // Reserve 20 slots for Relic tools
                                                // Play Menu
    IDM_PlayCOH,
    IDM_PlayW40k,
    IDM_PlayWXP,
    IDM_PlayDC,
    IDM_PlaySS,
    IDM_PlayWarn,
    IDM_PlayDev,
    IDM_PlayNoMovies,
    // Help Menu
    IDM_Index,
    IDM_LuaRef,
    IDM_ForumDoW,
    IDM_ForumCoH,
    IDM_RDNWikiNew,
    IDM_RDNWiki,
    IDM_KresjahWiki,
    IDM_About,
    IDM_Credits,
    IDM_HideDonate,

    // View Menu — Theme
    IDM_ThemeSystem,
    IDM_ThemeLight,
    IDM_ThemeDark,

    // Accelerators
    IDM_SaveActive,
    IDM_CloseActiveTab,
    IDM_NextTab,

    // Stuff
    IDC_Splitter
};
