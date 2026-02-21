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

#include "MenuController.h"
#include "Construct.h"
#include "common/strings.h"
#include "ToolRegistry.h"
#include "RelicToolResolver.h"

void MenuController::Build(wxFrame *pFrame, ToolRegistry &registry, RelicToolResolver &relicResolver)
{
    auto *pConstruct = static_cast<ConstructFrame *>(pFrame);

    // File menu
    auto *pMenu_File = new wxMenu;
    pMenu_File->Append(wxID_NEW, AppStr(new_mod_menu), AppStr(new_mod));
    pMenu_File->Append(IDM_LoadModDoWWA, AppStr(open_mod_menu), AppStr(open_mod_help));
    pMenu_File->Append(IDM_LoadModDC, AppStr(open_moddc_menu), AppStr(open_moddc_help));
    pMenu_File->Append(IDM_LoadModSS, AppStr(open_modss_menu), AppStr(open_modss_help));
    pMenu_File->Append(IDM_LoadModDE, AppStr(open_modde_menu), AppStr(open_modde_help));
    pMenu_File->Append(IDM_LoadModCoH, AppStr(open_modcoh_menu), AppStr(open_modcoh_help));
    pMenu_File->Append(IDM_LoadSga, AppStr(open_sga_menu), AppStr(open_sga_help));
    pMenu_File->Append(wxID_CLOSE, AppStr(close_mod_menu), AppStr(close_mod_help));
    pMenu_File->AppendSeparator();
    pMenu_File->Append(wxID_EXIT, AppStr(exit_menu), AppStr(exit_help));
    pMenu_File->Enable(wxID_CLOSE, false);

    // Mod menu
    auto *pMenu_Mod = new wxMenu;
    pMenu_Mod->Append(wxID_PROPERTIES, AppStr(mod_properties_menu), AppStr(mod_properties_help));
    pMenu_Mod->AppendSeparator();
    for (size_t t = 0; t < registry.GetCount(); ++t)
    {
        int id = IDM_ModToolBase + static_cast<int>(t);
        pMenu_Mod->Append(id, registry.Get(t)->GetName(), registry.Get(t)->GetHelpString());
        pConstruct->Bind(wxEVT_MENU, &ConstructFrame::OnToolMenuCommand, pConstruct, id);
    }

    // Relic tools menu — built from resolver's tool list
    auto *pMenu_RelicTools = new wxMenu;
    for (size_t i = 0; i < relicResolver.GetToolCount(); ++i)
    {
        const auto &tool = relicResolver.GetTool(i);
        int id = IDM_RelicToolBase + static_cast<int>(i);
        pMenu_RelicTools->Append(id, tool.sMenuLabel, tool.sHelpString);
        pMenu_RelicTools->Enable(id, tool.bFound);
        pConstruct->Bind(wxEVT_MENU, &ConstructFrame::OnRelicToolCommand, pConstruct, id);
    }

    // Play menu
    auto *pMenu_Play = new wxMenu;
    pMenu_Play->Append(IDM_PlayCOH, AppStr(play_coh), AppStr(play_coh_help));
    pMenu_Play->Append(IDM_PlayW40k, AppStr(play_w40k), AppStr(play_w40k_help));
    pMenu_Play->Append(IDM_PlayWXP, AppStr(play_wxp), AppStr(play_wxp_help));
    pMenu_Play->Append(IDM_PlayDC, AppStr(play_dc), AppStr(play_dc_help));
    pMenu_Play->Append(IDM_PlaySS, AppStr(play_ss), AppStr(play_ss_help));
    pMenu_Play->Append(IDM_PlayWarn, AppStr(play_warn), AppStr(play_warn_help));
    pMenu_Play->AppendSeparator();
    pMenu_Play->AppendCheckItem(IDM_PlayDev, AppStr(play_dev), AppStr(play_dev_help));
    pMenu_Play->AppendCheckItem(IDM_PlayNoMovies, AppStr(play_nomov), AppStr(play_nomov_help));
    pMenu_Play->Check(IDM_PlayNoMovies, true);
    pMenu_Play->Enable(IDM_PlayCOH, false);
    pMenu_Play->Enable(IDM_PlayW40k, false);
    pMenu_Play->Enable(IDM_PlayWXP, false);
    pMenu_Play->Enable(IDM_PlayDC, false);
    pMenu_Play->Enable(IDM_PlaySS, false);

    // Help menu
    auto *pMenu_Help = new wxMenu;
    pMenu_Help->Append(wxID_HELP_CONTENTS, AppStr(help_index_menu), AppStr(help_index_help));
    pMenu_Help->Append(IDM_RDNWikiNew, AppStr(rdn_new_wiki_menu), AppStr(rdn_new_wiki_help));
    pMenu_Help->Append(IDM_ForumDoW, AppStr(forum_dow_menu), AppStr(forum_dow_help));
    pMenu_Help->Append(IDM_ForumCoH, AppStr(forum_coh_menu), AppStr(forum_coh_help));
    pMenu_Help->Append(IDM_RDNWiki, AppStr(rdn_wiki_menu), AppStr(rdn_wiki_help));
    pMenu_Help->Append(IDM_LuaRef, AppStr(lua_ref_menu), AppStr(lua_ref_help));
    pMenu_Help->Append(IDM_KresjahWiki, AppStr(kresjah_wiki_menu), AppStr(kresjah_wiki_help));
    pMenu_Help->AppendSeparator();
    pMenu_Help->Append(wxID_ABOUT, AppStr(about_menu), AppStr(about_help));
    pMenu_Help->Append(IDM_Credits, AppStr(credits_menu), AppStr(credits_help));
    pMenu_Help->Append(IDM_HideDonate, AppStr(hidedonate_menu), AppStr(hidedonate_help));

    // Assemble menu bar
    auto *pMenuBar = new wxMenuBar;
    pMenuBar->Append(pMenu_File, AppStr(file_menu));
    pMenuBar->Append(pMenu_Mod, AppStr(mod_menu));
    pMenuBar->Append(pMenu_RelicTools, AppStr(relic_tools_menu));
    pMenuBar->Append(pMenu_Play, AppStr(play_menu));
    pMenuBar->Append(pMenu_Help, AppStr(help_menu));
    pFrame->SetMenuBar(pMenuBar);
    pMenuBar->EnableTop(1, false);
}

void MenuController::UpdateRelicToolsState(wxFrame *pFrame, const RelicToolResolver &resolver)
{
    wxMenuBar *pMenuBar = pFrame->GetMenuBar();
    if (!pMenuBar)
    {
        return;
    }

    for (size_t i = 0; i < resolver.GetToolCount(); ++i)
    {
        int id = IDM_RelicToolBase + static_cast<int>(i);
        wxMenuItem *pItem = pMenuBar->FindItem(id);
        if (pItem)
        {
            pItem->Enable(resolver.GetTool(i).bFound);
        }
    }
}
