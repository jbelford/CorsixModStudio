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

#include "CScarEditorPresenter.h"
#include <algorithm>
#include <cstring>

CScarEditorPresenter::CScarEditorPresenter(IScarEditorView &view) : m_view(view) {}

std::vector<CScarEditorPresenter::FunctionDef>
CScarEditorPresenter::ParseFunctionDefinitions(const wxString &sSourceText)
{
    std::vector<FunctionDef> aResult;
    const wchar_t *pText = sSourceText.c_str();
    const wchar_t *pRoot = pText;

    pText = wcsstr(pText, L"function");
    while (pText != nullptr)
    {
        const wchar_t *pFuncKeyword = pText;
        pText += 8; // skip "function"

        // Skip whitespace after "function"
        while (*pText == ' ' || *pText == '\t' || *pText == '\r' || *pText == '\n')
            ++pText;

        // Find the opening parenthesis
        const wchar_t *pEnd = wcschr(pText, '(');
        if (pEnd != nullptr)
        {
            // Trim trailing whitespace before '('
            const wchar_t *pNameEnd = pEnd - 1;
            while (pNameEnd > pText &&
                   (*pNameEnd == ' ' || *pNameEnd == '\t' || *pNameEnd == '\r' || *pNameEnd == '\n'))
                --pNameEnd;

            if (pNameEnd >= pText)
            {
                // Validate: no spaces, newlines, or dashes in the name
                bool bValid = true;
                for (const wchar_t *pTest = pText; pTest <= pNameEnd; ++pTest)
                {
                    if (*pTest == ' ' || *pTest == '\r' || *pTest == '\n' || *pTest == '-')
                    {
                        bValid = false;
                        break;
                    }
                }

                if (bValid)
                {
                    FunctionDef def;
                    def.sName = wxString(pText, static_cast<std::size_t>(1 + (pNameEnd - pText)));
                    def.iPosition = static_cast<int>(pFuncKeyword - pRoot);
                    aResult.push_back(std::move(def));
                }
            }
        }

        pText = wcsstr(pText, L"function");
    }

    // Sort alphabetically by name
    std::sort(aResult.begin(), aResult.end(),
              [](const FunctionDef &a, const FunctionDef &b) { return a.sName < b.sName; });

    return aResult;
}

bool CScarEditorPresenter::ParseLuaError(const char *sRawError, LuaError &oError)
{
    if (sRawError == nullptr)
        return false;

    // Skip to first ':'
    const char *p = std::strchr(sRawError, ':');
    if (p == nullptr)
        return false;
    ++p;

    // Parse line number
    unsigned long iLine = 0;
    while (*p != '\0' && *p != ':')
    {
        if (*p >= '0' && *p <= '9')
        {
            iLine *= 10;
            iLine += static_cast<unsigned long>(*p - '0');
        }
        ++p;
    }

    if (*p == ':')
        ++p; // skip the second ':'

    oError.iLine = iLine;
    oError.sMessage = wxString::FromUTF8(p);
    return true;
}
