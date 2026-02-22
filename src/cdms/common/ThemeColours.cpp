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

#include "ThemeColours.h"
#include "Common.h"
#include <wx/msgdlg.h>
#include <wx/generic/msgdlgg.h>
#include <wx/app.h>

namespace ThemeColours
{

/// Mapping from Scintilla Lua style IDs to ThemeColours accessors.
struct StyleMapping
{
    int iStyleId;
    wxColour (*fnForeground)();
};

static const StyleMapping g_StyleMappings[] = {
    {wxSTC_LUA_DEFAULT, EditorFg},
    {wxSTC_LUA_COMMENT, CommentFg},
    {wxSTC_LUA_COMMENTLINE, CommentFg},
    {wxSTC_LUA_COMMENTDOC, CommentFg},
    {wxSTC_LUA_NUMBER, NumberFg},
    {wxSTC_LUA_WORD, Keyword1Fg},
    {wxSTC_LUA_STRING, StringFg},
    {wxSTC_LUA_CHARACTER, CharacterFg},
    {wxSTC_LUA_LITERALSTRING, LiteralStringFg},
    {wxSTC_LUA_PREPROCESSOR, PreprocessorFg},
    {wxSTC_LUA_OPERATOR, OperatorFg},
    {wxSTC_LUA_IDENTIFIER, IdentifierFg},
    {wxSTC_LUA_STRINGEOL, StringEolFg},
    {wxSTC_LUA_WORD2, Keyword2Fg},
    {wxSTC_LUA_WORD3, Keyword3Fg},
    {wxSTC_LUA_WORD4, Keyword4Fg},
    {wxSTC_LUA_WORD5, Keyword5Fg},
    {wxSTC_LUA_WORD6, UnusedKeywordFg},
    {wxSTC_LUA_WORD7, UnusedKeywordFg},
    {wxSTC_LUA_WORD8, UnusedKeywordFg},
};

void ApplyEditorTheme(wxStyledTextCtrl *pSTC)
{
    const wxColour bg = EditorBg();

    // Default style
    pSTC->StyleSetBackground(wxSTC_STYLE_DEFAULT, bg);
    pSTC->StyleSetForeground(wxSTC_STYLE_DEFAULT, EditorFg());

    // Line numbers
    pSTC->StyleSetForeground(wxSTC_STYLE_LINENUMBER, EditorLineNumFg());
    pSTC->StyleSetBackground(wxSTC_STYLE_LINENUMBER, EditorLineNumBg());

    // Indent guides
    pSTC->StyleSetForeground(wxSTC_STYLE_INDENTGUIDE, EditorIndentGuide());

    // Caret colour — ensure it's visible against the editor background
    pSTC->SetCaretForeground(EditorFg());

    // Per-style foreground and background
    for (const auto &mapping : g_StyleMappings)
    {
        pSTC->StyleSetForeground(mapping.iStyleId, mapping.fnForeground());
        pSTC->StyleSetBackground(mapping.iStyleId, bg);
    }
}

int ShowMessageBox(const wxString &message, const wxString &caption, long style, wxWindow *parent)
{
    if (!parent)
    {
        parent = wxTheApp ? wxTheApp->GetTopWindow() : nullptr;
    }

    wxGenericMessageDialog dlg(parent, message, caption, style);
    const int ans = dlg.ShowModal();

    // Translate wxID_* return values to wxMessageBox-style wxOK/wxYES/wxNO/etc.
    switch (ans)
    {
    case wxID_OK:
        return wxOK;
    case wxID_YES:
        return wxYES;
    case wxID_NO:
        return wxNO;
    case wxID_CANCEL:
        return wxCANCEL;
    case wxID_HELP:
        return wxHELP;
    default:
        return wxCANCEL;
    }
}

} // namespace ThemeColours
