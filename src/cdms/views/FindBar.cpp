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

#include "FindBar.h"
#include <wx/sizer.h>
#include <algorithm>

FindBar::FindBar(wxWindow *parent, wxStyledTextCtrl *pSTC) : wxPanel(parent, wxID_ANY), m_pSTC(pSTC)
{
    auto *pSizer = new wxBoxSizer(wxHORIZONTAL);

    m_pSearchInput =
        new wxTextCtrl(this, IDC_SearchInput, wxEmptyString, wxDefaultPosition, wxSize(220, -1), wxTE_PROCESS_ENTER);
    m_pSearchInput->SetHint("Find");

    m_pPrevBtn = new wxButton(this, IDC_PrevBtn, wxS("\u25B2"), wxDefaultPosition, wxSize(30, -1));
    m_pNextBtn = new wxButton(this, IDC_NextBtn, wxS("\u25BC"), wxDefaultPosition, wxSize(30, -1));
    m_pCloseBtn = new wxButton(this, IDC_CloseBtn, wxS("\u00D7"), wxDefaultPosition, wxSize(30, -1));

    m_pPrevBtn->SetToolTip("Previous Match (Shift+Enter)");
    m_pNextBtn->SetToolTip("Next Match (Enter)");
    m_pCloseBtn->SetToolTip("Close (Escape)");

    m_pMatchLabel = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(80, -1),
                                     wxALIGN_CENTRE_HORIZONTAL | wxST_NO_AUTORESIZE);

    pSizer->AddSpacer(4);
    pSizer->Add(m_pSearchInput, 0, wxALIGN_CENTER_VERTICAL | wxTOP | wxBOTTOM, 3);
    pSizer->Add(m_pPrevBtn, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 2);
    pSizer->Add(m_pNextBtn, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 1);
    pSizer->Add(m_pMatchLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 6);
    pSizer->AddStretchSpacer();
    pSizer->Add(m_pCloseBtn, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 4);

    SetSizer(pSizer);

    // Set up the Scintilla indicator for highlighting all matches
    m_pSTC->IndicatorSetStyle(INDICATOR_FIND_HIGHLIGHT, wxSTC_INDIC_ROUNDBOX);
    m_pSTC->IndicatorSetForeground(INDICATOR_FIND_HIGHLIGHT, wxColour(255, 200, 0));
    m_pSTC->IndicatorSetAlpha(INDICATOR_FIND_HIGHLIGHT, 80);
    m_pSTC->IndicatorSetOutlineAlpha(INDICATOR_FIND_HIGHLIGHT, 120);

    // Bind events
    m_pSearchInput->Bind(wxEVT_TEXT, &FindBar::OnSearchTextChanged, this);
    m_pSearchInput->Bind(wxEVT_TEXT_ENTER,
                         [this](wxCommandEvent &)
                         {
                             wxCommandEvent dummy;
                             OnNext(dummy);
                         });
    m_pSearchInput->Bind(wxEVT_KEY_DOWN, &FindBar::OnKeyDown, this);
    m_pNextBtn->Bind(wxEVT_BUTTON, &FindBar::OnNext, this);
    m_pPrevBtn->Bind(wxEVT_BUTTON, &FindBar::OnPrev, this);
    m_pCloseBtn->Bind(wxEVT_BUTTON, &FindBar::OnClose, this);

    // Re-highlight when the viewport scrolls or is repainted
    m_pSTC->Bind(wxEVT_STC_PAINTED, &FindBar::OnSTCPainted, this);

    // Debounce timer for the expensive full-document scan
    m_debounceTimer.SetOwner(this, IDC_DebounceTimer);
    Bind(wxEVT_TIMER, &FindBar::OnDebounceTimer, this, IDC_DebounceTimer);

    Hide();
}

void FindBar::Activate()
{
    Show();
    GetParent()->Layout();

    // If there's a selection in the STC, pre-fill the search input
    wxString sSelection = m_pSTC->GetSelectedText();
    if (!sSelection.empty() && !sSelection.Contains('\n'))
    {
        m_pSearchInput->SetValue(sSelection);
    }

    m_pSearchInput->SelectAll();
    m_pSearchInput->SetFocus();
}

void FindBar::Deactivate()
{
    m_debounceTimer.Stop();
    ClearHighlights();
    m_pMatchLabel->SetLabel(wxEmptyString);
    m_iCurrentMatch = -1;
    m_iSearchLen = 0;
    m_vMatchPositions.clear();
    m_iHighlightStart = 0;
    m_iHighlightEnd = 0;
    Hide();
    GetParent()->Layout();
    m_pSTC->SetFocus();
}

void FindBar::OnSearchTextChanged(wxCommandEvent &)
{
    wxString sText = m_pSearchInput->GetValue();
    if (sText.empty())
    {
        m_debounceTimer.Stop();
        ClearHighlights();
        m_pMatchLabel->SetLabel(wxEmptyString);
        m_iCurrentMatch = -1;
        m_iSearchLen = 0;
        m_vMatchPositions.clear();
        m_iHighlightStart = 0;
        m_iHighlightEnd = 0;
        return;
    }

    // Immediately navigate to the nearest match for responsiveness
    // (single FindText call is cheap)
    int iCaretPos = m_pSTC->GetCurrentPos();
    int iPos = m_pSTC->FindText(iCaretPos, m_pSTC->GetTextLength(), sText, 0);
    if (iPos == wxNOT_FOUND)
    {
        iPos = m_pSTC->FindText(0, iCaretPos, sText, 0);
    }

    if (iPos != wxNOT_FOUND)
    {
        m_pSTC->SetSelection(iPos, iPos + static_cast<int>(sText.length()));
        m_pSTC->EnsureCaretVisible();
        m_pMatchLabel->SetLabel(wxS("\u2026")); // ellipsis while debouncing
    }
    else
    {
        m_pMatchLabel->SetLabel("No results");
    }

    // Debounce the expensive full-document scan + highlight pass
    m_debounceTimer.StartOnce(DEBOUNCE_MS);
}

void FindBar::OnDebounceTimer(wxTimerEvent &)
{
    wxString sText = m_pSearchInput->GetValue();
    if (sText.empty())
    {
        return;
    }

    // Single-pass: build cache of all match positions
    BuildMatchCache(sText);
    m_iSearchLen = static_cast<int>(sText.length());

    // Reset viewport tracking so highlights are fully redrawn
    m_iHighlightStart = 0;
    m_iHighlightEnd = 0;
    ApplyViewportHighlights();

    // Set current match index based on current selection
    if (!m_vMatchPositions.empty())
    {
        m_iCurrentMatch = FindNearestMatch(m_pSTC->GetSelectionStart());
        NavigateToCurrentMatch();
    }
    else
    {
        m_iCurrentMatch = -1;
    }

    UpdateMatchLabel();
}

void FindBar::OnNext(wxCommandEvent &)
{
    wxString sText = m_pSearchInput->GetValue();
    if (sText.empty() || m_vMatchPositions.empty())
    {
        return;
    }

    // If cache is stale (timer pending), flush it now
    if (m_debounceTimer.IsRunning())
    {
        m_debounceTimer.Stop();
        BuildMatchCache(sText);
        m_iSearchLen = static_cast<int>(sText.length());
        m_iHighlightStart = 0;
        m_iHighlightEnd = 0;
        ApplyViewportHighlights();
        if (m_vMatchPositions.empty())
        {
            return;
        }
    }

    int iSelEnd = m_pSTC->GetSelectionEnd();
    auto it = std::upper_bound(m_vMatchPositions.begin(), m_vMatchPositions.end(), iSelEnd - 1);
    if (it == m_vMatchPositions.end())
    {
        it = m_vMatchPositions.begin(); // wrap around
    }

    m_iCurrentMatch = static_cast<int>(it - m_vMatchPositions.begin());
    NavigateToCurrentMatch();
    UpdateMatchLabel();
}

void FindBar::OnPrev(wxCommandEvent &)
{
    wxString sText = m_pSearchInput->GetValue();
    if (sText.empty() || m_vMatchPositions.empty())
    {
        return;
    }

    if (m_debounceTimer.IsRunning())
    {
        m_debounceTimer.Stop();
        BuildMatchCache(sText);
        m_iSearchLen = static_cast<int>(sText.length());
        m_iHighlightStart = 0;
        m_iHighlightEnd = 0;
        ApplyViewportHighlights();
        if (m_vMatchPositions.empty())
        {
            return;
        }
    }

    int iSelStart = m_pSTC->GetSelectionStart();
    auto it = std::lower_bound(m_vMatchPositions.begin(), m_vMatchPositions.end(), iSelStart);
    if (it == m_vMatchPositions.begin())
    {
        it = m_vMatchPositions.end(); // wrap around
    }
    --it;

    m_iCurrentMatch = static_cast<int>(it - m_vMatchPositions.begin());
    NavigateToCurrentMatch();
    UpdateMatchLabel();
}

void FindBar::OnClose(wxCommandEvent &) { Deactivate(); }

void FindBar::OnKeyDown(wxKeyEvent &event)
{
    if (event.GetKeyCode() == WXK_ESCAPE)
    {
        Deactivate();
        return;
    }

    if (event.GetKeyCode() == WXK_RETURN || event.GetKeyCode() == WXK_NUMPAD_ENTER)
    {
        if (event.ShiftDown())
        {
            wxCommandEvent dummy;
            OnPrev(dummy);
        }
        else
        {
            wxCommandEvent dummy;
            OnNext(dummy);
        }
        return;
    }

    event.Skip();
}

void FindBar::OnSTCPainted(wxStyledTextEvent &)
{
    if (!IsShown() || m_vMatchPositions.empty() || m_iSearchLen == 0)
    {
        return;
    }

    ApplyViewportHighlights();
}

void FindBar::BuildMatchCache(const wxString &sText)
{
    m_vMatchPositions.clear();

    if (sText.empty())
    {
        return;
    }

    int iTextLen = m_pSTC->GetTextLength();
    int iPos = 0;
    while (true)
    {
        int iFound = m_pSTC->FindText(iPos, iTextLen, sText, 0);
        if (iFound == wxNOT_FOUND)
        {
            break;
        }
        m_vMatchPositions.push_back(iFound);
        iPos = iFound + 1;
    }
}

void FindBar::ApplyViewportHighlights()
{
    if (m_vMatchPositions.empty() || m_iSearchLen == 0)
    {
        return;
    }

    // Calculate the byte range of the visible viewport plus a buffer
    int iFirstVisible = m_pSTC->GetFirstVisibleLine();
    int iFirstLine = m_pSTC->DocLineFromVisible(iFirstVisible);
    int iLinesOnScreen = m_pSTC->LinesOnScreen();
    int iLastLine = iFirstLine + iLinesOnScreen;

    int iBufferedFirst = std::max(0, iFirstLine - VIEWPORT_BUFFER_LINES);
    int iBufferedLast = std::min(m_pSTC->GetLineCount() - 1, iLastLine + VIEWPORT_BUFFER_LINES);

    int iRangeStart = m_pSTC->PositionFromLine(iBufferedFirst);
    int iRangeEnd = m_pSTC->GetLineEndPosition(iBufferedLast);

    // Skip if the viewport hasn't moved outside the already-highlighted region
    if (iRangeStart >= m_iHighlightStart && iRangeEnd <= m_iHighlightEnd)
    {
        return;
    }

    // Freeze to batch all indicator changes into a single repaint
    m_pSTC->Freeze();

    m_pSTC->SetIndicatorCurrent(INDICATOR_FIND_HIGHLIGHT);
    m_pSTC->IndicatorClearRange(0, m_pSTC->GetTextLength());

    // Binary-search for matches in the viewport range
    auto itBegin = std::lower_bound(m_vMatchPositions.begin(), m_vMatchPositions.end(), iRangeStart);
    auto itEnd = std::upper_bound(itBegin, m_vMatchPositions.end(), iRangeEnd);

    for (auto it = itBegin; it != itEnd; ++it)
    {
        m_pSTC->IndicatorFillRange(*it, m_iSearchLen);
    }

    m_pSTC->Thaw();

    m_iHighlightStart = iRangeStart;
    m_iHighlightEnd = iRangeEnd;
}

void FindBar::ClearHighlights()
{
    m_pSTC->SetIndicatorCurrent(INDICATOR_FIND_HIGHLIGHT);
    m_pSTC->IndicatorClearRange(0, m_pSTC->GetTextLength());
    m_iHighlightStart = 0;
    m_iHighlightEnd = 0;
}

void FindBar::UpdateMatchLabel()
{
    if (m_vMatchPositions.empty())
    {
        m_pMatchLabel->SetLabel("No results");
    }
    else
    {
        m_pMatchLabel->SetLabel(
            wxString::Format("%d of %d", m_iCurrentMatch + 1, static_cast<int>(m_vMatchPositions.size())));
    }
}

void FindBar::NavigateToCurrentMatch()
{
    if (m_iCurrentMatch < 0 || m_iCurrentMatch >= static_cast<int>(m_vMatchPositions.size()))
    {
        return;
    }

    int iPos = m_vMatchPositions[m_iCurrentMatch];
    m_pSTC->SetSelection(iPos, iPos + m_iSearchLen);
    m_pSTC->EnsureCaretVisible();
}

int FindBar::FindNearestMatch(int iPos) const
{
    if (m_vMatchPositions.empty())
    {
        return -1;
    }

    auto it = std::lower_bound(m_vMatchPositions.begin(), m_vMatchPositions.end(), iPos);
    if (it == m_vMatchPositions.end())
    {
        return 0; // wrap to first
    }
    return static_cast<int>(it - m_vMatchPositions.begin());
}
