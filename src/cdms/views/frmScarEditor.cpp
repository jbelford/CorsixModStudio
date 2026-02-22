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

#include "frmScarEditor.h"
#include "frame/Construct.h"
#include "common/strconv.h"
#include "common/strings.h"
#include "common/Utility.h"
#include "common/ThemeColours.h"
#include "presenters/CScarEditorPresenter.h"
#include "res/Icons.h"
extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <rainman/lua/Lua51.h>
}
#include <wx/artprov.h>
#include <wx/toolbar.h>
#include <wx/tbarbase.h>
#include <wx/popupwin.h>
#include <wx/display.h>
#include <algorithm>
#include "common/Common.h"
#include <cstdint>
#include <rainman/core/RainmanLog.h>
#include <lsp/LspClient.h>
#include <lsp/Protocol.h>

BEGIN_EVENT_TABLE(frmScarEditor, wxWindow)
EVT_SIZE(frmScarEditor::OnSize)
EVT_TOOL(IDC_ToolCheck, frmScarEditor::OnCheckLua)
EVT_BUTTON(IDC_Compile, frmScarEditor::OnCheckLua)
EVT_TOOL(IDC_ToolSave, frmScarEditor::OnSave)
EVT_BUTTON(wxID_SAVE, frmScarEditor::OnSave)
EVT_STC_CHARADDED(IDC_Text, frmScarEditor::OnCharAdded)
EVT_STC_STYLENEEDED(IDC_Text, frmScarEditor::OnStyleNeeded)
EVT_STC_USERLISTSELECTION(IDC_Text, frmScarEditor::OnAutoCompChoose)
EVT_STC_SAVEPOINTLEFT(IDC_Text, frmScarEditor::OnSavePointLeave)
EVT_STC_SAVEPOINTREACHED(IDC_Text, frmScarEditor::OnSavePointReach)
EVT_CLOSE(frmScarEditor::OnCloseWindow)
EVT_CHOICE(IDC_FunctionDrop, frmScarEditor::OnFuncListChoose)
EVT_TIMER(wxID_ANY, frmScarEditor::OnLspTimer)
EVT_STC_DWELLSTART(IDC_Text, frmScarEditor::OnDwellStart)
EVT_STC_DWELLEND(IDC_Text, frmScarEditor::OnDwellEnd)
END_EVENT_TABLE()

#define mySTC_STYLE_BOLD 1
#define mySTC_STYLE_ITALIC 2
#define mySTC_STYLE_UNDERL 4
#define mySTC_STYLE_HIDDEN 8

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

struct HightlightSet
{
    int type;
    const wxChar *words;
};

static HightlightSet g_LuaWords[] = {
    {wxSTC_LUA_DEFAULT, 0},
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
    {wxSTC_LUA_WORD2, _T("and break do else elseif end false for function if in local nil not or repeat ")
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

// Autocomplete type IDs for icon registration
enum AutoCompType
{
    ACT_Keyword = 0,
    ACT_Function = 1,
    ACT_Constant = 2,
    ACT_LocalFunc = 3
};

// clang-format off

// 16x16 XPM icon: blue "K" for Lua keywords
static const char *const g_xpmKeyword[] = {
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
static const char *const g_xpmFunction[] = {
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
static const char *const g_xpmConstant[] = {
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
static const char *const g_xpmLocalFunc[] = {
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

void frmScarEditor::_RestorePreviousCalltip()
{
    if (m_stkCalltips.size())
    {
        if (m_pSTC->CallTipActive())
        {
            m_pSTC->CallTipCancel();
        }
        m_oThisCalltip = m_stkCalltips.top();
        m_pSTC->CallTipShow(m_oThisCalltip.iPos, m_oThisCalltip.sTip);
        m_stkCalltips.pop();
    }
}

void frmScarEditor::OnSave(wxCommandEvent &event)
{
    UNUSED(event);
    DoSave();
}

void frmScarEditor::DoSave()
{
    BackupFile(TheConstruct->GetModuleService().GetModule(), m_sFilename);

    wxString sDir = m_sFilename.BeforeLast('\\');
    auto dirResult = TheConstruct->GetFileService().Iterate(sDir);
    if (!dirResult)
    {
        ErrorBoxS(dirResult.error());
        return;
    }

    auto streamResult = TheConstruct->GetFileService().OpenOutputStream(m_sFilename, true);
    if (!streamResult)
    {
        ErrorBoxS(streamResult.error());
        return;
    }

    TheConstruct->GetFilesList()->UpdateDirectoryChildren(m_oFileParent, dirResult.value().get());

    wxString sContent = m_pSTC->GetText();
    auto saContent = wxStringToAscii(sContent);
    if (!saContent)
    {
        ErrorBoxAS(err_memory);
        return;
    }

    try
    {
        streamResult.value()->VWrite((unsigned long)sContent.Length(), 1, saContent.get());
    }
    catch (const CRainmanException &e)
    {
        RestoreBackupFile(TheConstruct->GetModuleService().GetModule(), m_sFilename);
        ErrorBoxE(e);
        return;
    }
    m_bNeedsSaving = false;
    m_pSTC->SetSavePoint();
    TheConstruct->GetTabManager().UpdateDirtyState(this, false);
    ThemeColours::ShowMessageBox(AppStr(scar_savegood), AppStr(scar_save), wxICON_INFORMATION, this);
}

void frmScarEditor::OnSavePointLeave(wxStyledTextEvent &event)
{
    UNUSED(event);
    m_bNeedsSaving = true;
    TheConstruct->GetTabManager().UpdateDirtyState(this, true);
}

void frmScarEditor::OnSavePointReach(wxStyledTextEvent &event)
{
    UNUSED(event);
    m_bNeedsSaving = false;
    TheConstruct->GetTabManager().UpdateDirtyState(this, false);
}

void frmScarEditor::OnCloseWindow(wxCloseEvent &event)
{
    if (m_bNeedsSaving)
    {
        int ans = ThemeColours::ShowMessageBox(wxT("Text has been modified. Save file?"), m_sFilename,
                                               wxYES_NO | wxCANCEL | wxICON_WARNING, this);

        switch (ans)
        {
        case wxYES:
            DoSave();
            break;

        case wxCANCEL:
        default:
            if (event.CanVeto())
            {
                event.Veto();
            }
        case wxNO:
            break;
        }
    }
}

void frmScarEditor::OnAutoCompChoose(wxStyledTextEvent &event)
{
    UNUSED(event);
    _RestorePreviousCalltip();
}

void frmScarEditor::ShowAutoComplete()
{
    if (m_pSTC->AutoCompActive())
    {
        return;
    }

    int iPos = m_pSTC->GetCurrentPos();
    int iWordPos = m_pSTC->WordStartPosition(iPos, true);
    int iWordLen = iPos - iWordPos;

    if (m_bLspOpen)
    {
        // LSP is active — use language server completions exclusively
        auto *pFrame = GetConstruct();
        auto *pClient = pFrame ? pFrame->GetLspClient() : nullptr;
        if (pClient)
        {
            int line = m_pSTC->LineFromPosition(iPos);
            int col = iPos - m_pSTC->PositionFromLine(line);
            pClient->RequestCompletion(m_sLspUri, line, col,
                                       [this, iWordLen](lsp::CompletionList completions)
                                       {
                                           if (completions.items.empty())
                                           {
                                               return;
                                           }

                                           // Map LSP CompletionItemKind to our icon types
                                           auto mapKind = [](lsp::CompletionItemKind kind) -> int
                                           {
                                               switch (kind)
                                               {
                                               case lsp::CompletionItemKind::Function:
                                               case lsp::CompletionItemKind::Method:
                                                   return ACT_Function;
                                               case lsp::CompletionItemKind::Keyword:
                                                   return ACT_Keyword;
                                               case lsp::CompletionItemKind::Constant:
                                               case lsp::CompletionItemKind::EnumMember:
                                                   return ACT_Constant;
                                               case lsp::CompletionItemKind::Variable:
                                               case lsp::CompletionItemKind::Field:
                                               case lsp::CompletionItemKind::Property:
                                                   return ACT_LocalFunc;
                                               default:
                                                   return ACT_Keyword;
                                               }
                                           };

                                           wxString sItems;
                                           for (size_t i = 0; i < completions.items.size(); ++i)
                                           {
                                               if (i > 0)
                                               {
                                                   sItems.Append(' ');
                                               }
                                               sItems.Append(wxString::FromUTF8(completions.items[i].label));
                                               sItems.Append(
                                                   wxString::Format(wxT("?%d"), mapKind(completions.items[i].kind)));
                                           }

                                           _PushThisCalltip();
                                           m_pSTC->AutoCompShow(iWordLen, sItems);
                                       });
        }
        return;
    }

    // LSP is not active — use legacy SCAR/Lua autocomplete
    wxString sPrefix;
    if (iWordLen > 0)
    {
        sPrefix = m_pSTC->GetTextRange(iWordPos, iPos).Lower();
    }

    // Collect candidates as (name, type) pairs
    struct AutoCompItem
    {
        wxString sName;
        int iType;
    };
    std::vector<AutoCompItem> vCandidates;

    // SCAR API functions and constants
    for (const auto &fn : m_lstScarFunctions)
    {
        wxString sName = AsciiTowxString(fn.sName);
        if (iWordLen == 0 || sName.Lower().StartsWith(sPrefix))
        {
            vCandidates.push_back({sName, fn.iType == 0 ? ACT_Function : ACT_Constant});
        }
    }

    // Lua keywords
    static const wxChar *const kLuaKeywords[] = {
        wxT("and"),   wxT("break"), wxT("do"),       wxT("else"),   wxT("elseif"), wxT("end"),
        wxT("false"), wxT("for"),   wxT("function"), wxT("if"),     wxT("in"),     wxT("local"),
        wxT("nil"),   wxT("not"),   wxT("or"),       wxT("repeat"), wxT("return"), wxT("then"),
        wxT("true"),  wxT("until"), wxT("while"),    nullptr};
    for (const wxChar *const *pp = kLuaKeywords; *pp; ++pp)
    {
        wxString sKw(*pp);
        if (iWordLen == 0 || sKw.Lower().StartsWith(sPrefix))
        {
            vCandidates.push_back({sKw, ACT_Keyword});
        }
    }

    // Local function definitions from current source
    auto localFuncs = CScarEditorPresenter::ParseFunctionDefinitions(m_pSTC->GetText());
    for (const auto &def : localFuncs)
    {
        if (iWordLen == 0 || def.sName.Lower().StartsWith(sPrefix))
        {
            vCandidates.push_back({def.sName, ACT_LocalFunc});
        }
    }

    if (vCandidates.empty())
    {
        return;
    }

    // Sort by name case-insensitively and deduplicate
    std::sort(vCandidates.begin(), vCandidates.end(),
              [](const AutoCompItem &a, const AutoCompItem &b) { return a.sName.CmpNoCase(b.sName) < 0; });
    vCandidates.erase(std::unique(vCandidates.begin(), vCandidates.end(),
                                  [](const AutoCompItem &a, const AutoCompItem &b)
                                  { return a.sName.CmpNoCase(b.sName) == 0; }),
                      vCandidates.end());

    // Build space-separated list with type suffixes (e.g. "SGroup_Count?1")
    wxString sItems;
    for (size_t i = 0; i < vCandidates.size(); ++i)
    {
        if (i > 0)
        {
            sItems.Append(' ');
        }
        sItems.Append(vCandidates[i].sName);
        sItems.Append(wxString::Format(wxT("?%d"), vCandidates[i].iType));
    }

    _PushThisCalltip();
    m_pSTC->AutoCompShow(iWordLen, sItems);
}

void frmScarEditor::OnKeyDown(wxKeyEvent &event)
{
    if (event.GetKeyCode() == WXK_SPACE && event.ControlDown() && !event.AltDown() && !event.ShiftDown())
    {
        ShowAutoComplete();
        return;
    }
    event.Skip();
}

void frmScarEditor::_PushThisCalltip()
{
    if (m_pSTC->CallTipActive())
    {
        m_stkCalltips.push(m_oThisCalltip);
    }
    else
    {
        while (m_stkCalltips.size())
        {
            m_stkCalltips.pop();
        }
    }
}

void frmScarEditor::OnCharAdded(wxStyledTextEvent &event)
{
    m_bLspNeedsSync = true;
    int iCurrentLine = m_pSTC->GetCurrentLine();
    if ((char)event.GetKey() == '\n')
    {
        int iLineIndentation = 0;
        if (iCurrentLine > 0)
        {
            iLineIndentation = m_pSTC->GetLineIndentation(iCurrentLine - 1);
        }
        if (iLineIndentation == 0)
        {
            return;
        }
        m_pSTC->SetLineIndentation(iCurrentLine, iLineIndentation);
        m_pSTC->LineEnd();
    }
    else if ((char)event.GetKey() == '(')
    {
        _PushThisCalltip();
        int iPos = m_pSTC->GetCurrentPos() - 1;
        int iWordPos = m_pSTC->WordStartPosition(iPos, false);
        m_oThisCalltip.iPos = iWordPos;
        wxString sWord = m_pSTC->GetTextRange(iWordPos, iPos);
        auto saWord = wxStringToAscii(sWord);
        for (std::list<_ScarFunction>::iterator itr = m_lstScarFunctions.begin(); itr != m_lstScarFunctions.end();
             ++itr)
        {
            if (itr->iType == 0 && strcmp(saWord.get(), itr->sName) == 0)
            {
                m_oThisCalltip.sTip = wxT("");
                m_oThisCalltip.sTip.Append(AsciiTowxString(itr->sReturn));
                m_oThisCalltip.sTip.Append(wxT(" "));
                m_oThisCalltip.sTip.Append(AsciiTowxString(itr->sName));
                m_oThisCalltip.sTip.Append(wxT("("));
                bool bNeedComma = false;
                for (std::list<char *>::iterator itr2 = itr->sArguments.begin(); itr2 != itr->sArguments.end(); ++itr2)
                {
                    if (bNeedComma)
                    {
                        m_oThisCalltip.sTip.Append(wxT(",\n    "));
                    }
                    m_oThisCalltip.sTip.Append(AsciiTowxString(*itr2));
                    bNeedComma = true;
                }
                m_oThisCalltip.sTip.Append(wxT(")\n"));
                m_oThisCalltip.sTip.Append(AsciiTowxString(itr->sDesc));
                if (m_pSTC->CallTipActive())
                {
                    m_pSTC->CallTipCancel();
                }
                m_pSTC->CallTipSetBackground(ThemeColours::CallTipBg());
                m_pSTC->CallTipSetForeground(ThemeColours::CallTipFg());
                m_pSTC->CallTipShow(iWordPos, m_oThisCalltip.sTip);
                return;
            }
        }
        if (m_pSTC->CallTipActive())
        {
            m_pSTC->CallTipCancel();
        }

        // Fall back to LSP signature help when no SCAR match found
        if (m_bLspOpen)
        {
            auto *pFrame = GetConstruct();
            auto *pClient = pFrame ? pFrame->GetLspClient() : nullptr;
            if (pClient)
            {
                int line = m_pSTC->LineFromPosition(m_pSTC->GetCurrentPos());
                int col = m_pSTC->GetCurrentPos() - m_pSTC->PositionFromLine(line);
                int wordPos = iWordPos;
                pClient->RequestSignatureHelp(
                    m_sLspUri, line, col,
                    [this, wordPos](std::optional<lsp::SignatureHelp> help)
                    {
                        if (!help || help->signatures.empty())
                        {
                            return;
                        }
                        const auto &sig = help->signatures[static_cast<size_t>(help->activeSignature)];
                        wxString tip = wxString::FromUTF8(sig.label);
                        if (sig.documentation)
                        {
                            tip += wxT("\n") + wxString::FromUTF8(*sig.documentation);
                        }
                        m_pSTC->CallTipSetBackground(ThemeColours::CallTipBg());
                        m_pSTC->CallTipSetForeground(ThemeColours::CallTipFg());
                        m_pSTC->CallTipShow(wordPos, tip);

                        // Highlight the active parameter
                        if (!sig.parameters.empty() && help->activeParameter < static_cast<int>(sig.parameters.size()))
                        {
                            const auto &param = sig.parameters[static_cast<size_t>(help->activeParameter)];
                            size_t hlStart = sig.label.find(param.label);
                            if (hlStart != std::string::npos)
                            {
                                m_pSTC->CallTipSetHighlight(static_cast<int>(hlStart),
                                                            static_cast<int>(hlStart + param.label.size()));
                            }
                        }
                    });
                return;
            }
        }

        m_pSTC->CallTipSetBackground(ThemeColours::CallTipBg());
        m_pSTC->CallTipSetForeground(ThemeColours::CallTipFg());
        m_oThisCalltip.sTip = wxT("No help available");
        m_pSTC->CallTipShow(iWordPos, m_oThisCalltip.sTip);
    }
    else if ((char)event.GetKey() == '_')
    {
        ShowAutoComplete();
    }
    else if ((char)event.GetKey() == ')')
    {
        m_pSTC->CallTipCancel();
        _RestorePreviousCalltip();
    }
}

void frmScarEditor::Load(IFileStore::IStream *pFile)
{
    long iLength;
    try
    {
        pFile->VSeek(0, IFileStore::IStream::SL_End);
        iLength = pFile->VTell();
        pFile->VSeek(0, IFileStore::IStream::SL_Root);
    }
    catch (const CRainmanException &e)
    {
        ErrorBoxE(e);
        return;
    }

    unsigned char *sBuffer = new unsigned char[iLength];
    try
    {
        pFile->VRead(iLength, 1, sBuffer);
    }
    catch (const CRainmanException &e)
    {
        ErrorBoxE(e);
        delete[] sBuffer;
        return;
    }
    wchar_t *pBuffer = new wchar_t[iLength + 1];
    for (long i = 0; i < iLength; ++i)
    {
        pBuffer[i] = sBuffer[i];
    }
    pBuffer[iLength] = 0;
    m_pSTC->AddText(pBuffer);
    delete[] pBuffer;
    delete[] sBuffer;
    m_bNeedsSaving = false;
    m_pSTC->EmptyUndoBuffer();
    m_pSTC->SetSavePoint();

    m_pFunctionDropdown->Clear();
    FillFunctionDrop(wxString());

    // Now that the text is loaded, open the document with the LSP
    if (!m_bLspOpen)
    {
        LspOpenDocument();
    }
    else
    {
        LspSyncDocument();
    }
}

void frmScarEditor::OnStyleNeeded(wxStyledTextEvent &event)
{
    int iEndStyle = m_pSTC->GetEndStyled();
    m_pSTC->Colourise(iEndStyle, event.GetPosition());
}

const char *lua_tolstring(lua_State *L, int index, size_t *len)
{
    if (len)
    {
        *len = lua_strlen(L, index);
    }
    return lua_tostring(L, index);
}

template <class T> static inline void loadLuaFn(T &oP, wxString sN)
{
    (void)oP;
    (void)sN;
}

void frmScarEditor::OnCheckLua(wxCommandEvent &event)
{
    UNUSED(event);
    lua_State *(*pfn_lua_open)(void) = lua_open;
    int (*pfn_luaL_loadbuffer)(lua_State *, const char *, size_t, const char *) = luaL_loadbuffer;
    const char *(*pfn_lua_tolstring)(lua_State *, int, size_t *) = lua_tolstring;
    void (*pfn_lua_close)(lua_State *) = lua_close;

    if (GetConstruct()->GetModuleService().GetModuleType() != CModuleFile::MT_DawnOfWar)
    {
        pfn_lua_open = (lua_State * (*)(void)) lua51L_newstate;
        pfn_luaL_loadbuffer = lua51L_loadbuffer;
        pfn_lua_tolstring = lua51_tolstring;
        pfn_lua_close = lua51_close;
    }

    auto sLua = wxStringToAscii(m_pSTC->GetText());

    lua_State *L;
    L = pfn_lua_open();
    int iLuaError = pfn_luaL_loadbuffer(L, sLua.get(), strlen(sLua.get()), "");
    if (iLuaError)
    {
        const char *sRawErr = pfn_lua_tolstring(L, -1, 0);
        CScarEditorPresenter::LuaError oError;
        if (CScarEditorPresenter::ParseLuaError(sRawErr, oError))
        {
            wxString sError;
            sError.Printf(AppStr(scar_bad), oError.iLine, oError.sMessage.c_str());
            ThemeColours::ShowMessageBox(sError, AppStr(scar_checklua), wxICON_ERROR, this);
        }
        else
        {
            ThemeColours::ShowMessageBox(wxString::FromUTF8(sRawErr), AppStr(scar_checklua), wxICON_ERROR, this);
        }
    }
    else
    {
        ThemeColours::ShowMessageBox(AppStr(scar_luagood), AppStr(scar_checklua), wxICON_INFORMATION, this);
    }

    pfn_lua_close(L);

    m_pFunctionDropdown->Clear();
    FillFunctionDrop(wxString());
}

char *frmScarEditor::_ReadNiceString(FILE *f)
{
    uint32_t iLen;
    fread(&iLen, sizeof(uint32_t), 1, f);
    char *s = new char[iLen + 1];
    fread(s, iLen, 1, f);
    s[iLen] = 0;
    return s;
}

void frmScarEditor::OnFuncListChoose(wxCommandEvent &event)
{
    if (event.GetSelection())
    {
        m_pFunctionDropdown->Clear();
        int iPosNew = FillFunctionDrop(event.GetString());
        if (iPosNew != -1)
        {
            m_pSTC->GotoPos(iPosNew);
            m_pSTC->Home();
            m_pSTC->LineEndExtend();
            m_pSTC->SetFocus();
        }
    }
}

int frmScarEditor::FillFunctionDrop(const wxString &sNameTarget)
{
    int iRet = -1;
    m_pFunctionDropdown->Append(AppStr(scar_funcdrop), (void *)0);
    m_pFunctionDropdown->SetSelection(0);

    auto aFunctions = CScarEditorPresenter::ParseFunctionDefinitions(m_pSTC->GetText());

    for (const auto &def : aFunctions)
    {
        if (iRet == -1 && sNameTarget.IsSameAs(def.sName, false))
        {
            iRet = def.iPosition;
        }
        m_pFunctionDropdown->Append(def.sName, reinterpret_cast<void *>(static_cast<intptr_t>(def.iPosition)));
    }
    return iRet;
}

frmScarEditor::frmScarEditor(const wxTreeItemId &oFileParent, wxString sFilename, wxWindow *parent, wxWindowID id,
                             const wxPoint &pos, const wxSize &size, const wchar_t *pLangRef)
    : m_oFileParent(oFileParent), m_sFilename(sFilename), wxWindow(parent, id, pos, size), m_bNeedsSaving(false)
{
    // Load ref
    if (pLangRef)
    {
        FILE *f;
        if (GetConstruct()->GetModuleService().GetModuleType() == CModuleFile::MT_DawnOfWar)
        {
            f = _wfopen(AppStr(app_scarreffile), L"rb");
        }
        else
        {
            f = _wfopen(AppStr(app_cohscarreffile), L"rb");
        }

        if (!f)
        {
            CDMS_LOG_WARN("Could not open SCAR reference file");
            pLangRef = nullptr;
        }
        else
        {
            uint32_t iFnCount;
            fread(&iFnCount, sizeof(uint32_t), 1, f);
            for (uint32_t i = 0; i < iFnCount; ++i)
            {
                _ScarFunction oFunction;
                oFunction.sReturn = _ReadNiceString(f);
                oFunction.sName = _ReadNiceString(f);
                oFunction.iType = 0;
                if (*oFunction.sName == 0)
                {
                    oFunction.iType = 1;
                    delete[] oFunction.sName;
                    oFunction.sName = oFunction.sReturn;
                    oFunction.sReturn = 0;
                    oFunction.sDesc = 0;
                }
                else
                {
                    uint32_t iArgCount;
                    fread(&iArgCount, sizeof(uint32_t), 1, f);
                    for (uint32_t j = 0; j < iArgCount; ++j)
                    {
                        oFunction.sArguments.push_back(_ReadNiceString(f));
                    }
                    oFunction.sDesc = _ReadNiceString(f);
                }
                m_lstScarFunctions.push_back(oFunction);
            }
            fclose(f);
        }
    }

    // Gui
    wxBoxSizer *pTopSizer = new wxBoxSizer(wxVERTICAL);

    wxToolBar *pToolbar;
    pTopSizer->Add(pToolbar = new wxToolBar(this, -1, wxDefaultPosition, wxDefaultSize,
                                            wxTB_NODIVIDER | wxTB_HORIZONTAL | wxNO_BORDER),
                   0, wxEXPAND | wxALL, 3);

    wxArrayString lstFuncDrop;
    pTopSizer->Add(m_pFunctionDropdown =
                       new wxChoice(this, IDC_FunctionDrop, wxDefaultPosition, wxDefaultSize, lstFuncDrop),
                   0, wxEXPAND | wxALL, 3);

    auto oSaveBundle = wxArtProvider::GetBitmapBundle(wxART_FILE_SAVE, wxART_TOOLBAR, wxSize(32, 32));
    auto oCheckBundle = wxBitmapBundle::FromSVG(cdms::icons::kCheck, wxSize(32, 32));
    pToolbar->SetToolBitmapSize(wxSize(32, 32));
    pToolbar->AddTool(IDC_ToolSave, AppStr(scar_save), oSaveBundle, AppStr(scar_save));
    pToolbar->AddTool(IDC_ToolCheck, AppStr(scar_checklua), oCheckBundle, AppStr(scar_checklua));
    pToolbar->Realize();

    pTopSizer->Add(m_pSTC = new wxStyledTextCtrl(this, IDC_Text), 1, wxEXPAND | wxALL, 0);
    FillFunctionDrop(wxString());

    m_pSTC->StyleClearAll();
    // default font for all styles
    m_pSTC->SetTabWidth(4);
    m_pSTC->SetLexer(wxSTC_LEX_LUA);
    m_pSTC->SetProperty(wxT("fold.compact"), wxT("0"));
    m_pSTC->SetViewEOL(false);
    m_pSTC->SetIndentationGuides(true);
    m_pSTC->SetEdgeMode(wxSTC_EDGE_NONE);
    m_pSTC->SetViewWhiteSpace(wxSTC_WS_INVISIBLE);
    m_pSTC->SetOvertype(false);
    m_pSTC->SetReadOnly(false);
    m_pSTC->SetWrapMode(wxSTC_WRAP_NONE);
    wxFont font = MakeCodeFont(10);
    m_pSTC->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
    m_pSTC->SetUseTabs(true);
    m_pSTC->SetTabIndents(true);
    m_pSTC->SetBackSpaceUnIndents(true);
    m_pSTC->SetIndent(4);

    // default fonts for all styles!
    for (int i = 0; i < wxSTC_STYLE_LASTPREDEFINED; ++i)
    {
        m_pSTC->StyleSetFont(i, font);
    }

    int iWordSet = 0;
    for (int i = 0; g_LuaWords[i].type != -1; ++i)
    {
        const StyleInfo &curType = g_StylePrefs[g_LuaWords[i].type];
        wxFont styleFont = MakeCodeFont(curType.fontsize, (curType.fontstyle & mySTC_STYLE_BOLD) != 0,
                                        (curType.fontstyle & mySTC_STYLE_ITALIC) != 0);
        m_pSTC->StyleSetFont(g_LuaWords[i].type, styleFont);
        m_pSTC->StyleSetUnderline(g_LuaWords[i].type, (curType.fontstyle & mySTC_STYLE_UNDERL) > 0);
        m_pSTC->StyleSetVisible(g_LuaWords[i].type, (curType.fontstyle & mySTC_STYLE_HIDDEN) == 0);
        m_pSTC->StyleSetCase(g_LuaWords[i].type, curType.lettercase);
        if (g_LuaWords[i].words)
        {
            m_pSTC->SetKeyWords(iWordSet++, g_LuaWords[i].words);
        }
    }

    // Apply theme-aware colours (editor bg/fg, syntax highlighting, line numbers)
    ThemeColours::ApplyEditorTheme(m_pSTC);

    // Autocomplete settings
    m_pSTC->AutoCompSetIgnoreCase(true);
    m_pSTC->AutoCompSetAutoHide(true);
    m_pSTC->AutoCompSetChooseSingle(false);
    m_pSTC->AutoCompSetMaxHeight(15);
    m_pSTC->AutoCompSetMaxWidth(40);

    // Register autocomplete type icons
    m_pSTC->RegisterImage(ACT_Keyword, g_xpmKeyword);
    m_pSTC->RegisterImage(ACT_Function, g_xpmFunction);
    m_pSTC->RegisterImage(ACT_Constant, g_xpmConstant);
    m_pSTC->RegisterImage(ACT_LocalFunc, g_xpmLocalFunc);

    // LSP diagnostic indicator styles
    m_pSTC->IndicatorSetStyle(INDICATOR_LSP_ERROR, wxSTC_INDIC_SQUIGGLE);
    m_pSTC->IndicatorSetForeground(INDICATOR_LSP_ERROR, wxColour(255, 0, 0));
    m_pSTC->IndicatorSetStyle(INDICATOR_LSP_WARNING, wxSTC_INDIC_SQUIGGLE);
    m_pSTC->IndicatorSetForeground(INDICATOR_LSP_WARNING, wxColour(200, 150, 0));
    m_pSTC->IndicatorSetStyle(INDICATOR_LSP_INFO, wxSTC_INDIC_DOTS);
    m_pSTC->IndicatorSetForeground(INDICATOR_LSP_INFO, wxColour(100, 100, 200));

    // Enable mouse hover (dwell) events for diagnostic tooltips
    m_pSTC->SetMouseDwellTime(500);

    // Bind Ctrl+Space to show autocomplete
    m_pSTC->Bind(wxEVT_KEY_DOWN, &frmScarEditor::OnKeyDown, this);

    wxString sScarFns, sScarConstants;
    size_t iLenFns = 0, iLenCons = 0;
    for (std::list<_ScarFunction>::iterator itr = m_lstScarFunctions.begin(); itr != m_lstScarFunctions.end(); ++itr)
    {
        if (itr->iType == 0)
        {
            iLenFns += (strlen(itr->sName) + 1);
        }
        else
        {
            iLenCons += (strlen(itr->sName) + 1);
        }
    }
    sScarFns.Alloc(iLenFns);
    sScarConstants.Alloc(iLenCons);
    for (std::list<_ScarFunction>::iterator itr = m_lstScarFunctions.begin(); itr != m_lstScarFunctions.end(); ++itr)
    {
        if (itr->iType == 0)
        {
            sScarFns.Append(AsciiTowxString(itr->sName));
            sScarFns.Append(' ');
        }
        else
        {
            sScarConstants.Append(AsciiTowxString(itr->sName));
            sScarConstants.Append(' ');
        }
    }
    m_pSTC->SetKeyWords(iWordSet, sScarFns);
    m_pSTC->SetKeyWords(++iWordSet, sScarConstants);

    // misc. STC stuff
    m_pSTC->SetMarginWidth(0, m_pSTC->TextWidth(wxSTC_STYLE_LINENUMBER, _T("_999999")));

    // Buttons
    wxBoxSizer *pButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxWindow *pBgTemp;

    pButtonSizer->Add(pBgTemp = new wxButton(this, IDC_Compile, AppStr(scar_checklua)), 0, wxEXPAND | wxALL, 3);
    pButtonSizer->Add(new wxButton(this, wxID_SAVE, AppStr(scar_save)), 0, wxEXPAND | wxALL, 3);

    pTopSizer->Add(pButtonSizer, 0, wxALIGN_RIGHT);

    SetBackgroundColour(pBgTemp->GetBackgroundColour());

    SetSizer(pTopSizer);
    pTopSizer->SetSizeHints(this);

    // Start LSP polling timer (document will be opened after Load() populates text)
    m_lspTimer.SetOwner(this);
    m_lspTimer.Start(100); // Poll every 100ms
}

frmScarEditor::~frmScarEditor()
{
    m_lspTimer.Stop();
    LspCloseDocument();

    // Unload Ref
    for (std::list<_ScarFunction>::iterator itr = m_lstScarFunctions.begin(); itr != m_lstScarFunctions.end(); ++itr)
    {
        delete[] itr->sDesc;
        delete[] itr->sName;
        delete[] itr->sReturn;
        for (std::list<char *>::iterator itr2 = itr->sArguments.begin(); itr2 != itr->sArguments.end(); ++itr2)
        {
            delete[] *itr2;
        }
    }
}

// ---------------------------------------------------------------------------
// LSP integration
// ---------------------------------------------------------------------------

void frmScarEditor::LspOpenDocument()
{
    auto *pFrame = GetConstruct();
    if (!pFrame)
    {
        return;
    }
    auto *pClient = pFrame->GetLspClient();
    if (!pClient)
    {
        CDMS_LOG_DEBUG("LSP: No language server available for editor");
        return;
    }

    // Build a file:// URI from the filename
    m_sLspUri = "file:///" + std::string(m_sFilename.ToUTF8());
    // Normalize backslashes to forward slashes for URI
    for (char &c : m_sLspUri)
    {
        if (c == '\\')
        {
            c = '/';
        }
    }

    m_iLspVersion = 1;
    std::string text(m_pSTC->GetText().ToUTF8());
    pClient->OpenDocument(m_sLspUri, "lua", text);
    m_bLspOpen = true;

    // Register per-URI diagnostics callback
    pClient->RegisterDiagnosticsCallback(m_sLspUri, [this](const std::string &uri, std::vector<lsp::Diagnostic> diags)
                                         { ApplyDiagnostics(uri, std::move(diags)); });
}

void frmScarEditor::LspSyncDocument()
{
    if (!m_bLspOpen)
    {
        return;
    }

    auto *pFrame = GetConstruct();
    if (!pFrame)
    {
        return;
    }
    auto *pClient = pFrame->GetLspClient();
    if (!pClient)
    {
        return;
    }

    ++m_iLspVersion;
    std::string text(m_pSTC->GetText().ToUTF8());
    pClient->ChangeDocument(m_sLspUri, text);
}

void frmScarEditor::LspCloseDocument()
{
    if (!m_bLspOpen)
    {
        return;
    }

    m_bLspOpen = false;

    // During app shutdown the ConstructFrame may already be partially
    // destroyed when child windows are torn down by the base-class
    // destructor.  Calling GetConstruct() at that point is UB.
    // IsBeingDeleted() on the top-level window detects this case.
    auto *pTopWin = wxTheApp ? wxTheApp->GetTopWindow() : nullptr;
    if (!pTopWin || pTopWin->IsBeingDeleted())
    {
        return;
    }

    auto *pFrame = GetConstruct();
    if (!pFrame)
    {
        return;
    }
    auto *pClient = pFrame->GetLspClient();
    if (pClient)
    {
        pClient->UnregisterDiagnosticsCallback(m_sLspUri);
        pClient->CloseDocument(m_sLspUri);
    }
}

void frmScarEditor::OnLspTimer(wxTimerEvent &event)
{
    UNUSED(event);
    auto *pFrame = GetConstruct();
    if (!pFrame)
    {
        return;
    }
    auto *pClient = pFrame->GetLspClient();
    if (!pClient)
    {
        return;
    }

    if (m_bLspNeedsSync && m_bLspOpen)
    {
        m_bLspNeedsSync = false;
        LspSyncDocument();
    }

    pClient->Poll();
}

void frmScarEditor::ApplyDiagnostics(const std::string &uri, std::vector<lsp::Diagnostic> diagnostics)
{
    if (uri != m_sLspUri)
    {
        return;
    }

    // Store diagnostics for hover tooltip lookup
    m_vDiagnostics = diagnostics;

    // Clear existing diagnostic indicators
    m_pSTC->SetIndicatorCurrent(INDICATOR_LSP_ERROR);
    m_pSTC->IndicatorClearRange(0, m_pSTC->GetLength());
    m_pSTC->SetIndicatorCurrent(INDICATOR_LSP_WARNING);
    m_pSTC->IndicatorClearRange(0, m_pSTC->GetLength());
    m_pSTC->SetIndicatorCurrent(INDICATOR_LSP_INFO);
    m_pSTC->IndicatorClearRange(0, m_pSTC->GetLength());

    for (const auto &diag : diagnostics)
    {
        int startPos = m_pSTC->PositionFromLine(diag.range.start.line) + diag.range.start.character;
        int endPos = m_pSTC->PositionFromLine(diag.range.end.line) + diag.range.end.character;
        int length = endPos - startPos;
        if (length <= 0)
        {
            length = 1;
        }

        int indicator = INDICATOR_LSP_INFO;
        switch (diag.severity)
        {
        case lsp::DiagnosticSeverity::Error:
            indicator = INDICATOR_LSP_ERROR;
            break;
        case lsp::DiagnosticSeverity::Warning:
            indicator = INDICATOR_LSP_WARNING;
            break;
        default:
            indicator = INDICATOR_LSP_INFO;
            break;
        }

        m_pSTC->SetIndicatorCurrent(indicator);
        m_pSTC->IndicatorFillRange(startPos, length);
    }
}

/// Parse LuaLS hover markdown into code signature and description parts.
struct DiagnosticEntry
{
    lsp::DiagnosticSeverity severity;
    std::string text; // Pre-formatted: "Error: message [source]"
};

struct HoverParts
{
    std::string code;                         // Function signature (from code fences)
    std::string description;                  // Description text (after ---)
    std::vector<DiagnosticEntry> diagnostics; // Diagnostics at this position
};

static HoverParts ParseHoverMarkdown(const std::string &md)
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
// CHoverPopup — rich hover window with syntax-highlighted code + description
// ---------------------------------------------------------------------------

class CHoverPopup : public wxPopupTransientWindow
{
  public:
    CHoverPopup(wxWindow *parent, const HoverParts &parts, const wxPoint &position, int cursorLineHeight)
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
            int totalDisplayLines = 0;
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
                auto *pDiag = new wxStaticText(pInnerPanel, wxID_ANY, diagText, wxDefaultPosition, wxDefaultSize,
                                               wxST_NO_AUTORESIZE);
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
};

void frmScarEditor::OnDwellStart(wxStyledTextEvent &event)
{
    int pos = event.GetPosition();
    if (pos < 0)
    {
        return;
    }

    // Don't overwrite an active calltip from function signatures
    if (m_pSTC->CallTipActive())
    {
        return;
    }

    // Collect all diagnostics that cover the hovered position
    std::vector<DiagnosticEntry> matchedDiags;
    for (const auto &diag : m_vDiagnostics)
    {
        int startPos = m_pSTC->PositionFromLine(diag.range.start.line) + diag.range.start.character;
        int endPos = m_pSTC->PositionFromLine(diag.range.end.line) + diag.range.end.character;
        if (endPos <= startPos)
        {
            endPos = startPos + 1;
        }

        if (pos >= startPos && pos < endPos)
        {
            std::string prefix;
            switch (diag.severity)
            {
            case lsp::DiagnosticSeverity::Error:
                prefix = "Error: ";
                break;
            case lsp::DiagnosticSeverity::Warning:
                prefix = "Warning: ";
                break;
            case lsp::DiagnosticSeverity::Information:
                prefix = "Info: ";
                break;
            case lsp::DiagnosticSeverity::Hint:
                prefix = "Hint: ";
                break;
            }
            std::string text = prefix + diag.message;
            if (!diag.source.empty())
            {
                text += " [" + diag.source + "]";
            }
            matchedDiags.push_back({diag.severity, std::move(text)});
        }
    }

    // Request hover info from LSP (diagnostics will be merged into the popup)
    auto *pFrame = GetConstruct();
    auto *pClient = pFrame ? pFrame->GetLspClient() : nullptr;
    if (pClient && m_bLspOpen)
    {
        int line = m_pSTC->LineFromPosition(pos);
        int col = pos - m_pSTC->PositionFromLine(line);
        int hoverPos = pos;

        pClient->RequestHover(m_sLspUri, line, col,
                              [this, hoverPos, diags = std::move(matchedDiags)](std::optional<lsp::HoverResult> hover)
                              {
                                  HoverParts parts;
                                  if (hover && !hover->contents.empty())
                                  {
                                      parts = ParseHoverMarkdown(hover->contents);
                                  }
                                  parts.diagnostics = std::move(diags);

                                  if (parts.code.empty() && parts.description.empty() && parts.diagnostics.empty())
                                  {
                                      return;
                                  }
                                  // Don't show if a calltip is already active
                                  if (m_pSTC->CallTipActive())
                                  {
                                      return;
                                  }
                                  DismissHoverPopup();

                                  wxPoint ptEditor = m_pSTC->PointFromPosition(hoverPos);
                                  int lineHeight = m_pSTC->TextHeight(m_pSTC->LineFromPosition(hoverPos));
                                  wxPoint ptScreen = m_pSTC->ClientToScreen(ptEditor);
                                  ptScreen.y += lineHeight + 4;

                                  m_pHoverPopup = new CHoverPopup(this, parts, ptScreen, lineHeight);
                                  m_pHoverPopup->Popup();
                              });
    }
    else if (!matchedDiags.empty())
    {
        // LSP unavailable — show diagnostics-only popup
        DismissHoverPopup();
        HoverParts parts;
        parts.diagnostics = std::move(matchedDiags);

        wxPoint ptEditor = m_pSTC->PointFromPosition(pos);
        int lineHeight = m_pSTC->TextHeight(m_pSTC->LineFromPosition(pos));
        wxPoint ptScreen = m_pSTC->ClientToScreen(ptEditor);
        ptScreen.y += lineHeight + 4;

        m_pHoverPopup = new CHoverPopup(this, parts, ptScreen, lineHeight);
        m_pHoverPopup->Popup();
    }
}

void frmScarEditor::OnDwellEnd(wxStyledTextEvent &event)
{
    UNUSED(event);
    // Only cancel CallTip if it's not a function signature calltip
    if (m_pSTC->CallTipActive() && m_oThisCalltip.sTip.IsEmpty())
    {
        m_pSTC->CallTipCancel();
    }
    DismissHoverPopup();
}

void frmScarEditor::DismissHoverPopup()
{
    if (m_pHoverPopup)
    {
        m_pHoverPopup->Dismiss();
        m_pHoverPopup = nullptr;
    }
}

void frmScarEditor::OnSize(wxSizeEvent &event)
{
    UNUSED(event);
    Layout();
}
