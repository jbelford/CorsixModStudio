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
extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <rainman/lua/Lua51.h>
}
#include <wx/toolbar.h>
#include <wx/tbarbase.h>
#include <algorithm>
#include "common/Common.h"
#include <cstdint>
#include <rainman/core/RainmanLog.h>

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

    wxBitmap oSaveBmp(wxT("IDB_32SAVE"), wxBITMAP_TYPE_BMP_RESOURCE),
        oCheckBmp(wxT("IDB_32CHECK"), wxBITMAP_TYPE_BMP_RESOURCE);
    oSaveBmp.SetMask(new wxMask(oSaveBmp, wxColour(128, 128, 128)));
    oCheckBmp.SetMask(new wxMask(oCheckBmp, wxColour(128, 128, 128)));
    pToolbar->SetToolBitmapSize(wxSize(32, 32));
    pToolbar->AddTool(IDC_ToolSave, AppStr(scar_save), oSaveBmp, AppStr(scar_save));
    pToolbar->AddTool(IDC_ToolCheck, AppStr(scar_checklua), oCheckBmp, AppStr(scar_checklua));
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
}

frmScarEditor::~frmScarEditor()
{
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

void frmScarEditor::OnSize(wxSizeEvent &event)
{
    UNUSED(event);
    Layout();
}
