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
#include "CHoverPopup.h"
#include "FindBar.h"
#include "ScarReferenceLoader.h"
#include "ScarSyntaxConfig.h"
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
    for (const auto &fn : m_vScarFunctions)
    {
        wxString sName = wxString::FromUTF8(fn.sName);
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
        std::string sWordUtf8(sWord.ToUTF8());
        for (const auto &fn : m_vScarFunctions)
        {
            if (fn.iType == 0 && sWordUtf8 == fn.sName)
            {
                m_oThisCalltip.sTip = wxT("");
                m_oThisCalltip.sTip.Append(wxString::FromUTF8(fn.sReturn));
                m_oThisCalltip.sTip.Append(wxT(" "));
                m_oThisCalltip.sTip.Append(wxString::FromUTF8(fn.sName));
                m_oThisCalltip.sTip.Append(wxT("("));
                bool bNeedComma = false;
                for (const auto &arg : fn.sArguments)
                {
                    if (bNeedComma)
                    {
                        m_oThisCalltip.sTip.Append(wxT(",\n    "));
                    }
                    m_oThisCalltip.sTip.Append(wxString::FromUTF8(arg));
                    bNeedComma = true;
                }
                m_oThisCalltip.sTip.Append(wxT(")\n"));
                m_oThisCalltip.sTip.Append(wxString::FromUTF8(fn.sDesc));
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
    // Load SCAR API reference data
    if (pLangRef)
    {
        const wchar_t *pRefPath = (GetConstruct()->GetModuleService().GetModuleType() == CModuleFile::MT_DawnOfWar)
                                      ? AppStr(app_scarreffile)
                                      : AppStr(app_cohscarreffile);
        m_vScarFunctions = LoadScarReference(pRefPath);
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

    m_pSTC = new wxStyledTextCtrl(this, IDC_Text);
    m_pFindBar = new FindBar(this, m_pSTC);
    pTopSizer->Add(m_pFindBar, 0, wxEXPAND);

    pTopSizer->Add(m_pSTC, 1, wxEXPAND | wxALL, 0);
    FillFunctionDrop(wxString());

    // Apply syntax highlighting, autocomplete icons, and indicator styles
    ApplyScarSyntaxConfig(m_pSTC);

    // Bind Ctrl+Space to show autocomplete
    m_pSTC->Bind(wxEVT_KEY_DOWN, &frmScarEditor::OnKeyDown, this);

    // Register SCAR API keywords for syntax highlighting (keyword sets 2+)
    // ApplyScarSyntaxConfig sets keyword sets 0-1 (builtins, Lua keywords).
    // Count how many keyword sets were already used.
    int iWordSet = 0;
    for (int i = 0; g_LuaWords[i].type != -1; ++i)
    {
        if (g_LuaWords[i].words)
        {
            ++iWordSet;
        }
    }

    wxString sScarFns, sScarConstants;
    for (const auto &fn : m_vScarFunctions)
    {
        if (fn.iType == 0)
        {
            sScarFns.Append(wxString::FromUTF8(fn.sName));
            sScarFns.Append(' ');
        }
        else
        {
            sScarConstants.Append(wxString::FromUTF8(fn.sName));
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

    // If the server hasn't finished initializing, defer the open to the next timer tick
    if (!pClient->IsReady())
    {
        m_bLspNeedsOpen = true;
        CDMS_LOG_DEBUG("LSP: Server not ready yet, deferring document open");
        return;
    }

    m_bLspNeedsOpen = false;

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

    // Retry deferred document open if the server is now ready
    if (m_bLspNeedsOpen && !m_bLspOpen)
    {
        LspOpenDocument();
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

void frmScarEditor::ShowFindBar() { m_pFindBar->Activate(); }

void frmScarEditor::HideFindBar() { m_pFindBar->Deactivate(); }
