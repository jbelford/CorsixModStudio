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

#ifndef _CDMS_FILE_SERVICE_H_
#define _CDMS_FILE_SERVICE_H_

#include <wx/string.h>
#include "Result.h"
#include <rainman/io/IFileStore.h>
#include <rainman/io/IDirectoryTraverser.h>
#include <memory>

//! RAII wrapper for IDirectoryTraverser::IIterator pointers.
class IteratorGuard
{
  public:
    explicit IteratorGuard(IDirectoryTraverser::IIterator *p = nullptr) noexcept : m_p(p) {}
    ~IteratorGuard() { delete m_p; }

    IteratorGuard(IteratorGuard &&other) noexcept : m_p(other.m_p) { other.m_p = nullptr; }
    IteratorGuard &operator=(IteratorGuard &&other) noexcept
    {
        if (this != &other)
        {
            delete m_p;
            m_p = other.m_p;
            other.m_p = nullptr;
        }
        return *this;
    }

    IteratorGuard(const IteratorGuard &) = delete;
    IteratorGuard &operator=(const IteratorGuard &) = delete;

    IDirectoryTraverser::IIterator *get() const noexcept { return m_p; }
    IDirectoryTraverser::IIterator *operator->() const noexcept { return m_p; }
    IDirectoryTraverser::IIterator *release() noexcept
    {
        auto *p = m_p;
        m_p = nullptr;
        return p;
    }
    explicit operator bool() const noexcept { return m_p != nullptr; }

  private:
    IDirectoryTraverser::IIterator *m_p;
};

//! Service adapter wrapping IFileStore and IDirectoryTraverser for the CDMS GUI layer.
/*!
    Provides RAII-managed stream and iterator access, translating between
    wxString and char* at the service boundary. Catches CRainmanException
    and returns Result<> for caller-friendly error handling.

    Does NOT own the IFileStore or IDirectoryTraverser — those are typically
    the CModuleFile owned by ConstructFrame.
*/
class FileService
{
  public:
    FileService() : m_pStore(nullptr), m_pTraverser(nullptr) {}
    FileService(IFileStore *pStore, IDirectoryTraverser *pTraverser) : m_pStore(pStore), m_pTraverser(pTraverser) {}

    void SetStore(IFileStore *pStore) { m_pStore = pStore; }
    void SetTraverser(IDirectoryTraverser *pTraverser) { m_pTraverser = pTraverser; }
    IFileStore *GetStore() const { return m_pStore; }
    IDirectoryTraverser *GetTraverser() const { return m_pTraverser; }

    // --- Stream operations (RAII) ---

    //! Open a read stream, returning a unique_ptr that auto-deletes on scope exit.
    Result<std::unique_ptr<IFileStore::IStream>> OpenStream(const wxString &sPath);

    //! Open a write stream, returning a unique_ptr.
    Result<std::unique_ptr<IFileStore::IOutputStream>> OpenOutputStream(const wxString &sPath, bool bEraseIfPresent);

    // --- Directory traversal ---

    //! Iterate a directory path, returning an RAII-guarded iterator.
    Result<IteratorGuard> Iterate(const wxString &sPath);

    //! Get the number of entry points in the traverser.
    unsigned long GetEntryPointCount() const;

    //! Get an entry point by index.
    wxString GetEntryPoint(unsigned long iID) const;

    //! Check whether a directory exists.
    Result<bool> DirectoryExists(const wxString &sPath);

    //! Get the last-write time of a path.
    Result<tLastWriteTime> GetLastWriteTime(const wxString &sPath);

    // --- Static utilities ---

    //! Iterate a local filesystem directory (not the module's virtual FS).
    static Result<IteratorGuard> IterateFileSystem(const wxString &sPath);

    //! Copy all data from an input stream to an output stream.
    static Result<void> CopyStream(IFileStore::IStream *pIn, IFileStore::IOutputStream *pOut);

  private:
    IFileStore *m_pStore;
    IDirectoryTraverser *m_pTraverser;
};

#endif
