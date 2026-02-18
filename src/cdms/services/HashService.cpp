#include "HashService.h"
#include "common/strconv.h"
#include <rainman/core/Exception.h>
#include <rainman/formats/CRgdHashTable.h>
#include <rainman/io/CFileSystemStore.h>
#include <rainman/io/IDirectoryTraverser.h>

HashService::~HashService() { Shutdown(); }

Result<void> HashService::Initialize(const wxString &sDictionaryPath)
{
    // Clean up any previous state
    Shutdown();

    m_pHashTable = std::make_unique<CRgdHashTable>();

    auto sDicPath = wxStringToAscii(sDictionaryPath);
    if (!sDicPath)
    {
        m_pHashTable.reset();
        return Result<void>::Err(wxT("Memory allocation error"));
    }

    CFileSystemStore oStore;
    std::unique_ptr<IDirectoryTraverser::IIterator> pItr;
    try
    {
        pItr.reset(oStore.VIterate(sDicPath.get()));
    }
    catch (const CRainmanException &e)
    {
        m_pHashTable.reset();
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
                    m_sCustomOutPath = pItr->VGetFullPath();
                }
                m_pHashTable->ExtendWithDictionary(pItr->VGetFullPath(),
                                                   (stricmp(pItr->VGetName(), "custom.txt") == 0));
            }
        } while (pItr->VNextItem() == IDirectoryTraverser::IIterator::E_OK);
    }
    catch (const CRainmanException &e)
    {
        m_sCustomOutPath.clear();
        m_pHashTable.reset();
        return ResultFromException(e);
    }

    return Result<void>::Ok();
}

void HashService::Shutdown()
{
    if (m_pHashTable)
    {
        if (!m_sCustomOutPath.empty())
            m_pHashTable->SaveCustomKeys(m_sCustomOutPath.c_str());
        m_pHashTable.reset();
    }
    m_sCustomOutPath.clear();
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
