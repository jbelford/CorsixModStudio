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

#include "views/interfaces/IModuleSettingsView.h"
#include <rainman/module/CModuleFile.h>
#include <wx/arrstr.h>
#include <wx/string.h>

//! Presenter for the module settings editor.
/*!
    Extracts business logic from frmModule: directory filtering for mod folder
    selection, DLL filtering, and module metadata list formatting.
    All methods are pure functions that operate on provided data, making them
    testable without a running GUI or global state.
*/
class CModuleSettingsPresenter
{
  public:
    explicit CModuleSettingsPresenter(IModuleSettingsView &view);

    //! Filter a list of directory names, removing known engine/system directories.
    /*!
        \param aAllDirs   All directory names found in the game folder.
        \return Filtered list with engine directories removed.
    */
    [[nodiscard]] static wxArrayString FilterModFolders(const wxArrayString &aAllDirs);

    //! Filter a list of DLL filenames, removing known engine/system DLLs.
    /*!
        \param aAllDlls     All .dll filenames (without extension) found in the game folder.
        \param sCurrentDll  The currently selected DLL name (always included in output).
        \return Filtered list with engine DLLs removed.
    */
    [[nodiscard]] static wxArrayString FilterDllFiles(const wxArrayString &aAllDlls, const wxString &sCurrentDll);

    //! Check if a directory name is a known engine/system directory.
    [[nodiscard]] static bool IsEngineDirectory(const wxString &sName);

    //! Check if a DLL name (without extension) is a known engine/system DLL.
    [[nodiscard]] static bool IsEngineDll(const wxString &sName);

    //! Format a module folder entry for display: "[number] name".
    [[nodiscard]] static wxString FormatFolderEntry(long iNumber, const wxString &sName);

    //! Format a module archive entry for display: "[number] filename".
    [[nodiscard]] static wxString FormatArchiveEntry(long iNumber, const wxString &sFileName);

    //! Get formatted folder entries from a CModuleFile.
    [[nodiscard]] static wxArrayString GetFolderEntries(CModuleFile *pMod);

    //! Get formatted archive entries from a CModuleFile.
    [[nodiscard]] static wxArrayString GetArchiveEntries(CModuleFile *pMod);

    //! Get formatted required mod entries from a CModuleFile.
    [[nodiscard]] static wxArrayString GetRequiredEntries(CModuleFile *pMod);

    //! Get formatted compatible mod entries from a CModuleFile.
    [[nodiscard]] static wxArrayString GetCompatibleEntries(CModuleFile *pMod);

  private:
    IModuleSettingsView &m_view;
};
