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

#include "views/interfaces/IUcsEditorView.h"
#include <rainman/module/CModuleFile.h>
#include <rainman/localization/CUcsMap.h>
#include <wx/string.h>

//! Presenter for the UCS localization editor.
/*!
    Extracts business logic from frmUCSEditor: UCS ID validation, range
    checking, next-ID suggestion, duplicate detection, and locale save-path
    resolution. All validation methods are static pure functions.
*/
class CUcsEditorPresenter
{
  public:
    //! Recommended range for mod UCS IDs.
    static constexpr unsigned long kMinRecommendedId = 15000000;
    static constexpr unsigned long kMaxRecommendedId = 20000000;

    explicit CUcsEditorPresenter(IUcsEditorView &view);

    //! Check whether a UCS ID is within the recommended modding range.
    [[nodiscard]] static bool IsIdInRecommendedRange(unsigned long iId);

    //! Suggest the next available UCS ID (max existing + 1).
    /*!
        \param entries  Raw UCS entry map (id → string).
        \return Suggested next ID, or kMinRecommendedId if entries is empty.
    */
    [[nodiscard]] static unsigned long SuggestNextId(const CUcsMap &entries);

    //! Result of validating a new UCS ID.
    enum class IdValidation
    {
        Valid,     //!< ID is usable.
        Duplicate, //!< ID already exists in the map.
    };

    //! Validate a proposed new UCS ID against existing entries.
    [[nodiscard]] static IdValidation ValidateNewId(unsigned long iId, const CUcsMap &entries);

    //! Parse a UCS ID from user input text.
    /*!
        Skips leading non-digit characters (e.g. '$') and parses the number.
        \param sInput  User-entered text (may contain '$' prefix).
        \param[out] iId  Parsed ID on success.
        \return true if a valid number was found, false otherwise.
    */
    [[nodiscard]] static bool ParseIdFromInput(const wxString &sInput, unsigned long &iId);

    //! Build the locale base directory path for saving UCS files.
    /*!
        \param sModuleDir    Directory containing the .module file.
        \param eModType      Module type (DoW vs CoH).
        \param sLocaleFolder Locale folder from module metadata (may be empty).
        \param sModFolder    Mod folder name.
        \param sLocale       Current locale string (e.g. "english").
        \return Full path to the locale directory (with trailing backslash).
    */
    [[nodiscard]] static wxString BuildLocaleBasePath(const wxString &sModuleDir, CModuleFile::eModuleType eModType,
                                                      const wxString &sLocaleFolder, const wxString &sModFolder,
                                                      const wxString &sLocale);

  private:
    IUcsEditorView &m_view;
};
