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

#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stc/stc.h>
#include <wx/propgrid/propgrid.h>

/// Centralized theme-aware colour provider for CDMS.
/// All colours adapt automatically based on whether the system is in dark mode.
namespace ThemeColours
{

/// Returns true when the current system appearance is dark.
inline bool IsDarkMode()
{
    const wxColour bg = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
    // Luminance check: dark backgrounds have low perceived brightness
    const int luminance = (bg.Red() * 299 + bg.Green() * 587 + bg.Blue() * 114) / 1000;
    return luminance < 128;
}

// ── Editor background / foreground ──────────────────────────────────────

inline wxColour EditorBg() { return IsDarkMode() ? wxColour(30, 30, 30) : *wxWHITE; }

inline wxColour EditorFg() { return IsDarkMode() ? wxColour(212, 212, 212) : *wxBLACK; }

inline wxColour EditorLineNumFg() { return IsDarkMode() ? wxColour(133, 133, 133) : wxColour(_T("DARK GREY")); }

inline wxColour EditorLineNumBg() { return IsDarkMode() ? wxColour(45, 45, 45) : wxColour(_T("WHEAT")); }

inline wxColour EditorIndentGuide() { return IsDarkMode() ? wxColour(64, 64, 64) : wxColour(_T("DARK GREY")); }

// ── CallTip colours ─────────────────────────────────────────────────────

inline wxColour CallTipBg() { return IsDarkMode() ? wxColour(45, 45, 48) : wxColour(_T("LIGHT BLUE")); }

inline wxColour CallTipFg() { return IsDarkMode() ? wxColour(212, 212, 212) : *wxBLACK; }

// ── Syntax highlighting foreground colours ──────────────────────────────

inline wxColour CommentFg() { return IsDarkMode() ? wxColour(106, 153, 85) : wxColour(_T("FOREST GREEN")); }

inline wxColour NumberFg() { return IsDarkMode() ? wxColour(181, 206, 168) : *wxBLACK; }

inline wxColour Keyword1Fg() { return IsDarkMode() ? wxColour(86, 156, 214) : wxColour(_T("BLUE")); }

inline wxColour StringFg() { return IsDarkMode() ? wxColour(206, 145, 120) : wxColour(_T("PURPLE")); }

inline wxColour CharacterFg() { return IsDarkMode() ? wxColour(215, 186, 125) : wxColour(_T("GOLD")); }

inline wxColour OperatorFg() { return IsDarkMode() ? wxColour(212, 212, 212) : wxColour(_T("RED")); }

inline wxColour IdentifierFg() { return IsDarkMode() ? wxColour(156, 220, 254) : *wxBLACK; }

inline wxColour Keyword2Fg() { return IsDarkMode() ? wxColour(78, 201, 176) : wxColour(_T("MEDIUM BLUE")); }

inline wxColour Keyword3Fg() { return IsDarkMode() ? wxColour(220, 220, 170) : wxColour(_T("TAN")); }

inline wxColour Keyword4Fg() { return IsDarkMode() ? wxColour(244, 71, 71) : wxColour(_T("FIREBRICK")); }

inline wxColour Keyword5Fg() { return IsDarkMode() ? wxColour(128, 128, 128) : wxColour(_T("DARK GREY")); }

inline wxColour PreprocessorFg() { return IsDarkMode() ? wxColour(155, 155, 155) : wxColour(_T("GREY")); }

inline wxColour LiteralStringFg() { return StringFg(); }

inline wxColour StringEolFg() { return StringFg(); }

inline wxColour UnusedKeywordFg() { return IsDarkMode() ? wxColour(155, 155, 155) : wxColour(_T("GREY")); }

// ── File tree item colours ──────────────────────────────────────────────

inline wxColour FileThisMod() { return IsDarkMode() ? wxColour(255, 130, 130) : wxColour(128, 64, 64); }

inline wxColour FileOtherMod() { return IsDarkMode() ? wxColour(140, 140, 255) : wxColour(64, 64, 128); }

inline wxColour FileEngine() { return IsDarkMode() ? wxColour(180, 180, 180) : wxColour(128, 128, 128); }

// ── PropertyGrid colours ────────────────────────────────────────────────

inline wxColour PropGridBg()
{
    return IsDarkMode() ? wxColour(30, 30, 30) : wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
}

inline wxColour PropGridFg()
{
    return IsDarkMode() ? wxColour(230, 230, 230) : wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
}

inline wxColour PropGridCaptionBg()
{
    return IsDarkMode() ? wxColour(45, 45, 48) : wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION);
}

inline wxColour PropGridCaptionFg()
{
    return IsDarkMode() ? wxColour(230, 230, 230) : wxSystemSettings::GetColour(wxSYS_COLOUR_CAPTIONTEXT);
}

inline wxColour PropGridMarginColour()
{
    return IsDarkMode() ? wxColour(45, 45, 45) : wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
}

inline wxColour PropGridLineFg()
{
    return IsDarkMode() ? wxColour(60, 60, 60) : wxSystemSettings::GetColour(wxSYS_COLOUR_BTNSHADOW);
}

inline wxColour PropGridSelectionBg()
{
    return IsDarkMode() ? wxColour(38, 79, 120) : wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT);
}

inline wxColour PropGridSelectionFg()
{
    return IsDarkMode() ? wxColour(255, 255, 255) : wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT);
}

inline wxColour PropGridCellDisabledFg()
{
    return IsDarkMode() ? wxColour(128, 128, 128) : wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT);
}

// ── Dialog colours ──────────────────────────────────────────────────────

inline wxColour DialogBg() { return wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW); }

inline wxColour DialogFg() { return wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT); }

inline wxColour CancelBtnBg() { return DialogBg(); }

inline wxColour CancelBtnFg() { return IsDarkMode() ? wxColour(180, 180, 180) : wxColour(100, 100, 100); }

inline wxColour CancelBtnHoverBg() { return wxColour(232, 17, 35); }

inline wxColour CancelBtnHoverFg() { return *wxWHITE; }

// ── Shared editor theme application ─────────────────────────────────────

/// Apply the full colour theme to a wxStyledTextCtrl configured for Lua syntax.
/// Call this after the STC lexer, fonts, and keywords are set up.
void ApplyEditorTheme(wxStyledTextCtrl *pSTC);

/// Apply theme-aware colours to a wxPropertyGrid.
/// Call this after constructing the property grid.
void ApplyPropertyGridTheme(wxPropertyGrid *pPG);

/// Theme-aware replacement for wxMessageBox.
/// Uses wxGenericMessageDialog so the dialog content area respects dark mode
/// (native Win32 TaskDialog only darkens the title bar).
/// Return value matches wxMessageBox: wxOK, wxYES, wxNO, wxCANCEL, wxHELP.
int ShowMessageBox(const wxString &message, const wxString &caption = wxEmptyString, long style = wxOK,
                   wxWindow *parent = nullptr);

} // namespace ThemeColours
