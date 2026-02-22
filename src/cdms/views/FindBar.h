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

#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/stc/stc.h>

/// A compact find bar for wxStyledTextCtrl editors, similar to VS Code's Ctrl+F bar.
///
/// Shows a search text input, next/prev navigation buttons, a match count label,
/// and a close button. Highlights all matches using a Scintilla indicator and
/// navigates between them. Case-insensitive by default.
class FindBar : public wxPanel
{
  public:
    /// Constructs the find bar. The bar starts hidden; call Show()/Hide() to toggle.
    /// @param parent  The parent window (the editor panel).
    /// @param pSTC    The styled text control to search within. Not owned.
    FindBar(wxWindow *parent, wxStyledTextCtrl *pSTC);

    /// Activates the bar: shows it, selects any existing search text, and focuses the input.
    void Activate();

    /// Deactivates the bar: hides it and clears all search highlights.
    void Deactivate();

    static constexpr int INDICATOR_FIND_HIGHLIGHT = 11;

  private:
    wxStyledTextCtrl *m_pSTC;
    wxTextCtrl *m_pSearchInput;
    wxStaticText *m_pMatchLabel;
    wxButton *m_pNextBtn;
    wxButton *m_pPrevBtn;
    wxButton *m_pCloseBtn;

    int m_iCurrentMatch = -1;
    int m_iTotalMatches = 0;

    void OnSearchTextChanged(wxCommandEvent &event);
    void OnNext(wxCommandEvent &event);
    void OnPrev(wxCommandEvent &event);
    void OnClose(wxCommandEvent &event);
    void OnKeyDown(wxKeyEvent &event);

    void DoSearch(bool bForward);
    void HighlightAllMatches(const wxString &sText);
    void ClearHighlights();
    void UpdateMatchLabel();
    void NavigateToMatch(int iMatchPos, int iLen);
    int CountMatches(const wxString &sText);

    enum
    {
        IDC_SearchInput = wxID_HIGHEST + 500,
        IDC_NextBtn,
        IDC_PrevBtn,
        IDC_CloseBtn
    };
};
