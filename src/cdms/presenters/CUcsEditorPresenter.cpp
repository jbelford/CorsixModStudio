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

#include "CUcsEditorPresenter.h"
#include <algorithm>

CUcsEditorPresenter::CUcsEditorPresenter(IUcsEditorView &view) : m_view(view) {}

bool CUcsEditorPresenter::IsIdInRecommendedRange(unsigned long iId)
{
    return iId >= kMinRecommendedId && iId <= kMaxRecommendedId;
}

unsigned long CUcsEditorPresenter::SuggestNextId(const CUcsFile::UcsMap &entries)
{
    if (entries.empty())
        return kMinRecommendedId;

    auto itMax = std::max_element(entries.begin(), entries.end(),
                                  [](const auto &a, const auto &b) { return a.first < b.first; });
    return itMax->first + 1;
}

CUcsEditorPresenter::IdValidation CUcsEditorPresenter::ValidateNewId(unsigned long iId, const CUcsFile::UcsMap &entries)
{
    auto it = entries.find(iId);
    if (it != entries.end() && it->second != nullptr)
        return IdValidation::Duplicate;

    return IdValidation::Valid;
}

bool CUcsEditorPresenter::ParseIdFromInput(const wxString &sInput, unsigned long &iId)
{
    const wchar_t *pStr = sInput.c_str();
    // Skip leading non-digit characters (e.g. '$')
    while (*pStr && (*pStr < '0' || *pStr > '9'))
        ++pStr;
    if (!*pStr)
        return false;

    iId = wcstoul(pStr, nullptr, 10);
    return true;
}

wxString CUcsEditorPresenter::BuildLocaleBasePath(const wxString &sModuleDir, CModuleFile::eModuleType eModType,
                                                  const wxString &sLocaleFolder, const wxString &sModFolder,
                                                  const wxString &sLocale)
{
    wxString sPath = sModuleDir;
    if (!sPath.EndsWith(wxT("\\")))
        sPath.Append(wxT("\\"));

    if (eModType == CModuleFile::MT_CompanyOfHeroes)
    {
        sPath.Append(wxT("Engine\\Locale\\"));
    }
    else
    {
        if (!sLocaleFolder.IsEmpty())
        {
            sPath.Append(sLocaleFolder);
            sPath.Append(wxT("\\"));
        }
        else
        {
            sPath.Append(sModFolder);
            sPath.Append(wxT("\\Locale\\"));
        }
    }

    sPath.Append(sLocale);
    sPath.Append(wxT("\\"));
    return sPath;
}
