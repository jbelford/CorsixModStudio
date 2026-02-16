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

#ifndef _CDMS_HASH_SERVICE_H_
#define _CDMS_HASH_SERVICE_H_

#include <wx/string.h>
#include "Result.h"
#include <vector>

class CRgdHashTable;

//! Service adapter wrapping CRgdHashTable with lazy initialization.
/*!
    Manages the lifecycle of CRgdHashTable, including loading dictionary
    files from the Mod_Studio_Files/Dictionaries directory. The hash table
    is initialized lazily on first access.

    Does NOT own the CRgdHashTable after GetHashTable() — ConstructFrame
    retains ownership for save-on-exit semantics.
*/
class HashService
{
  public:
    HashService() : m_pHashTable(nullptr), m_sCustomOutPath(nullptr) {}
    ~HashService();

    HashService(const HashService &) = delete;
    HashService &operator=(const HashService &) = delete;

    //! Initialize the hash table by loading dictionaries from the given path.
    /*!
        Iterates the dictionary directory, loading each .txt file.
        Remembers the path to custom.txt for save-on-exit.
        \param sDictionaryPath Path to the dictionaries directory.
        \return Result<void> — Err on failure.
    */
    Result<void> Initialize(const wxString &sDictionaryPath);

    //! Get the hash table (must call Initialize first).
    CRgdHashTable *GetHashTable() const { return m_pHashTable; }

    //! Whether the hash table has been initialized.
    bool IsInitialized() const { return m_pHashTable != nullptr; }

    //! Save custom keys and release the hash table.
    void Shutdown();

    //! Get the custom output path (for SaveCustomKeys).
    const char *GetCustomOutPath() const { return m_sCustomOutPath; }

    //! Cross-reference hash table with a string list file.
    Result<void> CrossRefWithStringList(const wxString &sFile);

    //! Fill a vector with unknown (un-resolved) hash values.
    void FillUnknownList(std::vector<unsigned long> &oList);

    //! Hash a string value, registering it if not already known.
    unsigned long ValueToHash(const char *sValue);

  private:
    CRgdHashTable *m_pHashTable;
    char *m_sCustomOutPath;
};

#endif
