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

#ifndef _C_MODULE_METADATA_H_
#define _C_MODULE_METADATA_H_

#include <cstdlib>
#include <cstring>

//! Holds the parsed directive fields from a .module file.
//! This is a value type that owns all its string memory via strdup/free.
struct CModuleMetadata
{
    // String directives (all nullable, owned via strdup)
    char *m_sUiName = nullptr;
    char *m_sName = nullptr;
    char *m_sDescription = nullptr;
    char *m_sDllName = nullptr;
    char *m_sModFolder = nullptr;
    char *m_sTextureFe = nullptr;
    char *m_sTextureIcon = nullptr;
    char *m_sPatcherUrl = nullptr;
    char *m_sLocFolder = nullptr;
    char *m_sScenarioPackFolder = nullptr;

    // Version directives
    signed long m_iModVersionMajor = 0;
    signed long m_iModVersionMinor = 0;
    signed long m_iModVersionRevision = 0;

    CModuleMetadata() = default;
    ~CModuleMetadata() { Reset(); }

    CModuleMetadata(const CModuleMetadata &) = delete;
    CModuleMetadata &operator=(const CModuleMetadata &) = delete;

    CModuleMetadata(CModuleMetadata &&other) noexcept { MoveFrom(other); }

    CModuleMetadata &operator=(CModuleMetadata &&other) noexcept
    {
        if (this != &other)
        {
            Reset();
            MoveFrom(other);
        }
        return *this;
    }

    //! Free all owned strings and reset version fields to zero.
    void Reset()
    {
        FreeField(m_sUiName);
        FreeField(m_sName);
        FreeField(m_sDescription);
        FreeField(m_sDllName);
        FreeField(m_sModFolder);
        FreeField(m_sTextureFe);
        FreeField(m_sTextureIcon);
        FreeField(m_sPatcherUrl);
        FreeField(m_sLocFolder);
        FreeField(m_sScenarioPackFolder);

        m_iModVersionMajor = 0;
        m_iModVersionMinor = 0;
        m_iModVersionRevision = 0;
    }

    //! Set a string field, freeing the previous value.
    static void SetField(char *&field, const char *value)
    {
        if (field)
            free(field);
        field = value ? strdup(value) : nullptr;
    }

  private:
    static void FreeField(char *&field)
    {
        if (field)
            free(field);
        field = nullptr;
    }

    void MoveFrom(CModuleMetadata &other)
    {
        m_sUiName = other.m_sUiName;
        m_sName = other.m_sName;
        m_sDescription = other.m_sDescription;
        m_sDllName = other.m_sDllName;
        m_sModFolder = other.m_sModFolder;
        m_sTextureFe = other.m_sTextureFe;
        m_sTextureIcon = other.m_sTextureIcon;
        m_sPatcherUrl = other.m_sPatcherUrl;
        m_sLocFolder = other.m_sLocFolder;
        m_sScenarioPackFolder = other.m_sScenarioPackFolder;
        m_iModVersionMajor = other.m_iModVersionMajor;
        m_iModVersionMinor = other.m_iModVersionMinor;
        m_iModVersionRevision = other.m_iModVersionRevision;

        other.m_sUiName = nullptr;
        other.m_sName = nullptr;
        other.m_sDescription = nullptr;
        other.m_sDllName = nullptr;
        other.m_sModFolder = nullptr;
        other.m_sTextureFe = nullptr;
        other.m_sTextureIcon = nullptr;
        other.m_sPatcherUrl = nullptr;
        other.m_sLocFolder = nullptr;
        other.m_sScenarioPackFolder = nullptr;
        other.m_iModVersionMajor = 0;
        other.m_iModVersionMinor = 0;
        other.m_iModVersionRevision = 0;
    }
};

#endif
