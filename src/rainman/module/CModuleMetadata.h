/*
Rainman Library
Copyright (C) 2006 Corsix <corsix@gmail.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#include <string>

//! Holds the parsed directive fields from a .module file.
//! This is a value type — all string memory is managed by std::string.
struct CModuleMetadata
{
    // String directives
    std::string m_sUiName;
    std::string m_sName;
    std::string m_sDescription;
    std::string m_sDllName;
    std::string m_sModFolder;
    std::string m_sTextureFe;
    std::string m_sTextureIcon;
    std::string m_sPatcherUrl;
    std::string m_sLocFolder;
    std::string m_sScenarioPackFolder;

    // Version directives
    signed long m_iModVersionMajor = 0;
    signed long m_iModVersionMinor = 0;
    signed long m_iModVersionRevision = 0;

    CModuleMetadata() = default;
    ~CModuleMetadata() = default;

    CModuleMetadata(const CModuleMetadata &) = default;
    CModuleMetadata &operator=(const CModuleMetadata &) = default;

    CModuleMetadata(CModuleMetadata &&) noexcept = default;
    CModuleMetadata &operator=(CModuleMetadata &&) noexcept = default;

    //! Clear all fields to defaults.
    void Reset()
    {
        m_sUiName.clear();
        m_sName.clear();
        m_sDescription.clear();
        m_sDllName.clear();
        m_sModFolder.clear();
        m_sTextureFe.clear();
        m_sTextureIcon.clear();
        m_sPatcherUrl.clear();
        m_sLocFolder.clear();
        m_sScenarioPackFolder.clear();

        m_iModVersionMajor = 0;
        m_iModVersionMinor = 0;
        m_iModVersionRevision = 0;
    }
};
