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

#include "views/interfaces/IRgdEditorView.h"
#include <string>
#include <wx/string.h>

//! Presenter for the RGD data editor.
/*!
    Extracts pure business logic from frmRGDEditor: node name formatting,
    table reference formatting, hash computation, and Lua2 save-path
    normalisation. The tree population, property grid, and clipboard
    operations remain in the view due to deep wxWidgets coupling.
*/
class CRgdEditorPresenter
{
  public:
    explicit CRgdEditorPresenter(IRgdEditorView &view);

    //! Format a meta node's display name.
    /*!
        Returns the node's name if it has one, otherwise formats
        its hash as "0xHHHHHHHH" (8 uppercase hex digits).

        \param sName  Node name from VGetName(), or nullptr if unnamed.
        \param iNameHash  Hash from VGetNameHash() (used only when sName is empty).
        \return Display name string.
    */
    [[nodiscard]] static wxString FormatNodeName(const char *sName, unsigned long iNameHash);

    //! Format a table's reference value for display.
    /*!
        Returns the reference formatted as Lua code:
        - No reference: "{}"
        - GameData table: "Inherit([[ref]])"
        - Other tables: "Reference([[ref]])"

        \param sReference  The reference string (may be empty).
        \param bIsGameData  true if the parent node is named "GameData".
        \param bHasReference  true if the table has a non-empty reference.
        \return Formatted reference string.
    */
    [[nodiscard]] static wxString FormatTableReference(const wxString &sReference, bool bIsGameData,
                                                       bool bHasReference);

    //! Compute the multi-hash for a node name.
    /*!
        Strips trailing digits from the name before hashing, so that
        "unit_01", "unit_02" etc. all produce the same hash as "unit_".

        \param sName  Wide-character node name.
        \return Hash value.
    */
    [[nodiscard]] static unsigned long ComputeMultiHash(const wchar_t *sName);

    //! Normalise a file path for Lua2 save.
    /*!
        Strips known attribute directory prefixes ("Generic\\Attrib\\",
        "Data\\Attrib\\", "Attrib\\Attrib\\") and lowercases the remainder.

        \param sFilePath  Input file path (ASCII).
        \return Normalised path suitable for CLuaFile2::saveFile().
    */
    [[nodiscard]] static std::string NormaliseLua2SavePath(const char *sFilePath);

  private:
    IRgdEditorView &m_view;
};
