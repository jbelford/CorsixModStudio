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

#include "views/interfaces/IScarEditorView.h"
#include <wx/string.h>
#include <vector>

//! Presenter for the SCAR/Lua script editor.
/*!
    Extracts business logic from frmScarEditor: function definition parsing
    from source text and Lua error message parsing for syntax checking.
    All methods are pure functions that operate on provided data.
*/
class CScarEditorPresenter
{
  public:
    //! A function definition found in source text.
    struct FunctionDef
    {
        wxString sName;    //!< Function name (e.g. "MyFunction").
        int iPosition = 0; //!< Character offset in source text.
    };

    explicit CScarEditorPresenter(IScarEditorView &view);

    //! Parse function definitions from Lua/SCAR source text.
    /*!
        Scans the text for `function Name(` patterns and extracts the function
        names and their character positions. Results are sorted alphabetically.

        \param sSourceText  The full source text to scan.
        \return Sorted list of function definitions found.
    */
    [[nodiscard]] static std::vector<FunctionDef> ParseFunctionDefinitions(const wxString &sSourceText);

    //! Parsed result from a Lua syntax check error string.
    struct LuaError
    {
        unsigned long iLine = 0; //!< 1-based line number where the error occurred.
        wxString sMessage;       //!< Error message text.
    };

    //! Parse a Lua error string into line number and message.
    /*!
        Lua error strings have the format ":line:message".
        This extracts the line number and the message portion.

        \param sRawError  Raw error string from lua_tolstring (e.g. ":42: unexpected symbol").
        \param[out] oError  Parsed error on success.
        \return true if parsing succeeded, false on malformed input.
    */
    [[nodiscard]] static bool ParseLuaError(const char *sRawError, LuaError &oError);

  private:
    IScarEditorView &m_view;
};
