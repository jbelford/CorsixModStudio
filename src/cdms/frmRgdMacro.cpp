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

#include "Construct.h"
#include "frmRgdMacro.h"
#include "common/strings.h"
#include "common/strconv.h"
#include "common/Utility.h"
#include <wx/file.h>
#include <rainman/util/Util.h>
#include "common/Common.h"

extern "C"
{
#include "rainman/util/crc32_case_idt.h"
}

BEGIN_EVENT_TABLE(frmRgdMacro, wxDialog)
EVT_BUTTON(IDC_Run, frmRgdMacro::OnRunClick)
EVT_BUTTON(IDC_Cancel, frmRgdMacro::OnCancelClick)
EVT_BUTTON(IDC_Load, frmRgdMacro::OnLoadClick)
EVT_BUTTON(IDC_Save, frmRgdMacro::OnSaveClick)
EVT_BUTTON(IDC_Mode, frmRgdMacro::OnModeClick)
EVT_STC_STYLENEEDED(-1, frmRgdMacro::OnStyleNeeded)
EVT_STC_CHARADDED(-1, frmRgdMacro::OnCharAdded)
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
    {_T("Operator"), _T("RED"), _T("WHITE"), _T(""), 10, 0 /*mySTC_STYLE_BOLD*/, 0},

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

void frmRgdMacro::OnStyleNeeded(wxStyledTextEvent &event)
{
    int iEndStyle = m_pSTC->GetEndStyled();
    m_pSTC->Colourise(iEndStyle, event.GetPosition());
}

frmRgdMacro::frmRgdMacro(wxString sFile, wxTreeItemId &oFolder)
    : m_oFolder(oFolder), m_sPath(sFile),
      wxDialog(wxTheApp->GetTopWindow(), -1, AppStr(rgdmacro_title), wxPoint(0, 0),
               wxWindow::FromDIP(wxSize(400, 600), wxTheApp->GetTopWindow()),
               wxFRAME_FLOAT_ON_PARENT | wxSYSTEM_MENU | wxCAPTION | wxRESIZE_BORDER | wxCLOSE_BOX),
      m_pPresenter(std::make_unique<CRgdMacroPresenter>(*this, this))
{
    m_bShowingOutput = false;
    wxBoxSizer *pTopSizer = m_pFormMainSizer = new wxBoxSizer(wxVERTICAL);

    pTopSizer->Add(m_pCaption = new wxStaticText(this, -1, AppStr(rgdmacro_caption)), 0, wxALIGN_LEFT | wxALL, 3);

    pTopSizer->Add(m_pTextbox = new wxTextCtrl(this, -1, wxT(""), wxDefaultPosition, wxDefaultSize,
                                               wxTE_PROCESS_TAB | wxTE_MULTILINE | wxHSCROLL),
                   1, wxEXPAND | wxALL, 3);
    pTopSizer->Hide(m_pTextbox);

    pTopSizer->Add(m_pSTC = new wxStyledTextCtrl(this, -1), 1, wxEXPAND | wxALL, 3);

    wxBoxSizer *pButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    pButtonSizer->Add(new wxButton(this, IDC_Cancel, AppStr(rgdmacro_cancel)), 0, wxFIXED_MINSIZE | wxALL, 3);
    pButtonSizer->Add(m_pModeBtn = new wxButton(this, IDC_Mode, AppStr(rgdmacro_gotooutput)), 0,
                      wxFIXED_MINSIZE | wxALL, 3);
    pButtonSizer->Add(m_pSaveBtn = new wxButton(this, IDC_Save, AppStr(rgdmacro_savemacro)), 0, wxFIXED_MINSIZE | wxALL,
                      3);
    pButtonSizer->Add(m_pLoadBtn = new wxButton(this, IDC_Load, AppStr(rgdmacro_loadmacro)), 0, wxFIXED_MINSIZE | wxALL,
                      3);
    pButtonSizer->Add(m_pRunBtn = new wxButton(this, IDC_Run, AppStr(rgdmacro_run)), 0, wxFIXED_MINSIZE | wxALL, 3);

    pTopSizer->Add(pButtonSizer, 0, wxALIGN_RIGHT);
    pTopSizer->SetMinSize(0, 400);

    SetSizer(pTopSizer);
    pTopSizer->SetSizeHints(this);

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
    wxFont font(10, wxMODERN, wxNORMAL, wxNORMAL);
    m_pSTC->StyleSetFont(wxSTC_STYLE_DEFAULT, font);
    m_pSTC->StyleSetBackground(wxSTC_STYLE_DEFAULT, *wxWHITE);
    m_pSTC->StyleSetForeground(wxSTC_STYLE_LINENUMBER, wxColour(_T("DARK GREY")));
    m_pSTC->StyleSetBackground(wxSTC_STYLE_LINENUMBER, wxColour(_T("WHEAT")));
    m_pSTC->SetUseTabs(true);
    m_pSTC->SetTabIndents(true);
    m_pSTC->SetBackSpaceUnIndents(true);
    m_pSTC->SetIndent(4);

    // default fonts for all styles!
    for (int i = 0; i < wxSTC_STYLE_LASTPREDEFINED; ++i)
    {
        wxFont font(10, wxMODERN, wxNORMAL, wxNORMAL);
        m_pSTC->StyleSetFont(i, font);
    }

    // set common styles
    m_pSTC->StyleSetForeground(wxSTC_STYLE_DEFAULT, wxColour(_T("DARK GREY")));
    m_pSTC->StyleSetForeground(wxSTC_STYLE_INDENTGUIDE, wxColour(_T("DARK GREY")));

    int iWordSet = 0;
    for (int i = 0; g_LuaWords[i].type != -1; ++i)
    {
        const StyleInfo &curType = g_StylePrefs[g_LuaWords[i].type];
        wxFont font(curType.fontsize, wxMODERN, wxNORMAL, wxNORMAL, false, curType.fontname);
        m_pSTC->StyleSetFont(g_LuaWords[i].type, font);
        if (curType.foreground)
            m_pSTC->StyleSetForeground(g_LuaWords[i].type, wxColour(curType.foreground));
        if (curType.background)
            m_pSTC->StyleSetBackground(g_LuaWords[i].type, wxColour(curType.background));
        m_pSTC->StyleSetBold(g_LuaWords[i].type, (curType.fontstyle & mySTC_STYLE_BOLD) > 0);
        m_pSTC->StyleSetItalic(g_LuaWords[i].type, (curType.fontstyle & mySTC_STYLE_ITALIC) > 0);
        m_pSTC->StyleSetUnderline(g_LuaWords[i].type, (curType.fontstyle & mySTC_STYLE_UNDERL) > 0);
        m_pSTC->StyleSetVisible(g_LuaWords[i].type, (curType.fontstyle & mySTC_STYLE_HIDDEN) == 0);
        m_pSTC->StyleSetCase(g_LuaWords[i].type, curType.lettercase);
        if (g_LuaWords[i].words)
            m_pSTC->SetKeyWords(iWordSet++, g_LuaWords[i].words);
    }

    m_pSTC->SetMarginWidth(0, m_pSTC->TextWidth(wxSTC_STYLE_LINENUMBER, _T("_9999")));

    wxString sDefaultContent =
        wxT("function each_file(rgd)\n") wxT("\t-- TODO:\n") wxT("\t-- Put some code here\n") wxT("end\n") wxT("\n")
            wxT("function at_end()\n") wxT("\t-- TODO:\n") wxT("\t-- Put some code here\n") wxT("end\n");
    m_pSTC->AddText(sDefaultContent);

    CentreOnParent();
}

void frmRgdMacro::OnModeClick(wxCommandEvent &event)
{
    UNUSED(event);
    if (m_bShowingOutput)
    {
        m_bShowingOutput = false;

        m_pFormMainSizer->Hide(m_pTextbox);
        m_pFormMainSizer->Show(m_pSTC);
        m_pSaveBtn->SetLabel(AppStr(rgdmacro_savemacro));
        m_pModeBtn->SetLabel(AppStr(rgdmacro_gotooutput));
        m_pCaption->SetLabel(AppStr(rgdmacro_caption));
        m_pLoadBtn->Enable(true);
        m_pRunBtn->Enable(true);
    }
    else
    {
        m_bShowingOutput = true;

        m_pFormMainSizer->Hide(m_pSTC);
        m_pFormMainSizer->Show(m_pTextbox);

        m_pSaveBtn->SetLabel(AppStr(rgdmacro_saveoutput));
        m_pModeBtn->SetLabel(AppStr(rgdmacro_gotomacro));
        m_pCaption->SetLabel(AppStr(rgdmacro_captionout));
        m_pLoadBtn->Enable(false);
        m_pRunBtn->Enable(false);
    }
    Layout();
}

void frmRgdMacro::OnCharAdded(wxStyledTextEvent &event)
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
            return;
        m_pSTC->SetLineIndentation(iCurrentLine, iLineIndentation);
        m_pSTC->LineEnd();
    }
}

// --- IRgdMacroView implementation ---

void frmRgdMacro::ShowProgress(const wxString &sMessage) { m_pCaption->SetLabel(sMessage); }

void frmRgdMacro::HideProgress() {}

void frmRgdMacro::ShowError(const wxString &sMessage) { _ErrorBox(sMessage, __FILE__, __LINE__); }

void frmRgdMacro::AppendOutput(const wxString &sText) { m_pTextbox->AppendText(sText); }

bool frmRgdMacro::RequestPermission(const wxString &sQuestion)
{
    wxString sFullQuestion = wxT("The macro is requesting permission for the ability ");
    sFullQuestion.Append(sQuestion);
    sFullQuestion.Append(wxT(".\nDo you want to grant it permission?"));

    return ::wxMessageBox(sFullQuestion, wxT("Macro Permissions"), wxYES_NO) == wxYES;
}

void frmRgdMacro::OnMacroComplete(const std::vector<wxString> &vFoldersToUpdate)
{
    for (const auto &sFolder : vFoldersToUpdate)
    {
        wxString sFolderCopy = sFolder;
        wxTreeCtrl *pTree = GetConstruct()->GetFilesList()->GetTree();
        wxTreeItemId oParent = pTree->GetRootItem();
        wxTreeItemIdValue oCookie;
        wxTreeItemId oChild = pTree->GetFirstChild(oParent, oCookie);
        wxString sPart = sFolderCopy.BeforeFirst('\\');
        bool bMoveNext = true;
        while (oChild.IsOk())
        {
            if (pTree->GetItemText(oChild).IsSameAs(sPart, false))
            {
                sFolderCopy = sFolderCopy.AfterFirst('\\');
                sPart = sFolderCopy.BeforeFirst('\\');

                if (!pTree->ItemHasChildren(oChild) || sPart.IsEmpty())
                {
                    break;
                }
                else
                {
                    oParent = oChild;
                    oChild = pTree->GetFirstChild(oParent, oCookie);
                    bMoveNext = false;
                }
            }
            if (bMoveNext)
                oChild = pTree->GetNextChild(oParent, oCookie);
            else
                bMoveNext = true;
        }
        if (sPart.IsEmpty())
        {
            auto saFolder = wxStringToAscii(sFolder);
            auto dirResult = TheConstruct->GetFileService().Iterate(sFolder);
            if (dirResult)
            {
                TheConstruct->GetFilesList()->UpdateDirectoryChildren(oChild, dirResult.value().get());
            }
        }
    }
}

void frmRgdMacro::DisableControls()
{
    m_pModeBtn->Enable(false);
    m_pSaveBtn->Enable(false);
    m_pRunBtn->Enable(false);
    m_pLoadBtn->Enable(false);
}

void frmRgdMacro::EnableControls()
{
    m_pModeBtn->Enable(true);
    m_pSaveBtn->Enable(true);
    m_pRunBtn->Enable(true);
    m_pLoadBtn->Enable(true);
}

void frmRgdMacro::OnRunClick(wxCommandEvent &event)
{
    UNUSED(event);

    wxString sContent = m_pSTC->GetText();
    auto saContent = wxStringToAscii(sContent);

    // Validate macro by loading it synchronously (fast, needs UI for error reporting)
    CRgdFileMacro oTestMacro;
    oTestMacro.setHashTable(TheConstruct->GetHashService().GetHashTable());
    try
    {
        oTestMacro.loadMacro(saContent.get());
    }
    catch (CRainmanException *pE)
    {
        ErrorBoxE(pE);
        return;
    }
    oTestMacro.unloadMacro();

    m_pTextbox->Clear();

    if (!m_bShowingOutput)
    {
        wxCommandEvent e;
        OnModeClick(e);
    }

    // Populate file list (needs UI / TheConstruct access on main thread)
    std::vector<char *> vRawFiles;
    _populateFileList(&vRawFiles);

    // Convert to std::string so the presenter can own the data
    std::vector<std::string> vFiles;
    vFiles.reserve(vRawFiles.size());
    for (char *s : vRawFiles)
    {
        vFiles.emplace_back(s);
        free(s);
    }

    IFileStore *pFileStore = TheConstruct->GetModuleService().GetModule();

    m_pPresenter->RunMacro(sContent, std::move(vFiles), pFileStore, TheConstruct->GetHashService().GetHashTable(),
                           TheConstruct->GetModuleService().GetModule(),
                           TheConstruct->GetModuleService().GetModuleType() == CModuleFile::MT_DawnOfWar, m_iPathLen,
                           m_iPathHash);
}

void ForEach_FindRgds(IDirectoryTraverser::IIterator *pItr, void *pTag)
{
    const char *sName = pItr->VGetName();
    sName = strrchr(sName, '.');
    if (sName && (strcmp(sName, ".rgd") == 0))
    {
        std::vector<char *> *lstFiles = (std::vector<char *> *)pTag;
        lstFiles->push_back(strdup(pItr->VGetFullPath()));
    }
}

void frmRgdMacro::_populateFileList(std::vector<char *> *lstFiles)
{
    auto saPath = wxStringToAscii(m_sPath);

    m_iPathLen = (unsigned long)m_sPath.Length();
    m_iPathHash = crc32_case_idt(0, (const unsigned char *)saPath.get(), m_iPathLen);
    if ((saPath.get()[m_iPathLen - 1] != '/') && (saPath.get()[m_iPathLen - 1] != '\\'))
    {
        m_iPathHash = crc32_case_idt(m_iPathHash, (const unsigned char *)"\\", 1);
        ++m_iPathLen;
    }

    auto dirResult = TheConstruct->GetFileService().Iterate(m_sPath);
    if (!dirResult)
    {
        return;
    }
    try
    {
        Rainman_ForEach(dirResult.value().get(), ForEach_FindRgds, (void *)lstFiles, true);
    }
    IGNORE_EXCEPTIONS
}

void frmRgdMacro::OnCancelClick(wxCommandEvent &event)
{
    UNUSED(event);
    EndModal(wxID_CLOSE);
}

void frmRgdMacro::OnSaveClick(wxCommandEvent &event)
{
    UNUSED(event);
    if (m_bShowingOutput)
    {
        wxString sFilename = wxFileSelector(AppStr(rgdmacro_saveoutputmsg), AppStr(app_macrospath), wxT(""), wxT("txt"),
                                            AppStr(rgdmacro_saveoutputfilter), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (!sFilename.IsEmpty())
        {
            wxFile oFile;
            oFile.Open(sFilename, wxFile::write);
            wxString sContent = m_pTextbox->GetValue();
            oFile.Write(sContent);
            oFile.Close();
        }
    }
    else
    {
        wxString sFilename = wxFileSelector(AppStr(rgdmacro_savemacromsg), AppStr(app_macrospath), wxT(""), wxT("lua"),
                                            AppStr(rgdmacro_loadmacrofilter), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
        if (!sFilename.IsEmpty())
        {
            wxFile oFile;
            oFile.Open(sFilename, wxFile::write);
            wxString sContent = m_pSTC->GetText();
            oFile.Write(sContent);
            oFile.Close();
        }
    }
}

void frmRgdMacro::OnLoadClick(wxCommandEvent &event)
{
    UNUSED(event);
    wxString sFilename = wxFileSelector(AppStr(rgdmacro_loadmacromsg), AppStr(app_macrospath), wxT(""), wxT("lua"),
                                        AppStr(rgdmacro_loadmacrofilter), wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (!sFilename.IsEmpty())
    {
        wxFile oFile;
        oFile.Open(sFilename);
        size_t iL = oFile.Length();
        char *pBuffer = new char[iL + 1];
        oFile.Read(pBuffer, iL);
        pBuffer[iL] = 0;
        oFile.Close();

        m_pSTC->ClearAll();
        m_pSTC->EmptyUndoBuffer();
        m_pSTC->AddText(AsciiTowxString(pBuffer));
        delete[] pBuffer;
    }
}
