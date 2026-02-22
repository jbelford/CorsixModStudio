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

#include "views/interfaces/IFileTreeView.h"
#include <wx/string.h>
#include <vector>

//! Presenter for the file browser panel.
/*!
    Extracts pure business logic from frmFiles: file type classification
    by extension, file source categorization, and engine file map detection.
    The tree population and update logic remains in the view due to deep
    wxTreeCtrl coupling.
*/
class CFileTreePresenter
{
  public:
    explicit CFileTreePresenter(IFileTreeView &view);

    //! File source category for colouring tree items.
    enum class FileSource
    {
        ThisMod, //!< File comes from the currently loaded mod.
        Engine,  //!< File comes from the game engine.
        OtherMod //!< File comes from another mod (dependency).
    };

    //! Icon image indices for file types in the tree.
    enum FileIcon : int
    {
        Icon_AI = 0,
        Icon_Lua = 1,
        Icon_Nil = 2,
        Icon_Rgd = 3,
        Icon_FolderClosed = 4,
        Icon_FolderOpen = 5,
        Icon_Scar = 6,
        Icon_Tga = 8,
        Icon_Rgt = 9,
        Icon_Dds = 10,
        Icon_Bfx = 11,
        Icon_Abp = 12,
        Icon_Rgm = 13,
        Icon_Unknown = 14
    };

    //! Determine the icon image index for a file based on its extension.
    /*!
        \param sFileName  File name (e.g. "data.rgd", "script.scar").
        \return Image index for the file type, or Icon_Unknown (-2) if unrecognised.
    */
    [[nodiscard]] static int ClassifyFileIcon(const wxString &sFileName);

    //! Check if a file map name matches any engine in the module.
    /*!
        \param sFileMapName  The file map name tag from the directory traverser.
        \param asEngineFileMapNames  List of engine file map names.
        \return true if the name matches any engine file map.
    */
    [[nodiscard]] static bool IsEngineFileMapName(const wxString &sFileMapName,
                                                  const std::vector<wxString> &asEngineFileMapNames);

    //! Determine the source category of a file.
    /*!
        \param sFileMapName  The file map name tag from the directory traverser.
        \param sThisModFileMapName  The current mod's file map name.
        \param asEngineFileMapNames  List of engine file map names.
        \return The source category for colouring.
    */
    [[nodiscard]] static FileSource CategorizeFileSource(const wxString &sFileMapName,
                                                         const wxString &sThisModFileMapName,
                                                         const std::vector<wxString> &asEngineFileMapNames);

  private:
    IFileTreeView &m_view;
};
