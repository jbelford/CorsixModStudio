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

#include "CModuleSettingsPresenter.h"
#include "common/strconv.h"
#include <algorithm>

CModuleSettingsPresenter::CModuleSettingsPresenter(IModuleSettingsView &view) : m_view(view) {}

// Engine/system directories excluded from the mod folder chooser.
static const wxString s_aEngineDirectories[] = {
    wxT("Badges"),          wxT("Banners"),     wxT("BugReport"), wxT("Drivers"), wxT("Engine"),
    wxT("GraphicsOptions"), wxT("Logfiles"),    wxT("ModTools"),  wxT("Patch"),   wxT("Playback"),
    wxT("Profiles"),        wxT("ScreenShots"), wxT("Stats"),     wxT("Utils"),
};

// Engine/system DLLs excluded from the DLL chooser (case-insensitive).
static const wxString s_aEngineDlls[] = {
    wxT("dbghelp"),       wxT("Debug"),    wxT("divxdecoder"), wxT("divxmedialib"), wxT("DllTie"),  wxT("fileparser"),
    wxT("FileSystem"),    wxT("GSLobby"),  wxT("ijl15"),       wxT("Localizer"),    wxT("luabind"), wxT("LuaConfig"),
    wxT("MathBox"),       wxT("Memory"),   wxT("mfc71"),       wxT("msvcp71"),      wxT("msvcr71"), wxT("Patch"),
    wxT("Platform"),      wxT("PlatHook"), wxT("seInterface"), wxT("SimEngine"),    wxT("spDx9"),   wxT("STLPort"),
    wxT("UserInterface"), wxT("Util"),
};

bool CModuleSettingsPresenter::IsEngineDirectory(const wxString &sName)
{
    for (const auto &sDir : s_aEngineDirectories)
    {
        if (sName == sDir)
            return true;
    }
    return false;
}

bool CModuleSettingsPresenter::IsEngineDll(const wxString &sName)
{
    for (const auto &sDll : s_aEngineDlls)
    {
        if (sName.IsSameAs(sDll, false))
            return true;
    }
    return false;
}

wxArrayString CModuleSettingsPresenter::FilterModFolders(const wxArrayString &aAllDirs)
{
    wxArrayString aResult;
    for (const auto &sDir : aAllDirs)
    {
        if (!IsEngineDirectory(sDir))
            aResult.Add(sDir);
    }
    return aResult;
}

wxArrayString CModuleSettingsPresenter::FilterDllFiles(const wxArrayString &aAllDlls, const wxString &sCurrentDll)
{
    wxArrayString aResult;
    for (const auto &sDll : aAllDlls)
    {
        if (!sDll.IsSameAs(sCurrentDll, false) && !IsEngineDll(sDll))
            aResult.Add(sDll);
    }
    // Always include the current DLL
    aResult.Add(sCurrentDll);
    return aResult;
}

wxString CModuleSettingsPresenter::FormatFolderEntry(long iNumber, const wxString &sName)
{
    wxString sResult;
    sResult.Printf(wxT("[%li] "), iNumber);
    sResult.Append(sName);
    return sResult;
}

wxString CModuleSettingsPresenter::FormatArchiveEntry(long iNumber, const wxString &sFileName)
{
    return FormatFolderEntry(iNumber, sFileName);
}

wxArrayString CModuleSettingsPresenter::GetFolderEntries(CModuleFile *pMod)
{
    wxArrayString aResult;
    if (pMod == nullptr)
        return aResult;

    auto iCount = pMod->GetFolderCount();
    for (std::size_t i = 0; i < iCount; ++i)
    {
        auto *pFolder = pMod->GetFolder(i);
        aResult.Add(FormatFolderEntry(pFolder->GetNumber(), AsciiTowxString(pFolder->GetName())));
    }
    return aResult;
}

wxArrayString CModuleSettingsPresenter::GetArchiveEntries(CModuleFile *pMod)
{
    wxArrayString aResult;
    if (pMod == nullptr)
        return aResult;

    auto iCount = pMod->GetArchiveCount();
    for (std::size_t i = 0; i < iCount; ++i)
    {
        auto *pArch = pMod->GetArchive(i);
        aResult.Add(FormatArchiveEntry(pArch->GetNumber(), AsciiTowxString(pArch->GetFileName())));
    }
    return aResult;
}

wxArrayString CModuleSettingsPresenter::GetRequiredEntries(CModuleFile *pMod)
{
    wxArrayString aResult;
    if (pMod == nullptr)
        return aResult;

    auto iCount = pMod->GetRequiredCount();
    for (std::size_t i = 0; i < iCount; ++i)
    {
        auto *pReq = pMod->GetRequired(i);
        aResult.Add(FormatArchiveEntry(pReq->GetNumber(), AsciiTowxString(pReq->GetFileName())));
    }
    return aResult;
}

wxArrayString CModuleSettingsPresenter::GetCompatibleEntries(CModuleFile *pMod)
{
    wxArrayString aResult;
    if (pMod == nullptr)
        return aResult;

    auto iCount = pMod->GetCompatibleCount();
    for (std::size_t i = 0; i < iCount; ++i)
    {
        auto *pCompat = pMod->GetCompatible(i);
        aResult.Add(FormatArchiveEntry(pCompat->GetNumber(), AsciiTowxString(pCompat->GetFileName())));
    }
    return aResult;
}
