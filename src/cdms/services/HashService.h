#pragma once

#include <wx/string.h>
#include "Result.h"
#include <memory>
#include <string>
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
    HashService() = default;
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
    CRgdHashTable *GetHashTable() const { return m_pHashTable.get(); }

    //! Whether the hash table has been initialized.
    bool IsInitialized() const { return m_pHashTable != nullptr; }

    //! Save custom keys and release the hash table.
    void Shutdown();

    //! Get the custom output path (for SaveCustomKeys).
    const char *GetCustomOutPath() const { return m_sCustomOutPath.empty() ? nullptr : m_sCustomOutPath.c_str(); }

    //! Cross-reference hash table with a string list file.
    Result<void> CrossRefWithStringList(const wxString &sFile);

    //! Fill a vector with unknown (un-resolved) hash values.
    void FillUnknownList(std::vector<unsigned long> &oList);

    //! Hash a string value, registering it if not already known.
    unsigned long ValueToHash(const char *sValue);

  private:
    std::unique_ptr<CRgdHashTable> m_pHashTable;
    std::string m_sCustomOutPath;
};
