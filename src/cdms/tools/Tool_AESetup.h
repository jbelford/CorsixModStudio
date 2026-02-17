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

#include <rainman/module/CModuleFile.h>
#include <vector>
#include <utility>
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
// ----------------------------

#include "presenters/CUcsToDatPresenter.h"
#include "views/interfaces/IUcsToDatView.h"
#include <memory>

class frmUCSToDAT : public wxDialog, public IUcsToDatView
{
  protected:
    wxTextCtrl *m_pOutFile, *m_pRangeStart, *m_pRangeEnd;
    wxButton *m_pGoBtn = nullptr;
    wxButton *m_pCancelBtn = nullptr;
    std::unique_ptr<CUcsToDatPresenter> m_pPresenter;

  public:
    frmUCSToDAT();

    void OnBrowseOutClick(wxCommandEvent &event);
    void OnGoClick(wxCommandEvent &event);
    void OnCancelClick(wxCommandEvent &event);

    // IUcsToDatView implementation
    void ShowProgress(const wxString &sMessage) override;
    void HideProgress() override;
    void ShowError(const wxString &sMessage) override;
    void OnConversionComplete() override;
    void DisableControls() override;
    void EnableControls() override;

    enum
    {
        IDC_FileOut = wxID_HIGHEST + 1,
        IDC_BrowseOut,
        IDC_Go,
        IDC_Cancel,
    };

    DECLARE_EVENT_TABLE()
};

class UCSToDATConvertor
{
  public:
    UCSToDATConvertor();
    ~UCSToDATConvertor();

    void setOutputFilename(const char *sFilename);
    void setRange(unsigned long iStart, unsigned long iEnd);
    void setModule(const CModuleFile *pModule);

    void doConvertion();

  protected:
    char *m_sOutputName;
    unsigned long m_iRangeStart, m_iRangeEnd;
    const CModuleFile *m_pModule;

    // Used while converting:
    void _startRange(unsigned long iValue);
    void _endRange();
    bool _nextEntry(unsigned long *, wchar_t **);

    using SortedEntry = std::pair<unsigned long, wchar_t *>;
    using SortedEntries = std::vector<SortedEntry>;

    FILE *m_fDAT;
    size_t m_iUCSCount;
    SortedEntries::const_iterator *m_aUCSFiles;
    SortedEntries::const_iterator *m_aUCSFileEnds;
    std::vector<SortedEntries> m_vSortedMaps;
};
