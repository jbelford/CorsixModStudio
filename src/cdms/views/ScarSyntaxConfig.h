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

#include <wx/stc/stc.h>
#include <wx/defs.h>

// Syntax style flags used by StyleInfo.
#define mySTC_STYLE_BOLD 1
#define mySTC_STYLE_ITALIC 2
#define mySTC_STYLE_UNDERL 4
#define mySTC_STYLE_HIDDEN 8

/// Description of a single syntax-highlighting style.
struct StyleInfo
{
    const wxChar *name;
    const wxChar *foreground;
    const wxChar *background;
    const wxChar *fontname;
    int fontsize;
    int fontstyle;
    int lettercase;
};

/// Mapping from Scintilla style IDs to keyword sets.
struct HightlightSet
{
    int type;
    const wxChar *words;
};

/// Autocomplete icon type IDs registered with wxStyledTextCtrl.
enum AutoCompType
{
    ACT_Keyword = 0,
    ACT_Function = 1,
    ACT_Constant = 2,
    ACT_LocalFunc = 3
};

/// Global Lua syntax style table (indexed by Scintilla style ID).
extern const StyleInfo g_StylePrefs[];

/// Global Lua keyword/style mapping table (terminated by type == -1).
extern HightlightSet g_LuaWords[];

/// XPM icon data for autocomplete type icons.
extern const char *const g_xpmKeyword[];
extern const char *const g_xpmFunction[];
extern const char *const g_xpmConstant[];
extern const char *const g_xpmLocalFunc[];

/// Configure a wxStyledTextCtrl for Lua/SCAR editing.
/// Sets up the lexer, styles, fonts, keywords, indicators, autocomplete
/// icons, and theme colours. Does NOT set keyword sets 2+ (SCAR API
/// functions/constants) — the caller must do that after loading ref data.
void ApplyScarSyntaxConfig(wxStyledTextCtrl *pSTC);
