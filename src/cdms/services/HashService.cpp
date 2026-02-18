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

#include "HashService.h"
#include "common/strconv.h"
#include <rainman/core/Exception.h>
#include <rainman/formats/CRgdHashTable.h>
#include <rainman/io/CFileSystemStore.h>
#include <rainman/io/IDirectoryTraverser.h>
#include <cstdlib>
#include <cstring>

HashService::~HashService() { Shutdown(); }

Result<void> HashService::Initialize(const wxString &sDictionaryPath)
{
    // Clean up any previous state
    Shutdown();

    m_pHashTable = new CRgdHashTable;
    if (!m_pHashTable)
        return Result<void>::Err(wxT("Memory allocation error"));

    auto sDicPath = wxStringToAscii(sDictionaryPath);
    if (!sDicPath)
    {
        delete m_pHashTable;
        m_pHashTable = nullptr;
        return Result<void>::Err(wxT("Memory allocation error"));
    }

    CFileSystemStore oStore;
    IDirectoryTraverser::IIterator *pItr = nullptr;
    try
    {
        pItr = oStore.VIterate(sDicPath.get());
    }
    catch (const CRainmanException &e)
    {
        delete m_pHashTable;
        m_pHashTable = nullptr;
        return ResultFromException(e);
    }

    if (!pItr)
        return Result<void>::Ok();

    try
    {
        do
        {
            if (pItr->VGetType() == IDirectoryTraverser::IIterator::T_File)
            {
                if (stricmp(pItr->VGetName(), "custom.txt") == 0)
                {
                    m_sCustomOutPath = strdup(pItr->VGetFullPath());
                }
                m_pHashTable->ExtendWithDictionary(pItr->VGetFullPath(),
                                                   (stricmp(pItr->VGetName(), "custom.txt") == 0));
            }
        } while (pItr->VNextItem() == IDirectoryTraverser::IIterator::E_OK);
    }
    catch (const CRainmanException &e)
    {
        delete pItr;
        if (m_sCustomOutPath)
        {
            free(m_sCustomOutPath);
            m_sCustomOutPath = nullptr;
        }
        delete m_pHashTable;
        m_pHashTable = nullptr;
        return ResultFromException(e);
    }
    delete pItr;

    return Result<void>::Ok();
}

void HashService::Shutdown()
{
    if (m_pHashTable)
    {
        if (m_sCustomOutPath)
            m_pHashTable->SaveCustomKeys(m_sCustomOutPath);
        delete m_pHashTable;
        m_pHashTable = nullptr;
    }
    if (m_sCustomOutPath)
    {
        free(m_sCustomOutPath);
        m_sCustomOutPath = nullptr;
    }
}

Result<void> HashService::CrossRefWithStringList(const wxString &sFile)
{
    auto s = wxStringToAscii(sFile);
    if (!s)
        return Result<void>::Err(wxT("Memory allocation error"));

    try
    {
        m_pHashTable->XRefWithStringList(s.get());
    }
    catch (const CRainmanException &e)
    {
        return ResultFromException(e);
    }

    return Result<void>::Ok();
}

void HashService::FillUnknownList(std::vector<unsigned long> &oList) { m_pHashTable->FillUnknownList(oList); }

unsigned long HashService::ValueToHash(const char *sValue) { return m_pHashTable->ValueToHash(sValue); }
