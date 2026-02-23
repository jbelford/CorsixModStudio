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
#include <wx/stattext.h>

/// LSP server status for display purposes.
enum class ELspStatus
{
    Idle,        ///< No LSP client created yet
    Starting,    ///< Client exists, server initializing
    Ready,       ///< Server initialized and ready
    Unavailable, ///< Server binary not found or initialization failed
    Disabled,    ///< User disabled LSP via settings
};

/// A small panel that displays a colored dot and text label
/// indicating the current LSP server status. Designed to be
/// positioned over a wxStatusBar field via GetFieldRect().
class CLspStatusPanel : public wxPanel
{
  public:
    CLspStatusPanel(wxWindow *parent, wxWindowID id = wxID_ANY);

    /// Update the displayed status. Only repaints if the state changed.
    void SetStatus(ELspStatus eStatus);

    /// Get the current displayed status.
    [[nodiscard]] ELspStatus GetStatus() const { return m_eStatus; }

  private:
    void OnPaint(wxPaintEvent &event);

    ELspStatus m_eStatus = ELspStatus::Idle;
    wxStaticText *m_pLabel = nullptr;

    static constexpr int kDotSize = 8;
    static constexpr int kDotMargin = 6;

    wxDECLARE_EVENT_TABLE();
};
