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

#include "CHoverPopup.h"
#include "ScarSyntaxConfig.h"
#include "common/ThemeColours.h"
#include "common/Utility.h"
#include <algorithm>
#include <wx/display.h>
#include <wx/stc/stc.h>

// ---------------------------------------------------------------------------
// ParseHoverMarkdown
// ---------------------------------------------------------------------------

HoverParts ParseHoverMarkdown(const std::string &md)
{
    HoverParts parts;
    bool inCodeBlock = false;
    bool pastSeparator = false;

    size_t i = 0;
    while (i < md.size())
    {
        size_t eol = md.find('\n', i);
        if (eol == std::string::npos)
        {
            eol = md.size();
        }

        std::string line = md.substr(i, eol - i);
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        // Code fence toggle
        if (line.size() >= 3 && line.substr(0, 3) == "```")
        {
            inCodeBlock = !inCodeBlock;
            i = eol + 1;
            continue;
        }

        if (inCodeBlock)
        {
            if (!parts.code.empty())
            {
                parts.code += "\n";
            }
            parts.code += line;
            i = eol + 1;
            continue;
        }

        // Horizontal rule marks the boundary between code and description
        bool isRule = !line.empty() && line.find_first_not_of("-* ") == std::string::npos &&
                      std::count(line.begin(), line.end(), '-') >= 3;
        if (isRule)
        {
            pastSeparator = true;
            i = eol + 1;
            continue;
        }

        // Skip blank lines before description starts
        size_t firstNonSpace = line.find_first_not_of(" \t");
        if (firstNonSpace == std::string::npos)
        {
            if (!parts.description.empty())
            {
                parts.description += "\n";
            }
            i = eol + 1;
            continue;
        }

        // Trim the line
        size_t lastNonSpace = line.find_last_not_of(" \t");
        line = line.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1);

        if (pastSeparator || !parts.code.empty())
        {
            if (!parts.description.empty())
            {
                parts.description += "\n";
            }
            parts.description += line;
        }
        else
        {
            // Text before any code block or separator goes to description
            if (!parts.description.empty())
            {
                parts.description += "\n";
            }
            parts.description += line;
        }

        i = eol + 1;
    }

    // Trim trailing whitespace from both parts
    while (!parts.code.empty() && (parts.code.back() == '\n' || parts.code.back() == ' '))
    {
        parts.code.pop_back();
    }
    while (!parts.description.empty() && (parts.description.back() == '\n' || parts.description.back() == ' '))
    {
        parts.description.pop_back();
    }

    return parts;
}

// ---------------------------------------------------------------------------
// CHoverPopup
// ---------------------------------------------------------------------------

CHoverPopup::CHoverPopup(wxWindow *parent, const HoverParts &parts, const wxPoint &position, int cursorLineHeight)
    : wxPopupTransientWindow(parent, wxBORDER_NONE)
{
    bool dark = ThemeColours::IsDarkMode();
    wxColour bgColour = dark ? wxColour(37, 37, 38) : wxColour(249, 249, 249);
    wxColour borderColour = dark ? wxColour(69, 69, 69) : wxColour(196, 196, 196);
    wxColour codeBg = dark ? wxColour(30, 30, 30) : wxColour(243, 243, 243);
    wxColour descFg = dark ? wxColour(204, 204, 204) : wxColour(51, 51, 51);

    SetBackgroundColour(bgColour);

    // Outer panel with border effect via nested sizers
    auto *pBorderPanel = new wxPanel(this, wxID_ANY);
    pBorderPanel->SetBackgroundColour(borderColour);
    auto *pInnerPanel = new wxPanel(pBorderPanel, wxID_ANY);
    pInnerPanel->SetBackgroundColour(bgColour);

    auto *pBorderSizer = new wxBoxSizer(wxVERTICAL);
    pBorderSizer->Add(pInnerPanel, 1, wxEXPAND | wxALL, 1);
    pBorderPanel->SetSizer(pBorderSizer);

    auto *pSizer = new wxBoxSizer(wxVERTICAL);

    // Scale max width relative to screen size (~40%, clamped to [400, 900])
    int displayIdx = wxDisplay::GetFromWindow(parent);
    if (displayIdx == wxNOT_FOUND)
    {
        displayIdx = 0;
    }
    wxDisplay display(displayIdx);
    wxRect screenRect = display.GetClientArea();
    const int kMaxWidth = std::clamp(screenRect.GetWidth() * 2 / 5, 400, 900);

    // --- Code section (syntax highlighted) ---
    if (!parts.code.empty())
    {
        auto *pCodeSTC = new wxStyledTextCtrl(pInnerPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                              wxBORDER_NONE | wxTE_MULTILINE | wxTE_READONLY);
        pCodeSTC->SetLexer(wxSTC_LEX_LUA);
        pCodeSTC->SetMarginWidth(0, 0);
        pCodeSTC->SetMarginWidth(1, 0);
        pCodeSTC->SetMarginWidth(2, 0);
        pCodeSTC->SetUseHorizontalScrollBar(false);
        pCodeSTC->SetUseVerticalScrollBar(false);
        pCodeSTC->SetWrapMode(wxSTC_WRAP_CHAR);
        pCodeSTC->SetScrollWidth(1);
        pCodeSTC->SetReadOnly(false);
        pCodeSTC->SetCaretWidth(0);

        wxFont codeFont = MakeCodeFont(9);
        pCodeSTC->StyleSetFont(wxSTC_STYLE_DEFAULT, codeFont);
        pCodeSTC->StyleClearAll();

        // Apply Lua syntax colours
        for (int i = 0; g_LuaWords[i].type != -1; ++i)
        {
            const StyleInfo &curType = g_StylePrefs[g_LuaWords[i].type];
            wxFont sf = MakeCodeFont(9, (curType.fontstyle & mySTC_STYLE_BOLD) != 0,
                                     (curType.fontstyle & mySTC_STYLE_ITALIC) != 0);
            pCodeSTC->StyleSetFont(g_LuaWords[i].type, sf);

            if (dark)
            {
                // Dark theme syntax colours
                switch (g_LuaWords[i].type)
                {
                case wxSTC_LUA_WORD:
                case wxSTC_LUA_WORD2:
                    pCodeSTC->StyleSetForeground(g_LuaWords[i].type, wxColour(86, 156, 214));
                    break;
                case wxSTC_LUA_STRING:
                case wxSTC_LUA_LITERALSTRING:
                case wxSTC_LUA_CHARACTER:
                    pCodeSTC->StyleSetForeground(g_LuaWords[i].type, wxColour(206, 145, 120));
                    break;
                case wxSTC_LUA_NUMBER:
                    pCodeSTC->StyleSetForeground(g_LuaWords[i].type, wxColour(181, 206, 168));
                    break;
                case wxSTC_LUA_OPERATOR:
                    pCodeSTC->StyleSetForeground(g_LuaWords[i].type, wxColour(212, 212, 212));
                    break;
                case wxSTC_LUA_COMMENT:
                case wxSTC_LUA_COMMENTLINE:
                case wxSTC_LUA_COMMENTDOC:
                    pCodeSTC->StyleSetForeground(g_LuaWords[i].type, wxColour(106, 153, 85));
                    break;
                default:
                    pCodeSTC->StyleSetForeground(g_LuaWords[i].type, wxColour(212, 212, 212));
                    break;
                }
            }
            else
            {
                pCodeSTC->StyleSetForeground(g_LuaWords[i].type, wxColour(curType.foreground));
            }
            pCodeSTC->StyleSetBackground(g_LuaWords[i].type, codeBg);

            int iWordSet = 0;
            if (g_LuaWords[i].words)
            {
                pCodeSTC->SetKeyWords(iWordSet++, g_LuaWords[i].words);
            }
        }

        // Set background for all default styles
        for (int i = 0; i <= wxSTC_STYLE_LASTPREDEFINED; ++i)
        {
            pCodeSTC->StyleSetBackground(i, codeBg);
            if (dark)
            {
                pCodeSTC->StyleSetForeground(i, wxColour(212, 212, 212));
            }
        }
        pCodeSTC->SetCaretForeground(codeBg);

        wxString codeText = wxString::FromUTF8(parts.code);
        pCodeSTC->SetText(codeText);
        pCodeSTC->SetReadOnly(true);
        pCodeSTC->Colourise(0, -1);

        // Size the STC to fit content, capped at kMaxWidth
        int lineCount = pCodeSTC->GetLineCount();
        int lineHeight = pCodeSTC->TextHeight(0);

        // Measure each line's pixel width for content-driven sizing and height
        int maxLineWidth = 0;
        for (int ln = 0; ln < lineCount; ++ln)
        {
            int w = pCodeSTC->TextWidth(wxSTC_STYLE_DEFAULT, pCodeSTC->GetLine(ln));
            if (w > maxLineWidth)
            {
                maxLineWidth = w;
            }
        }
        int codeWidth = std::min(maxLineWidth + 20, kMaxWidth);

        // Calculate wrapped height mathematically — WrapCount() is unreliable
        // on unrealized popup windows where Scintilla hasn't reflowed text yet
        int availWidth = std::max(codeWidth - 4, 1);
        int totalDisplayLines = 0;
        for (int ln = 0; ln < lineCount; ++ln)
        {
            int w = pCodeSTC->TextWidth(wxSTC_STYLE_DEFAULT, pCodeSTC->GetLine(ln));
            totalDisplayLines += std::max(1, (w + availWidth - 1) / availWidth);
        }
        int codeHeight = totalDisplayLines * lineHeight + 6;

        pCodeSTC->SetMinSize(wxSize(codeWidth, codeHeight));
        pSizer->Add(pCodeSTC, 0, wxEXPAND | wxALL, 6);
    }

    // --- Description section ---
    if (!parts.description.empty())
    {
        // Add a thin separator line if we have both code and description
        if (!parts.code.empty())
        {
            wxColour sepColour = dark ? wxColour(69, 69, 69) : wxColour(210, 210, 210);
            auto *pSep = new wxPanel(pInnerPanel, wxID_ANY, wxDefaultPosition, wxSize(-1, 1));
            pSep->SetBackgroundColour(sepColour);
            pSizer->Add(pSep, 0, wxEXPAND | wxLEFT | wxRIGHT, 8);
        }

        wxString descText = wxString::FromUTF8(parts.description);
        auto *pDesc =
            new wxStaticText(pInnerPanel, wxID_ANY, descText, wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);

        wxFont descFont(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Segoe UI"));
        if (!descFont.IsOk())
        {
            descFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
            descFont.SetPointSize(9);
        }
        pDesc->SetFont(descFont);
        pDesc->SetForegroundColour(descFg);
        pDesc->SetBackgroundColour(bgColour);
        pDesc->Wrap(kMaxWidth - 20);

        pSizer->Add(pDesc, 0, wxEXPAND | wxALL, 8);
    }

    // --- Diagnostics section ---
    if (!parts.diagnostics.empty())
    {
        bool hasPriorContent = !parts.code.empty() || !parts.description.empty();
        if (hasPriorContent)
        {
            wxColour sepColour = dark ? wxColour(69, 69, 69) : wxColour(210, 210, 210);
            auto *pSep = new wxPanel(pInnerPanel, wxID_ANY, wxDefaultPosition, wxSize(-1, 1));
            pSep->SetBackgroundColour(sepColour);
            pSizer->Add(pSep, 0, wxEXPAND | wxLEFT | wxRIGHT, 8);
        }

        wxFont diagFont(9, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Segoe UI"));
        if (!diagFont.IsOk())
        {
            diagFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
            diagFont.SetPointSize(9);
        }

        for (const auto &diag : parts.diagnostics)
        {
            wxColour diagFg;
            switch (diag.severity)
            {
            case lsp::DiagnosticSeverity::Error:
                diagFg = dark ? wxColour(244, 71, 71) : wxColour(205, 49, 49);
                break;
            case lsp::DiagnosticSeverity::Warning:
                diagFg = dark ? wxColour(206, 145, 40) : wxColour(191, 134, 0);
                break;
            case lsp::DiagnosticSeverity::Information:
            case lsp::DiagnosticSeverity::Hint:
                diagFg = dark ? wxColour(75, 154, 254) : wxColour(0, 122, 204);
                break;
            }

            wxString diagText = wxString::FromUTF8(diag.text);
            auto *pDiag =
                new wxStaticText(pInnerPanel, wxID_ANY, diagText, wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);
            pDiag->SetFont(diagFont);
            pDiag->SetForegroundColour(diagFg);
            pDiag->SetBackgroundColour(bgColour);
            pDiag->Wrap(kMaxWidth - 20);
            pSizer->Add(pDiag, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 8);
        }
        // Bottom margin after last diagnostic
        pSizer->AddSpacer(8);
    }

    pInnerPanel->SetSizer(pSizer);

    // Layout and size
    pInnerPanel->Layout();
    pSizer->Fit(pInnerPanel);
    pBorderPanel->Layout();
    pBorderSizer->Fit(pBorderPanel);

    auto *pOuterSizer = new wxBoxSizer(wxVERTICAL);
    pOuterSizer->Add(pBorderPanel, 1, wxEXPAND);
    SetSizerAndFit(pOuterSizer);

    // Clamp popup position to screen bounds
    wxSize popupSize = GetSize();
    int x = std::clamp(position.x, screenRect.x, screenRect.GetRight() - popupSize.GetWidth());
    int y = position.y;
    if (y + popupSize.GetHeight() > screenRect.GetBottom())
    {
        y = position.y - popupSize.GetHeight() - cursorLineHeight - 8;
    }
    y = std::max(y, screenRect.y);
    SetPosition(wxPoint(x, y));
}
