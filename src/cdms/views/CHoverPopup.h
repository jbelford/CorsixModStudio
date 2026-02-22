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

#include <lsp/Protocol.h>
#include <string>
#include <vector>
#include <wx/popupwin.h>

/// A single diagnostic entry formatted for display in the hover popup.
struct DiagnosticEntry
{
    lsp::DiagnosticSeverity severity;
    std::string text; // Pre-formatted: "Error: message [source]"
};

/// Parsed components of a LuaLS hover markdown response.
struct HoverParts
{
    std::string code;                         // Function signature (from code fences)
    std::string description;                  // Description text (after ---)
    std::vector<DiagnosticEntry> diagnostics; // Diagnostics at this position
};

/// Parse LuaLS hover markdown into code signature and description parts.
HoverParts ParseHoverMarkdown(const std::string &md);

/// Rich hover popup window with syntax-highlighted code and description text.
/// Displays LSP hover information and/or diagnostics at a given screen position.
class CHoverPopup : public wxPopupTransientWindow
{
  public:
    /// Construct and position the popup.
    /// @param parent         Parent window (typically the editor).
    /// @param parts          Parsed hover content to display.
    /// @param position       Screen position for the popup's top-left corner.
    /// @param cursorLineHeight  Height of a text line in the editor (for flip-above logic).
    CHoverPopup(wxWindow *parent, const HoverParts &parts, const wxPoint &position, int cursorLineHeight);
};
