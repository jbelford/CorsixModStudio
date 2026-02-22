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

#include "ScarSyntaxConfig.h"
#include "common/ThemeColours.h"
#include "common/Utility.h"

// ---------------------------------------------------------------------------
// Style table
// ---------------------------------------------------------------------------

const StyleInfo g_StylePrefs[] = {
    // wxSTC_LUA_DEFAULT
    {_T("Default"), _T("BLACK"), _T("WHITE"), _T(""), 10, 0, 0},

    // wxSTC_LUA_COMMENT
    {_T("Comment"), _T("FOREST GREEN"), _T("WHITE"), _T(""), 10, 0, 0},

    // wxSTC_LUA_COMMENTLINE
    {_T("Comment line"), _T("FOREST GREEN"), _T("WHITE"), _T(""), 10, 0, 0},

    // wxSTC_LUA_COMMENTDOC
    {_T("Comment (Doc)"), _T("FOREST GREEN"), _T("WHITE"), _T(""), 10, 0, 0},

    // wxSTC_LUA_NUMBER
    {_T("Number"), _T("BLACK"), _T("WHITE"), _T(""), 10, 0, 0},

    // wxSTC_LUA_WORD
    {_T("Keyword1"), _T("BLUE"), _T("WHITE"), _T(""), 10, mySTC_STYLE_BOLD, 0},

    // wxSTC_LUA_STRING
    {_T("String"), _T("PURPLE"), _T("WHITE"), _T(""), 10, 0, 0},

    // wxSTC_LUA_CHARACTER
    {_T("Character"), _T("GOLD"), _T("WHITE"), _T(""), 10, 0, 0},

    // wxSTC_LUA_LITERALSTRING
    {_T("Literal String"), _T("PURPLE"), _T("WHITE"), _T(""), 10, 0, 0},

    // wxSTC_LUA_PREPROCESSOR (n/a)
    {_T("Preprocessor"), _T("GREY"), _T("WHITE"), _T(""), 10, 0, 0},

    // wxSTC_LUA_OPERATOR
    {_T("Operator"), _T("RED"), _T("WHITE"), _T(""), 10, mySTC_STYLE_BOLD, 0},

    // wxSTC_LUA_IDENTIFIER
    {_T("Identifier"), _T("BLACK"), _T("WHITE"), _T(""), 10, 0, 0},

    // wxSTC_LUA_STRINGEOL
    {_T("String (EOL)"), _T("PURPLE"), _T("WHITE"), _T(""), 10, 0, 0},

    // mySTC_TYPE_WORD2
    {_T("Keyword2"), _T("MEDIUM BLUE"), _T("WHITE"), _T(""), 10, mySTC_STYLE_BOLD, 0},

    // mySTC_TYPE_WORD3
    {_T("Keyword3"), _T("TAN"), _T("WHITE"), _T(""), 10, 0, 0},

    // mySTC_TYPE_WORD4
    {_T("Keyword4"), _T("FIREBRICK"), _T("WHITE"), _T(""), 10, 0, 0},

    // mySTC_TYPE_WORD5
    {_T("Keyword5"), _T("DARK GREY"), _T("WHITE"), _T(""), 10, 0, 0},

    // mySTC_TYPE_WORD6
    {_T("Keyword6"), _T("GREY"), _T("WHITE"), _T(""), 10, 0, 0},

    // mySTC_TYPE_WORD7
    {_T("Keyword7"), _T("GREY"), _T("WHITE"), _T(""), 10, 0, 0},

    // mySTC_TYPE_WORD8
    {_T("Keyword8"), _T("GREY"), _T("WHITE"), _T(""), 10, 0, 0}};

// ---------------------------------------------------------------------------
// Keyword / highlight-set mapping
// ---------------------------------------------------------------------------

HightlightSet g_LuaWords[] = {{wxSTC_LUA_DEFAULT, 0},
                              {wxSTC_LUA_COMMENTLINE, 0},
                              {wxSTC_LUA_COMMENTDOC, 0},
                              {wxSTC_LUA_NUMBER, 0},
                              {wxSTC_LUA_STRING, 0},
                              {wxSTC_LUA_CHARACTER, 0},
                              {wxSTC_LUA_LITERALSTRING, 0},
                              {wxSTC_LUA_PREPROCESSOR, 0},
                              {wxSTC_LUA_OPERATOR, 0},
                              {wxSTC_LUA_IDENTIFIER, 0},
                              {wxSTC_LUA_STRINGEOL, 0},
                              {wxSTC_LUA_WORD, _T("true false nil")},
                              {wxSTC_LUA_WORD2,
                               _T("and break do else elseif end false for function if in local nil not or repeat ")
                               _T("return then true until while _G")},
                              // WORD3 - API Functions
                              {wxSTC_LUA_WORD3, 0},
                              // WORD4 - API Constants
                              {wxSTC_LUA_WORD4, 0},
                              // WORD5 to WORD8 unused
                              {wxSTC_LUA_WORD5, 0},
                              {wxSTC_LUA_WORD6, 0},
                              {wxSTC_LUA_WORD7, 0},
                              {wxSTC_LUA_WORD8, 0},
                              {-1, 0}};

// ---------------------------------------------------------------------------
// Autocomplete XPM icons
// ---------------------------------------------------------------------------

// clang-format off

// 16x16 XPM icon: blue "K" for Lua keywords
const char *const g_xpmKeyword[] = {
    "16 16 3 1",
    "  c None",
    ". c #3465A4",
    "x c #FFFFFF",
    "                ",
    "  ....          ",
    "  .xx.          ",
    "  .xx.   ...    ",
    "  .xx.  .xx.    ",
    "  .xx. .xx.     ",
    "  .xx..xx.      ",
    "  .xxxxx.       ",
    "  .xxxxx.       ",
    "  .xx..xx.      ",
    "  .xx. .xx.     ",
    "  .xx.  .xx.    ",
    "  .xx.   ...    ",
    "  ....          ",
    "                ",
    "                "};

// 16x16 XPM icon: purple "F" for API functions
const char *const g_xpmFunction[] = {
    "16 16 3 1",
    "  c None",
    ". c #75507B",
    "x c #FFFFFF",
    "                ",
    "  .........     ",
    "  .xxxxxxx.     ",
    "  .xx.....      ",
    "  .xx.          ",
    "  .xx.          ",
    "  .xxxxxxx.     ",
    "  .xxxxxxx.     ",
    "  .xx.          ",
    "  .xx.          ",
    "  .xx.          ",
    "  .xx.          ",
    "  .xx.          ",
    "  ....          ",
    "                ",
    "                "};

// 16x16 XPM icon: firebrick "C" for constants
const char *const g_xpmConstant[] = {
    "16 16 3 1",
    "  c None",
    ". c #B22222",
    "x c #FFFFFF",
    "                ",
    "    ......      ",
    "   .xxxxxx.     ",
    "  .xx....xx.    ",
    "  .xx.  ....    ",
    "  .xx.          ",
    "  .xx.          ",
    "  .xx.          ",
    "  .xx.          ",
    "  .xx.          ",
    "  .xx.  ....    ",
    "  .xx....xx.    ",
    "   .xxxxxx.     ",
    "    ......      ",
    "                ",
    "                "};

// 16x16 XPM icon: green "L" for local function definitions
const char *const g_xpmLocalFunc[] = {
    "16 16 3 1",
    "  c None",
    ". c #4E9A06",
    "x c #FFFFFF",
    "                ",
    "  ....          ",
    "  .xx.          ",
    "  .xx.          ",
    "  .xx.          ",
    "  .xx.          ",
    "  .xx.          ",
    "  .xx.          ",
    "  .xx.          ",
    "  .xx.          ",
    "  .xx.          ",
    "  .xx.          ",
    "  .xxxxxxxx.    ",
    "  .xxxxxxxx.    ",
    "  ..........    ",
    "                "};

// clang-format on

// ---------------------------------------------------------------------------
// ApplyScarSyntaxConfig
// ---------------------------------------------------------------------------

void ApplyScarSyntaxConfig(wxStyledTextCtrl *pSTC)
{
    pSTC->StyleClearAll();
    pSTC->SetTabWidth(4);
    pSTC->SetLexer(wxSTC_LEX_LUA);
    pSTC->SetProperty(wxT("fold.compact"), wxT("0"));
    pSTC->SetViewEOL(false);
    pSTC->SetIndentationGuides(true);
    pSTC->SetEdgeMode(wxSTC_EDGE_NONE);
    pSTC->SetViewWhiteSpace(wxSTC_WS_INVISIBLE);
    pSTC->SetOvertype(false);
    pSTC->SetReadOnly(false);
    pSTC->SetWrapMode(wxSTC_WRAP_NONE);
    wxFont font = MakeCodeFont(10);
    pSTC->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
    pSTC->SetUseTabs(true);
    pSTC->SetTabIndents(true);
    pSTC->SetBackSpaceUnIndents(true);
    pSTC->SetIndent(4);

    for (int i = 0; i < wxSTC_STYLE_LASTPREDEFINED; ++i)
    {
        pSTC->StyleSetFont(i, font);
    }

    int iWordSet = 0;
    for (int i = 0; g_LuaWords[i].type != -1; ++i)
    {
        const StyleInfo &curType = g_StylePrefs[g_LuaWords[i].type];
        wxFont styleFont = MakeCodeFont(curType.fontsize, (curType.fontstyle & mySTC_STYLE_BOLD) != 0,
                                        (curType.fontstyle & mySTC_STYLE_ITALIC) != 0);
        pSTC->StyleSetFont(g_LuaWords[i].type, styleFont);
        pSTC->StyleSetUnderline(g_LuaWords[i].type, (curType.fontstyle & mySTC_STYLE_UNDERL) > 0);
        pSTC->StyleSetVisible(g_LuaWords[i].type, (curType.fontstyle & mySTC_STYLE_HIDDEN) == 0);
        pSTC->StyleSetCase(g_LuaWords[i].type, curType.lettercase);
        if (g_LuaWords[i].words)
        {
            pSTC->SetKeyWords(iWordSet++, g_LuaWords[i].words);
        }
    }

    ThemeColours::ApplyEditorTheme(pSTC);

    // Autocomplete settings
    pSTC->AutoCompSetIgnoreCase(true);
    pSTC->AutoCompSetAutoHide(true);
    pSTC->AutoCompSetChooseSingle(false);
    pSTC->AutoCompSetMaxHeight(15);
    pSTC->AutoCompSetMaxWidth(40);

    // Register autocomplete type icons
    pSTC->RegisterImage(ACT_Keyword, g_xpmKeyword);
    pSTC->RegisterImage(ACT_Function, g_xpmFunction);
    pSTC->RegisterImage(ACT_Constant, g_xpmConstant);
    pSTC->RegisterImage(ACT_LocalFunc, g_xpmLocalFunc);

    // LSP diagnostic indicator styles
    constexpr int INDICATOR_LSP_ERROR = 8;
    constexpr int INDICATOR_LSP_WARNING = 9;
    constexpr int INDICATOR_LSP_INFO = 10;
    pSTC->IndicatorSetStyle(INDICATOR_LSP_ERROR, wxSTC_INDIC_SQUIGGLE);
    pSTC->IndicatorSetForeground(INDICATOR_LSP_ERROR, wxColour(255, 0, 0));
    pSTC->IndicatorSetStyle(INDICATOR_LSP_WARNING, wxSTC_INDIC_SQUIGGLE);
    pSTC->IndicatorSetForeground(INDICATOR_LSP_WARNING, wxColour(200, 150, 0));
    pSTC->IndicatorSetStyle(INDICATOR_LSP_INFO, wxSTC_INDIC_DOTS);
    pSTC->IndicatorSetForeground(INDICATOR_LSP_INFO, wxColour(100, 100, 200));

    // Enable mouse hover (dwell) events for diagnostic tooltips
    pSTC->SetMouseDwellTime(500);
}
