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

#include <wx/wxprec.h>
#include <wx/aui/auibook.h>
#include <wx/splitter.h>

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

class frmFiles;

class TabManager
{
  public:
    TabManager() : m_pSplitter(nullptr), m_pTabs(nullptr), m_pPreviewPage(nullptr) {}

    // Initialize splitter and notebook. Called from ConstructFrame constructor.
    void Init(wxWindow *parent, wxWindowID splitterId);

    wxAuiNotebook *GetTabs() const { return m_pTabs; }
    wxSplitterWindow *GetSplitter() const { return m_pSplitter; }
    frmFiles *GetFilesList() const;
    bool IsSplit() const;

    // Tab page management
    void AddPage(wxWindow *page, const wxString &caption, bool select = true);
    void ShowWelcomePage();

    // Preview tab support (VS Code-style single-click preview)
    void SetPreviewPage(wxWindow *page, const wxString &filePath);
    void PinPreviewTab();
    void ClosePreviewTab();
    bool HasPreviewTab() const { return m_pPreviewPage != nullptr; }
    bool IsPreviewTab(wxWindow *page) const { return page == m_pPreviewPage; }
    const wxString &GetPreviewFilePath() const { return m_sPreviewFilePath; }
    void OnPageClosed(wxWindow *page);

    // Split/unsplit management
    void SplitWithFileTree(frmFiles *pFiles, int sashPosition);
    void UnsplitFileTree();

    // Save/restore sash position
    void OnSashMoved(int position);

    // Close handling: vetoes if any tab vetoes
    bool CanCloseAll(bool canVeto);

  private:
    wxSplitterWindow *m_pSplitter;
    wxAuiNotebook *m_pTabs;
    wxWindow *m_pPreviewPage;
    wxString m_sPreviewFilePath;
};
