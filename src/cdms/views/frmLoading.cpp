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

#include "frmLoading.h"
#include "resource.h"
#include "common/Common.h"

BEGIN_EVENT_TABLE(frmLoading, wxFrame)
EVT_CLOSE(frmLoading::OnQuit)
EVT_PAINT(frmLoading::OnPaint)
EVT_BUTTON(wxID_CANCEL, frmLoading::OnCancel)
END_EVENT_TABLE()

frmLoading::frmLoading(const wxString &sTitle)
    : wxFrame(wxTheApp->GetTopWindow(), -1, sTitle, wxPoint(0, 0),
              wxWindow::FromDIP(wxSize(384, 420), wxTheApp->GetTopWindow()),
              wxFRAME_FLOAT_ON_PARENT | wxFRAME_TOOL_WINDOW)
{
    CentreOnParent();
    m_pLoadingImage = nullptr;
    m_pText = nullptr;
    m_pCancelButton = nullptr;

    m_pLoadingImage = new wxBitmap(wxT("RIDB_LOADING"), wxBITMAP_TYPE_BMP_RESOURCE);
    const wxSize originalSize(m_pLoadingImage->GetWidth(), m_pLoadingImage->GetHeight());
    const wxSize scaledSize = FromDIP(originalSize);
    if (scaledSize != originalSize)
    {
        wxImage scaledImage = m_pLoadingImage->ConvertToImage();
        if (scaledImage.IsOk())
        {
            scaledImage.Rescale(scaledSize.GetWidth(), scaledSize.GetHeight(), wxIMAGE_QUALITY_HIGH);
            *m_pLoadingImage = wxBitmap(scaledImage);
        }
    }

    m_pText = new wxStaticText(this, -1, sTitle, FromDIP(wxPoint(0, 317)), FromDIP(wxSize(384, 33)),
                               wxST_NO_AUTORESIZE | wxALIGN_CENTER);
    m_pText->Wrap(FromDIP(380));
    m_pText->SetBackgroundStyle(wxBG_STYLE_COLOUR);
    m_pText->SetBackgroundColour(wxColour(255, 255, 255));
    wxFont f = m_pText->GetFont();
    f.SetWeight(wxFONTWEIGHT_BOLD);
    m_pText->SetFont(f);

    m_pCancelButton =
        new wxButton(this, wxID_CANCEL, wxT("Cancel"), FromDIP(wxPoint(142, 358)), FromDIP(wxSize(100, 28)));
    m_pCancelButton->Hide();
}

frmLoading::~frmLoading()
{
    delete m_pLoadingImage;
    m_pLoadingImage = nullptr;
}

void frmLoading::OnQuit(wxCloseEvent &event) { UNUSED(event); }

void frmLoading::OnPaint(wxPaintEvent &event)
{
    UNUSED(event);
    wxPaintDC dc(this);
    if (m_pLoadingImage)
    {
        wxMemoryDC temp_dc;
        temp_dc.SelectObject(*m_pLoadingImage);
        dc.Blit(0, 0, m_pLoadingImage->GetWidth(), m_pLoadingImage->GetHeight(), &temp_dc, 0, 0);
    }
}

void frmLoading::SetMessage(const wxString &sMsg) { m_pText->SetLabel(sMsg); }

void frmLoading::SetCancelCallback(std::function<void()> fnOnCancel)
{
    m_fnOnCancel = std::move(fnOnCancel);
    if (m_pCancelButton)
        m_pCancelButton->Show(m_fnOnCancel != nullptr);
}

void frmLoading::OnCancel(wxCommandEvent &event)
{
    UNUSED(event);
    if (m_fnOnCancel)
    {
        m_pCancelButton->Disable();
        m_fnOnCancel();
    }
}
