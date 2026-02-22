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
#include <wx/timer.h>
#include <wx/stc/stc.h>
#include <vector>

/// A compact find bar for wxStyledTextCtrl editors, similar to VS Code's Ctrl+F bar.
///
/// Shows a search text input, next/prev navigation buttons, a match count label,
/// and a close button. Highlights all matches using a Scintilla indicator and
/// navigates between them. Case-insensitive by default.
///
/// Performance: match positions are cached in a sorted vector so navigation is
/// O(log N). Only matches visible in the current viewport (plus a buffer) are
/// painted as indicators — the full set is re-applied on scroll. The STC is
/// frozen during bulk indicator updates to suppress intermediate repaints.
class FindBar : public wxPanel
{
  public:
    /// Constructs the find bar. The bar starts hidden; call Activate()/Deactivate().
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
    wxTimer m_debounceTimer;

    int m_iCurrentMatch = -1;
    int m_iSearchLen = 0;
    std::vector<int> m_vMatchPositions;

    /// Byte range of the last viewport region we highlighted.
    int m_iHighlightStart = 0;
    int m_iHighlightEnd = 0;

    void OnSearchTextChanged(wxCommandEvent &event);
    void OnDebounceTimer(wxTimerEvent &event);
    void OnNext(wxCommandEvent &event);
    void OnPrev(wxCommandEvent &event);
    void OnClose(wxCommandEvent &event);
    void OnKeyDown(wxKeyEvent &event);
    void OnSTCPainted(wxStyledTextEvent &event);

    void BuildMatchCache(const wxString &sText);
    void ApplyViewportHighlights();
    void ClearHighlights();
    void UpdateMatchLabel();
    void NavigateToCurrentMatch();
    int FindNearestMatch(int iPos) const;

    static constexpr int DEBOUNCE_MS = 150;
    /// Extra lines beyond the viewport to highlight, reducing flicker during scroll.
    static constexpr int VIEWPORT_BUFFER_LINES = 50;

    enum
    {
        IDC_SearchInput = wxID_HIGHEST + 500,
        IDC_NextBtn,
        IDC_PrevBtn,
        IDC_CloseBtn,
        IDC_DebounceTimer
    };
};
