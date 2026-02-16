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
#include <rainman/io/CFileSystemStore.h>
#include <memory>

// --- Stream operations ---

Result<std::unique_ptr<IFileStore::IStream>> FileService::OpenStream(const wxString &sPath)
{
    auto sFile = wxStringToAscii(sPath);
    if (!sFile)
        return Result<std::unique_ptr<IFileStore::IStream>>::Err(wxT("Memory allocation error"));

    IFileStore::IStream *pStream = nullptr;
    try
    {
        pStream = m_pStore->VOpenStream(sFile.get());
    }
    catch (CRainmanException *pE)
    {
        return ResultFromExceptionT<std::unique_ptr<IFileStore::IStream>>(pE);
    }

    if (!pStream)
        return Result<std::unique_ptr<IFileStore::IStream>>::Err(wxT("Cannot open file"));

    return Result<std::unique_ptr<IFileStore::IStream>>::Ok(std::unique_ptr<IFileStore::IStream>(pStream));
}

Result<std::unique_ptr<IFileStore::IOutputStream>> FileService::OpenOutputStream(const wxString &sPath,
                                                                                 bool bEraseIfPresent)
{
    auto sFile = wxStringToAscii(sPath);
    if (!sFile)
        return Result<std::unique_ptr<IFileStore::IOutputStream>>::Err(wxT("Memory allocation error"));

    IFileStore::IOutputStream *pStream = nullptr;
    try
    {
        pStream = m_pStore->VOpenOutputStream(sFile.get(), bEraseIfPresent);
    }
    catch (CRainmanException *pE)
    {
        return ResultFromExceptionT<std::unique_ptr<IFileStore::IOutputStream>>(pE);
    }

    if (!pStream)
        return Result<std::unique_ptr<IFileStore::IOutputStream>>::Err(wxT("Cannot open output stream"));

    return Result<std::unique_ptr<IFileStore::IOutputStream>>::Ok(std::unique_ptr<IFileStore::IOutputStream>(pStream));
}

// --- Directory traversal ---

Result<IteratorGuard> FileService::Iterate(const wxString &sPath)
{
    auto sDir = wxStringToAscii(sPath);
    if (!sDir)
        return Result<IteratorGuard>::Err(wxT("Memory allocation error"));

    IDirectoryTraverser::IIterator *pItr = nullptr;
    try
    {
        pItr = m_pTraverser->VIterate(sDir.get());
    }
    catch (CRainmanException *pE)
    {
        return ResultFromExceptionT<IteratorGuard>(pE);
    }

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
    auto sDir = wxStringToAscii(sPath);
    if (!sDir)
        return Result<bool>::Err(wxT("Memory allocation error"));

    bool bExists = false;
    try
    {
        bExists = m_pTraverser->VDirectoryExists(sDir.get());
    }
    catch (CRainmanException *pE)
    {
        return ResultFromExceptionT<bool>(pE);
    }

    return Result<bool>::Ok(bExists);
}

Result<tLastWriteTime> FileService::GetLastWriteTime(const wxString &sPath)
{
    auto sFile = wxStringToAscii(sPath);
    if (!sFile)
        return Result<tLastWriteTime>::Err(wxT("Memory allocation error"));

    tLastWriteTime t;
    try
    {
        t = m_pTraverser->VGetLastWriteTime(sFile.get());
    }
    catch (CRainmanException *pE)
    {
        return ResultFromExceptionT<tLastWriteTime>(pE);
    }

    return Result<tLastWriteTime>::Ok(t);
}

// --- Static utilities ---

Result<IteratorGuard> FileService::IterateFileSystem(const wxString &sPath)
{
    auto sDir = wxStringToAscii(sPath);
    if (!sDir)
        return Result<IteratorGuard>::Err(wxT("Memory allocation error"));

    CFileSystemStore oStore;
    IDirectoryTraverser::IIterator *pItr = nullptr;
    try
    {
        pItr = oStore.VIterate(sDir.get());
    }
    catch (CRainmanException *pE)
    {
        return ResultFromExceptionT<IteratorGuard>(pE);
    }

    return Result<IteratorGuard>::Ok(IteratorGuard(pItr));
}

Result<void> FileService::CopyStream(IFileStore::IStream *pIn, IFileStore::IOutputStream *pOut)
{
    constexpr long kBufSize = 4194304; // 4 MB, matches existing extract pattern
    auto pBuffer = std::make_unique<char[]>(kBufSize);

    try
    {
        pIn->VSeek(0, IFileStore::IStream::SL_End);
        long iLen = pIn->VTell();
        pIn->VSeek(0, IFileStore::IStream::SL_Root);

        while (iLen > 0)
        {
            long iChunk = (iLen > kBufSize) ? kBufSize : iLen;
            pIn->VRead(iChunk, 1, pBuffer.get());
            pOut->VWrite(iChunk, 1, pBuffer.get());
            iLen -= iChunk;
        }
    }
    catch (CRainmanException *pE)
    {
        return ResultFromException(pE);
    }

    return Result<void>::Ok();
}
