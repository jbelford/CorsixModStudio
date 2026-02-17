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

#include "TabManager.h"
#include "views/frmFiles.h"
#include "views/frmWelcome.h"
#include "common/strings.h"
#include "common/config.h"

void TabManager::Init(wxWindow *parent, wxWindowID splitterId)
{
    m_pSplitter = new wxSplitterWindow(parent, splitterId);
    m_pSplitter->SetSashGravity(0.5);
    m_pSplitter->SetMinimumPaneSize(48);
    m_pTabs = new wxAuiNotebook(m_pSplitter, -1, wxPoint(0, 0), wxDefaultSize,
                                wxAUI_NB_DEFAULT_STYLE | wxAUI_NB_TAB_EXTERNAL_MOVE | wxAUI_NB_WINDOWLIST_BUTTON |
                                    wxNO_BORDER | wxAUI_NB_CLOSE_BUTTON);
    m_pSplitter->Initialize(m_pTabs);
}

frmFiles *TabManager::GetFilesList() const
{
    if (!m_pSplitter->IsSplit())
        return nullptr;
    return static_cast<frmFiles *>(m_pSplitter->GetWindow1());
}

bool TabManager::IsSplit() const { return m_pSplitter->IsSplit(); }

void TabManager::AddPage(wxWindow *page, const wxString &caption, bool select)
{
    m_pTabs->AddPage(page, caption, select);
}

void TabManager::ShowWelcomePage()
{
    m_pTabs->AddPage(new frmWelcome(m_pTabs, -1), AppStr(welcome_tabname), true);
    while (m_pTabs->GetPageCount() > 1)
        m_pTabs->DeletePage(0);
}

void TabManager::SplitWithFileTree(frmFiles *pFiles, int sashPosition)
{
    m_pSplitter->SplitVertically(pFiles, m_pTabs);
    m_pSplitter->SetSashPosition(sashPosition);
}

void TabManager::UnsplitFileTree()
{
    if (m_pSplitter->IsSplit())
    {
        frmFiles *pFiles = GetFilesList();
        m_pSplitter->Unsplit(pFiles);
        delete pFiles;
    }
}

void TabManager::OnSashMoved(int position) { TheConfig->Write(AppStr(config_sashposition), position); }

bool TabManager::CanCloseAll(bool canVeto)
{
    for (size_t i = 0; i < m_pTabs->GetPageCount(); ++i)
    {
        wxCloseEvent e(wxEVT_CLOSE_WINDOW);
        e.SetCanVeto(canVeto);
        m_pTabs->GetPage(i)->GetEventHandler()->ProcessEvent(e);
        if (e.GetVeto())
            return false;
    }
    return true;
}
