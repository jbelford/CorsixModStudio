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

#include "FileService.h"
#include "../strconv.h"
#include <rainman/core/Exception.h>

// --- Stream operations ---

Result<rainman::StreamGuard> FileService::OpenStream(const wxString &sPath)
{
    char *sFile = wxStringToAscii(sPath);
    if (!sFile)
        return Result<rainman::StreamGuard>::Err(wxT("Memory allocation error"));

    IFileStore::IStream *pStream = nullptr;
    try
    {
        pStream = m_pStore->VOpenStream(sFile);
    }
    catch (CRainmanException *pE)
    {
        delete[] sFile;
        return ResultFromExceptionT<rainman::StreamGuard>(pE);
    }

    delete[] sFile;
    if (!pStream)
        return Result<rainman::StreamGuard>::Err(wxT("Cannot open file"));

    return Result<rainman::StreamGuard>::Ok(rainman::StreamGuard(pStream));
}

Result<rainman::OutputStreamGuard> FileService::OpenOutputStream(const wxString &sPath, bool bEraseIfPresent)
{
    char *sFile = wxStringToAscii(sPath);
    if (!sFile)
        return Result<rainman::OutputStreamGuard>::Err(wxT("Memory allocation error"));

    IFileStore::IOutputStream *pStream = nullptr;
    try
    {
        pStream = m_pStore->VOpenOutputStream(sFile, bEraseIfPresent);
    }
    catch (CRainmanException *pE)
    {
        delete[] sFile;
        return ResultFromExceptionT<rainman::OutputStreamGuard>(pE);
    }

    delete[] sFile;
    if (!pStream)
        return Result<rainman::OutputStreamGuard>::Err(wxT("Cannot open output stream"));

    return Result<rainman::OutputStreamGuard>::Ok(rainman::OutputStreamGuard(pStream));
}

// --- Directory traversal ---

Result<IteratorGuard> FileService::Iterate(const wxString &sPath)
{
    char *sDir = wxStringToAscii(sPath);
    if (!sDir)
        return Result<IteratorGuard>::Err(wxT("Memory allocation error"));

    IDirectoryTraverser::IIterator *pItr = nullptr;
    try
    {
        pItr = m_pTraverser->VIterate(sDir);
    }
    catch (CRainmanException *pE)
    {
        delete[] sDir;
        return ResultFromExceptionT<IteratorGuard>(pE);
    }

    delete[] sDir;
    return Result<IteratorGuard>::Ok(IteratorGuard(pItr));
}

unsigned long FileService::GetEntryPointCount() const { return m_pTraverser->VGetEntryPointCount(); }

wxString FileService::GetEntryPoint(unsigned long iID) const
{
    const char *s = m_pTraverser->VGetEntryPoint(iID);
    return s ? wxString::FromUTF8(s) : wxString();
}

Result<bool> FileService::DirectoryExists(const wxString &sPath)
{
    char *sDir = wxStringToAscii(sPath);
    if (!sDir)
        return Result<bool>::Err(wxT("Memory allocation error"));

    bool bExists = false;
    try
    {
        bExists = m_pTraverser->VDirectoryExists(sDir);
    }
    catch (CRainmanException *pE)
    {
        delete[] sDir;
        return ResultFromExceptionT<bool>(pE);
    }

    delete[] sDir;
    return Result<bool>::Ok(bExists);
}

Result<tLastWriteTime> FileService::GetLastWriteTime(const wxString &sPath)
{
    char *sFile = wxStringToAscii(sPath);
    if (!sFile)
        return Result<tLastWriteTime>::Err(wxT("Memory allocation error"));

    tLastWriteTime t;
    try
    {
        t = m_pTraverser->VGetLastWriteTime(sFile);
    }
    catch (CRainmanException *pE)
    {
        delete[] sFile;
        return ResultFromExceptionT<tLastWriteTime>(pE);
    }

    delete[] sFile;
    return Result<tLastWriteTime>::Ok(t);
}
