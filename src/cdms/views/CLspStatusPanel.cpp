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

#include "CLspStatusPanel.h"
#include <wx/dcbuffer.h>
#include <wx/settings.h>
#include <wx/sizer.h>

wxBEGIN_EVENT_TABLE(CLspStatusPanel, wxPanel) EVT_PAINT(CLspStatusPanel::OnPaint) wxEND_EVENT_TABLE()

    CLspStatusPanel::CLspStatusPanel(wxWindow *parent, wxWindowID id)
    : wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxNO_BORDER)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    auto *pSizer = new wxBoxSizer(wxHORIZONTAL);

    // Reserve space for the painted dot
    pSizer->AddSpacer(kDotMargin + kDotSize + kDotMargin);

    m_pLabel = new wxStaticText(this, wxID_ANY, wxT("LSP: Idle"));
    pSizer->Add(m_pLabel, 1, wxALIGN_CENTER_VERTICAL);

    SetSizer(pSizer);
}

void CLspStatusPanel::SetStatus(ELspStatus eStatus)
{
    if (eStatus == m_eStatus)
    {
        return;
    }
    m_eStatus = eStatus;

    wxString sLabel;
    switch (eStatus)
    {
    case ELspStatus::Idle:
        sLabel = wxT("LSP: Idle");
        break;
    case ELspStatus::Starting:
        sLabel = wxT("LSP: Starting\u2026");
        break;
    case ELspStatus::Ready:
        sLabel = wxT("LSP: Ready");
        break;
    case ELspStatus::Unavailable:
        sLabel = wxT("LSP: Unavailable");
        break;
    case ELspStatus::Disabled:
        sLabel = wxT("LSP: Disabled");
        break;
    }

    m_pLabel->SetLabel(sLabel);
    Refresh();
}

void CLspStatusPanel::OnPaint(wxPaintEvent &event)
{
    wxAutoBufferedPaintDC dc(this);

    // Fill background to match the status bar
    dc.SetBackground(wxBrush(GetParent()->GetBackgroundColour()));
    dc.Clear();

    // Choose dot color based on status
    wxColour dotColour;
    switch (m_eStatus)
    {
    case ELspStatus::Idle:
    case ELspStatus::Disabled:
        dotColour = wxColour(160, 160, 160); // Gray
        break;
    case ELspStatus::Starting:
        dotColour = wxColour(220, 180, 50); // Yellow/amber
        break;
    case ELspStatus::Ready:
        dotColour = wxColour(80, 190, 80); // Green
        break;
    case ELspStatus::Unavailable:
        dotColour = wxColour(220, 60, 60); // Red
        break;
    }

    dc.SetBrush(wxBrush(dotColour));
    dc.SetPen(*wxTRANSPARENT_PEN);

    int y = (GetSize().GetHeight() - kDotSize) / 2;
    dc.DrawEllipse(kDotMargin, y, kDotSize, kDotSize);
}
