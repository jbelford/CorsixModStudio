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
    m_pSearchInput->Bind(wxEVT_TEXT_ENTER, [this](wxCommandEvent &) { DoSearch(true); });
    m_pSearchInput->Bind(wxEVT_KEY_DOWN, &FindBar::OnKeyDown, this);
    m_pNextBtn->Bind(wxEVT_BUTTON, &FindBar::OnNext, this);
    m_pPrevBtn->Bind(wxEVT_BUTTON, &FindBar::OnPrev, this);
    m_pCloseBtn->Bind(wxEVT_BUTTON, &FindBar::OnClose, this);

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
    ClearHighlights();
    m_pMatchLabel->SetLabel(wxEmptyString);
    m_iCurrentMatch = -1;
    m_iTotalMatches = 0;
    Hide();
    GetParent()->Layout();
    m_pSTC->SetFocus();
}

void FindBar::OnSearchTextChanged(wxCommandEvent &)
{
    wxString sText = m_pSearchInput->GetValue();
    if (sText.empty())
    {
        ClearHighlights();
        m_pMatchLabel->SetLabel(wxEmptyString);
        m_iCurrentMatch = -1;
        m_iTotalMatches = 0;
        return;
    }

    HighlightAllMatches(sText);
    m_iTotalMatches = CountMatches(sText);

    if (m_iTotalMatches > 0)
    {
        // Navigate to the first match from the current caret position
        m_iCurrentMatch = 0;
        DoSearch(true);
    }
    else
    {
        m_iCurrentMatch = -1;
        UpdateMatchLabel();
    }
}

void FindBar::OnNext(wxCommandEvent &) { DoSearch(true); }

void FindBar::OnPrev(wxCommandEvent &) { DoSearch(false); }

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
        DoSearch(!event.ShiftDown());
        return;
    }

    event.Skip();
}

void FindBar::DoSearch(bool bForward)
{
    wxString sText = m_pSearchInput->GetValue();
    if (sText.empty() || m_iTotalMatches == 0)
    {
        return;
    }

    int iStart;
    if (bForward)
    {
        // Search from end of current selection to avoid finding the same match
        iStart = m_pSTC->GetSelectionEnd();
        int iPos = m_pSTC->FindText(iStart, m_pSTC->GetTextLength(), sText, 0);
        if (iPos == wxNOT_FOUND)
        {
            // Wrap around to beginning
            iPos = m_pSTC->FindText(0, iStart, sText, 0);
        }
        if (iPos != wxNOT_FOUND)
        {
            NavigateToMatch(iPos, static_cast<int>(sText.length()));
        }
    }
    else
    {
        // Search backward from start of current selection
        iStart = m_pSTC->GetSelectionStart();
        int iPos = m_pSTC->FindText(iStart, 0, sText, 0);
        if (iPos == wxNOT_FOUND)
        {
            // Wrap around to end
            iPos = m_pSTC->FindText(m_pSTC->GetTextLength(), iStart, sText, 0);
        }
        if (iPos != wxNOT_FOUND)
        {
            NavigateToMatch(iPos, static_cast<int>(sText.length()));
        }
    }

    // Recount to update current match index
    if (m_iTotalMatches > 0)
    {
        int iSelStart = m_pSTC->GetSelectionStart();
        int iCurrent = 0;
        int iPos = 0;
        while (true)
        {
            int iFound = m_pSTC->FindText(iPos, m_pSTC->GetTextLength(), sText, 0);
            if (iFound == wxNOT_FOUND || iFound > iSelStart)
            {
                break;
            }
            if (iFound == iSelStart)
            {
                m_iCurrentMatch = iCurrent;
                break;
            }
            iCurrent++;
            iPos = iFound + 1;
        }
        UpdateMatchLabel();
    }
}

void FindBar::HighlightAllMatches(const wxString &sText)
{
    ClearHighlights();

    if (sText.empty())
    {
        return;
    }

    m_pSTC->SetIndicatorCurrent(INDICATOR_FIND_HIGHLIGHT);

    int iPos = 0;
    while (true)
    {
        int iFound = m_pSTC->FindText(iPos, m_pSTC->GetTextLength(), sText, 0);
        if (iFound == wxNOT_FOUND)
        {
            break;
        }

        m_pSTC->IndicatorFillRange(iFound, static_cast<int>(sText.length()));
        iPos = iFound + 1;
    }
}

void FindBar::ClearHighlights()
{
    m_pSTC->SetIndicatorCurrent(INDICATOR_FIND_HIGHLIGHT);
    m_pSTC->IndicatorClearRange(0, m_pSTC->GetTextLength());
}

void FindBar::UpdateMatchLabel()
{
    if (m_iTotalMatches == 0)
    {
        m_pMatchLabel->SetLabel("No results");
    }
    else
    {
        m_pMatchLabel->SetLabel(wxString::Format("%d of %d", m_iCurrentMatch + 1, m_iTotalMatches));
    }
}

void FindBar::NavigateToMatch(int iMatchPos, int iLen)
{
    m_pSTC->SetSelection(iMatchPos, iMatchPos + iLen);
    m_pSTC->EnsureCaretVisible();
    m_pSTC->ScrollToColumn(0);
}

int FindBar::CountMatches(const wxString &sText)
{
    if (sText.empty())
    {
        return 0;
    }

    int iCount = 0;
    int iPos = 0;
    while (true)
    {
        int iFound = m_pSTC->FindText(iPos, m_pSTC->GetTextLength(), sText, 0);
        if (iFound == wxNOT_FOUND)
        {
            break;
        }
        iCount++;
        iPos = iFound + 1;
    }
    return iCount;
}
