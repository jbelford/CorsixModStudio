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

#include "rainman/module/CDoWFileView.h"
#include <algorithm>
#include "rainman/core/memdebug.h"
#include "rainman/core/Exception.h"

CDoWFileView::CDoWFileView() { _Clean(); }

void CDoWFileView::Reset() { _Clean(); }

CDoWFileView::~CDoWFileView() { _Clean(); }

void CDoWFileView::VInit(void *pUnused) { m_bInited = true; }

CDoWFileView::_VirtFile *CDoWFileView::_FindFile(const char *sPath, CDoWFileView::_VirtFolder **ipFolder)
{
    _VirtFolder *pFolder = &m_RootFolder;
    unsigned long iPartLength;
    const char *sSlashLoc = strchr(sPath, '\\');
    if (ipFolder)
        *ipFolder = nullptr;

    size_t iBSL, iBSH, iBSM;
    // Identify Correct Folder
    iPartLength = (unsigned long)(sSlashLoc ? sSlashLoc - sPath : strlen(sPath));
    while (sSlashLoc && iPartLength)
    {
        bool bFound = false;
        iBSL = 0;
        iBSH = pFolder->vChildFolders.size();
        iBSM = (iBSL + iBSH) >> 1;
        while (!bFound && iBSH > iBSL)
        {
            int iNiRes = strnicmp(pFolder->vChildFolders[iBSM]->sName.c_str(), sPath, iPartLength);
            if (iNiRes > 0)
            {
                iBSH = iBSM;
                iBSM = (iBSL + iBSH) >> 1;
            }
            else if (iNiRes < 0)
            {
                iBSL = iBSM + 1;
                iBSM = (iBSL + iBSH) >> 1;
            }
            else
            {
                if (pFolder->vChildFolders[iBSM]->sName.size() != iPartLength)
                {
                    iBSH = iBSM;
                    iBSM = (iBSL + iBSH) >> 1;
                }
                else
                {
                    pFolder = pFolder->vChildFolders[iBSM].get();
                    bFound = true;
                }
            }
        }
        if (!bFound)
            throw CRainmanException(nullptr, __FILE__, __LINE__, "Could not find \'%s\'", sPath);

        sPath += iPartLength + (sSlashLoc ? 1 : 0);
        sSlashLoc = strchr(sPath, '\\');
        iPartLength = (unsigned long)(sSlashLoc ? sSlashLoc - sPath : strlen(sPath));
    }
    if (iPartLength == 0)
        throw CRainmanException(__FILE__, __LINE__, "No file name");

    // Find file in folder
    if (ipFolder)
        *ipFolder = pFolder;
    iBSL = 0;
    iBSH = pFolder->vChildFiles.size();
    iBSM = (iBSL + iBSH) >> 1;
    while (iBSH > iBSL)
    {
        int iNiRes = strnicmp(pFolder->vChildFiles[iBSM]->sName.c_str(), sPath, iPartLength);
        if (iNiRes > 0)
        {
            iBSH = iBSM;
            iBSM = (iBSL + iBSH) >> 1;
        }
        else if (iNiRes < 0)
        {
            iBSL = iBSM + 1;
            iBSM = (iBSL + iBSH) >> 1;
        }
        else
        {
            if (pFolder->vChildFiles[iBSM]->sName.size() != iPartLength)
            {
                iBSH = iBSM;
                iBSM = (iBSL + iBSH) >> 1;
            }
            else
            {
                return pFolder->vChildFiles[iBSM].get();
            }
        }
    }
    /*
    for(std::vector<_VirtFile*>::iterator itr = pFolder->vChildFiles.begin(); itr != pFolder->vChildFiles.end(); ++itr)
    {
        if(strlen((**itr).sName) == iPartLength && strnicmp((**itr).sName, sPath, iPartLength) == 0)
        {
            return *itr;
        }
    }
    */
    throw CRainmanException(nullptr, __FILE__, __LINE__, "Could not find \'%s\'", sPath);
}

void CDoWFileView::VCreateFolderIn(const char *sPath, const char *sNewFolderName)
{
    //! Todo
    throw CRainmanException(__FILE__, __LINE__, "TODO: Need to implement this :D");
}

bool CDoWFileView::_SortFolds(const std::unique_ptr<CDoWFileView::_VirtFolder> &a,
                              const std::unique_ptr<CDoWFileView::_VirtFolder> &b)
{
    return (stricmp(a->sName.c_str(), b->sName.c_str()) < 0);
}

bool CDoWFileView::_SortFiles(const std::unique_ptr<CDoWFileView::_VirtFile> &a,
                              const std::unique_ptr<CDoWFileView::_VirtFile> &b)
{
    return (stricmp(a->sName.c_str(), b->sName.c_str()) < 0);
}

void CDoWFileView::_EnsureOutputFolder(_VirtFolder *pFolder, unsigned long *pSourceID)
{
    for (auto itr = pFolder->mapSourceFolderNames.begin(); itr != pFolder->mapSourceFolderNames.end(); ++itr)
    {
        if (m_vSourceFlags[itr->first].second)
        {
            *pSourceID = itr->first;
            return;
        }
    }
    if (pFolder->pParent)
    {
        try
        {
            _EnsureOutputFolder(pFolder->pParent, pSourceID);
            m_vSourceDirItrs[*pSourceID]->VCreateFolderIn(pFolder->pParent->mapSourceFolderNames[*pSourceID].c_str(),
                                                          pFolder->sName.c_str());
        }
        catch (const CRainmanException &e)
        {
            throw CRainmanException(e, __FILE__, __LINE__, "Cannot ensure output folder (%s)",
                                    pFolder->sFullName.c_str());
        }
        pFolder->mapSourceFolderNames[*pSourceID] =
            pFolder->pParent->mapSourceFolderNames[*pSourceID] + "\\" + pFolder->sName;
        return;
    }
    throw CRainmanException(nullptr, __FILE__, __LINE__, "Cannot ensure output folder (%s)",
                            pFolder->sFullName.c_str());
}

IFileStore::IOutputStream *CDoWFileView::VOpenOutputStream(const char *sFile, bool bEraseIfPresent)
{
    _VirtFolder *pFolder;
    _VirtFile *pFile;
    try
    {
        pFile = _FindFile(sFile, &pFolder);
    }
    catch (const CRainmanException &e)
    {
        pFile = nullptr;
        if (pFolder == nullptr)
        {
            throw CRainmanException(e, __FILE__, __LINE__, "Failed to file \'%s\'", sFile);
        }
    }
    if (pFile)
    {
        sFile = pFile->sName.c_str();
        if (m_vSourceFlags[pFile->iSourceID].first || m_vSourceFlags[pFile->iSourceID].second)
        {
            std::string sStoragePath = pFolder->mapSourceFolderNames[pFile->iSourceID];
            if (!sStoragePath.empty() && sStoragePath.back() != '\\')
                sStoragePath += '\\';
            sStoragePath += sFile;

            try
            {
                return m_vSourceStores[pFile->iSourceID]->VOpenOutputStream(sStoragePath.c_str(), bEraseIfPresent);
            }
            catch (const CRainmanException &e)
            {
                throw CRainmanException(e, __FILE__, __LINE__, "Cannot open \'%s\'", sFile);
            }
        }
    }
    if (pFolder || pFile)
    {
        if (!pFile)
        {
            sFile = strrchr(sFile, '\\') + 1;
        }
        unsigned long iSource;
        try
        {
            _EnsureOutputFolder(pFolder, &iSource);
        }
        catch (const CRainmanException &e)
        {
            throw CRainmanException(e, __FILE__, __LINE__, "Cannot ensure output folder for \'%s\'", sFile);
        }
        if (!(!pFile || (iSource < pFile->iSourceID)))
        {
            throw CRainmanException(nullptr, __FILE__, __LINE__, "Problem ensuring output folder for \'%s\'", sFile);
        }
        if (!pFile)
        {
            auto pNewFile = std::make_unique<_VirtFile>();
            pNewFile->bInReqMod = false;
            pNewFile->pParent = pFolder;
            pNewFile->sName = sFile;
            pFile = pNewFile.get();
            pFolder->vChildFiles.push_back(std::move(pNewFile));
            std::sort(pFolder->vChildFiles.begin(), pFolder->vChildFiles.end(), _SortFiles);
        }
        pFile->iSourceID = iSource;
        pFile->iModID = iSource; // it's the same :)
        std::string sStoragePath = pFolder->mapSourceFolderNames[pFile->iSourceID];
        if (!sStoragePath.empty() && sStoragePath.back() != '\\')
            sStoragePath += '\\';
        sStoragePath += pFile->sName;

        try
        {
            return m_vSourceStores[pFile->iSourceID]->VOpenOutputStream(sStoragePath.c_str(), bEraseIfPresent);
        }
        catch (const CRainmanException &e)
        {
            throw CRainmanException(e, __FILE__, __LINE__, "Failed to open source store for \'%s\'", sFile);
        }
    }
    throw CRainmanException(nullptr, __FILE__, __LINE__, "Failed to open \'%s\'", sFile);
}

IFileStore::IStream *CDoWFileView::VOpenStream(const char *sPath)
{
    _VirtFolder *pFolder;
    _VirtFile *pFile;
    try
    {
        pFile = _FindFile(sPath, &pFolder);
    }
    catch (const CRainmanException &e)
    {
        throw CRainmanException(e, __FILE__, __LINE__, "Could not find \'%s\'", sPath);
    }
    sPath = pFile->sName.c_str();
    std::string sStoragePath = pFolder->mapSourceFolderNames[pFile->iSourceID];
    if (!sStoragePath.empty() && sStoragePath.back() != '\\')
        sStoragePath += '\\';
    sStoragePath += sPath;

    try
    {
        return m_vSourceStores[pFile->iSourceID]->VOpenStream(sStoragePath.c_str());
    }
    catch (const CRainmanException &e)
    {
        throw CRainmanException(e, __FILE__, __LINE__, "Could not open source \'%s\'", sPath);
    }
}

CDoWFileView::CIterator::CIterator(_VirtFolder *pFolder, CDoWFileView *pStore)
{
    m_sParentPath = pFolder->sFullName;
    m_pStore = pStore;
    m_pDirectory = pFolder;
    m_FoldIter = pFolder->vChildFolders.begin();
    m_FileIter = pFolder->vChildFiles.begin();
    m_iWhat = 1; // 1 = folder, 2 = file, 0 = nothing
    if (m_FoldIter == pFolder->vChildFolders.end())
    {
        m_iWhat = 2;
        if (m_FileIter == pFolder->vChildFiles.end())
        {
            m_iWhat = 0;
        }
        else
        {
            m_sFullPath = m_sParentPath;
            if (!m_sFullPath.empty())
                m_sFullPath += '\\';
            m_sFullPath += (**m_FileIter).sName;
        }
    }
    else
    {
        m_sFullPath = m_sParentPath;
        if (!m_sFullPath.empty())
            m_sFullPath += '\\';
        m_sFullPath += (**m_FoldIter).sName;
    }
}

CDoWFileView::CIterator::~CIterator() = default;

IDirectoryTraverser::IIterator::eTypes CDoWFileView::CIterator::VGetType()
{
    if (m_iWhat == 1)
        return T_Directory;
    if (m_iWhat == 2)
        return T_File;
    return T_Nothing;
}

IDirectoryTraverser::IIterator *CDoWFileView::CIterator::VOpenSubDir()
{
    if (m_iWhat != 1)
        throw CRainmanException(__FILE__, __LINE__, "Can only open directories");
    auto *pItr = new CIterator(m_FoldIter->get(), m_pStore);
    if (pItr && pItr->m_iWhat == 0)
    {
        delete pItr;
        pItr = nullptr;
    }
    return pItr;
}

IFileStore::IStream *CDoWFileView::CIterator::VOpenFile()
{
    try
    {
        return m_pStore->VOpenStream(m_sFullPath.c_str());
    }
    catch (const CRainmanException &e)
    {
        throw CRainmanException(e, __FILE__, __LINE__, "Could not open \'%s\'", m_sFullPath.c_str());
    }
}

const char *CDoWFileView::CIterator::VGetName()
{
    if (m_iWhat == 1)
        return (**m_FoldIter).sName.c_str();
    if (m_iWhat == 2)
        return (**m_FileIter).sName.c_str();
    throw CRainmanException(__FILE__, __LINE__, "No item");
}

const char *CDoWFileView::CIterator::VGetFullPath() { return m_sFullPath.c_str(); }

const char *CDoWFileView::CIterator::VGetDirectoryPath() { return m_sParentPath.c_str(); }

tLastWriteTime CDoWFileView::VGetLastWriteTime(const char *sPath)
{
    _VirtFolder *pFolder;
    _VirtFile *pFile;
    try
    {
        pFile = _FindFile(sPath, &pFolder);
    }
    catch (const CRainmanException &e)
    {
        throw CRainmanException(e, __FILE__, __LINE__, "Cannot find file \'%s\'", sPath);
    }
    if (pFile->bGotWriteTime)
        return pFile->oWriteTime;

    sPath = pFile->sName.c_str();
    std::string sStoragePath = pFolder->mapSourceFolderNames[pFile->iSourceID];
    if (!sStoragePath.empty() && sStoragePath.back() != '\\')
        sStoragePath += '\\';
    sStoragePath += sPath;

    pFile->bGotWriteTime = true;
    try
    {
        pFile->oWriteTime = m_vSourceDirItrs[pFile->iSourceID]->VGetLastWriteTime(sStoragePath.c_str());
    }
    catch (const CRainmanException &e)
    {
        throw CRainmanException(e, __FILE__, __LINE__, "Cannot get source write time for file \'%s\'", sPath);
    }
    return pFile->oWriteTime;
}

tLastWriteTime CDoWFileView::CIterator::VGetLastWriteTime()
{
    if (m_iWhat == 2)
    {
        _VirtFile *pFile = m_FileIter->get();
        if (pFile->bGotWriteTime)
            return pFile->oWriteTime;

        std::string sStoragePath = m_pDirectory->mapSourceFolderNames[pFile->iSourceID];
        if (!sStoragePath.empty() && sStoragePath.back() != '\\')
            sStoragePath += '\\';
        sStoragePath += pFile->sName;

        pFile->bGotWriteTime = true;
        try
        {
            pFile->oWriteTime = m_pStore->m_vSourceDirItrs[pFile->iSourceID]->VGetLastWriteTime(sStoragePath.c_str());
        }
        catch (const CRainmanException &e)
        {
            throw CRainmanException(e, __FILE__, __LINE__, "Cannot get source write time for file \'%s\'",
                                    pFile->sName.c_str());
        }
        return pFile->oWriteTime;
    }
    throw CRainmanException(__FILE__, __LINE__, "Can only get write times for files");
}

IDirectoryTraverser::IIterator::eErrors CDoWFileView::CIterator::VNextItem()
{
    if (m_iWhat == 0)
        throw CRainmanException(__FILE__, __LINE__, "Nothing here");
    if (m_iWhat == 1)
    {
        if (++m_FoldIter == m_pDirectory->vChildFolders.end())
        {
            m_iWhat = 2;
            if (m_FileIter == m_pDirectory->vChildFiles.end())
            {
                m_iWhat = 0;
                return E_AtEnd;
            }
            goto skip_to_files;
        }
        else
        {
            m_sFullPath = m_sParentPath;
            if (!m_sFullPath.empty())
                m_sFullPath += '\\';
            m_sFullPath += (**m_FoldIter).sName;
        }
    }
    if (m_iWhat == 2)
    {
        if (++m_FileIter == m_pDirectory->vChildFiles.end())
        {
            m_iWhat = 0;
            return E_AtEnd;
        }
        else
        {
        skip_to_files:
            m_sFullPath = m_sParentPath;
            if (!m_sFullPath.empty())
                m_sFullPath += '\\';
            m_sFullPath += (**m_FileIter).sName;
        }
    }

    return E_OK;
}

void *CDoWFileView::CIterator::VGetTag(long iTag) // 0 = ModName | 1 = Source Name
{
    if (m_iWhat != 2)
        throw CRainmanException(__FILE__, __LINE__, "Invalid tag");

    switch (iTag)
    {
    case 0:
        return const_cast<char *>(m_pStore->m_vModNames[(**m_FileIter).iModID].c_str());
    case 1:
        return const_cast<char *>(m_pStore->m_vSourceNames[(**m_FileIter).iSourceID].c_str());
    default:
        throw CRainmanException(__FILE__, __LINE__, "Invalid tag");
    };
}

IDirectoryTraverser::IIterator *CDoWFileView::VIterate(const char *sPath)
{
    _VirtFolder *pFolder = &m_RootFolder;
    unsigned long iPartLength;
    const char *sSlashLoc = strchr(sPath, '\\');

    iPartLength = (unsigned long)(sSlashLoc ? sSlashLoc - sPath : strlen(sPath));
    while (iPartLength)
    {
        bool bFound = false;
        for (auto itr = pFolder->vChildFolders.begin(); itr != pFolder->vChildFolders.end(); ++itr)
        {
            if ((**itr).sName.size() == iPartLength && strnicmp((**itr).sName.c_str(), sPath, iPartLength) == 0)
            {
                pFolder = itr->get();
                bFound = true;
                break;
            }
        }
        if (!bFound)
            throw CRainmanException(nullptr, __FILE__, __LINE__, "Could not find \'%s\'", sPath);

        sPath += iPartLength + (sSlashLoc ? 1 : 0);
        sSlashLoc = strchr(sPath, '\\');
        iPartLength = (unsigned long)(sSlashLoc ? sSlashLoc - sPath : strlen(sPath));
    }

    auto *pItr = new CIterator(pFolder, (CDoWFileView *)this);
    if (pItr && pItr->VGetType() == IDirectoryTraverser::IIterator::T_Nothing)
    {
        delete pItr;
        pItr = nullptr;
    }
    return pItr;
}

unsigned long CDoWFileView::VGetEntryPointCount() { return 1; }

const char *CDoWFileView::VGetEntryPoint(unsigned long iID) { return m_RootFolder.sName.c_str(); }

void CDoWFileView::AddFileSource(IDirectoryTraverser *pTrav, IDirectoryTraverser::IIterator *pDirectory,
                                 IFileStore *pIOStore, const char *sMod, const char *sSourceType, bool bIsReqMod,
                                 bool bCanWrite, bool bIsOutput)
{
    if (pDirectory == nullptr)
        throw CRainmanException(__FILE__, __LINE__, "Null pDirectory");
    if (pIOStore == nullptr)
        throw CRainmanException(__FILE__, __LINE__, "Null pIOStore");

    auto iModID = static_cast<unsigned long>(m_vModNames.size());
    auto iSourceID = static_cast<unsigned long>(m_vSourceNames.size());

    m_vModNames.emplace_back(sMod);
    m_vSourceNames.emplace_back(sSourceType);
    m_vSourceStores.push_back(pIOStore);
    m_vSourceFlags.emplace_back(bCanWrite, bIsOutput);
    m_vSourceDirItrs.push_back(pTrav);

    try
    {
        _RawMapFolder(iModID, iSourceID, pDirectory, &m_RootFolder, bIsReqMod);
    }
    catch (const CRainmanException &e)
    {
        throw CRainmanException(e, __FILE__, __LINE__, "Raw map failed");
    }
}

CDoWFileView::_VirtFile::_VirtFile()
{
    pParent = nullptr;
    bInReqMod = false;
    bGotWriteTime = false;
    oWriteTime = GetInvalidWriteTime();
}

CDoWFileView::_VirtFolder::_VirtFolder() { pParent = nullptr; }

void CDoWFileView::_Clean()
{
    m_RootFolder.mapSourceFolderNames.clear();
    m_RootFolder.vChildFiles.clear();
    m_RootFolder.vChildFolders.clear();

    m_vModNames.clear();
    m_vSourceNames.clear();
    m_vSourceStores.clear();
}

void CDoWFileView::_RawMapFolder(unsigned long iModID, unsigned long iSourceID,
                                 IDirectoryTraverser::IIterator *pSourceDirectory, _VirtFolder *pDestination,
                                 bool bIsReqMod)
{
    if (pSourceDirectory == nullptr)
        throw CRainmanException(__FILE__, __LINE__, "No source directory");

    if (pDestination->mapSourceFolderNames.find(iSourceID) == pDestination->mapSourceFolderNames.end())
    {
        pDestination->mapSourceFolderNames[iSourceID] = pSourceDirectory->VGetDirectoryPath();
    }

    while (pSourceDirectory->VGetType() != IDirectoryTraverser::IIterator::T_Nothing)
    {
        if (pSourceDirectory->VGetType() == IDirectoryTraverser::IIterator::T_File)
        {
            _VirtFile *pOldFile = nullptr;
            for (auto itr = pDestination->vChildFiles.begin(); !pOldFile && itr != pDestination->vChildFiles.end();
                 ++itr)
            {
                if (stricmp((**itr).sName.c_str(), pSourceDirectory->VGetName()) == 0)
                {
                    pOldFile = itr->get();
                    break;
                }
            }
            if (!pOldFile)
            {
                auto pNewFile = std::make_unique<_VirtFile>();
                if (pNewFile->bInReqMod = bIsReqMod) // NOLINT(bugprone-assignment-in-if-condition)
                {
                    //! \todo Establish if anything was meant to go here
                }
                pNewFile->sName = pSourceDirectory->VGetName();
                pNewFile->pParent = pDestination;
                pNewFile->iModID = iModID;
                pNewFile->iSourceID = iSourceID;
                pNewFile->bGotWriteTime = true;
                pNewFile->oWriteTime = pSourceDirectory->VGetLastWriteTime();
                pDestination->vChildFiles.push_back(std::move(pNewFile));
            }
            else if (!pOldFile->bInReqMod)
            {
                pOldFile->bInReqMod = true;
                pOldFile->iReqModID = iModID;
                pOldFile->iReqSourceID = iSourceID;
            }
        }
        else
        {
            _VirtFolder *pTarget = nullptr;
            for (auto itr = pDestination->vChildFolders.begin(); !pTarget && itr != pDestination->vChildFolders.end();
                 ++itr)
            {
                if (stricmp((**itr).sName.c_str(), pSourceDirectory->VGetName()) == 0)
                    pTarget = itr->get();
            }
            if (!pTarget)
            {
                auto pNewFolder = std::make_unique<_VirtFolder>();
                pNewFolder->pParent = pDestination;
                pNewFolder->sName = pSourceDirectory->VGetName();
                pNewFolder->sFullName = pDestination->sFullName;
                if (!pNewFolder->sFullName.empty())
                    pNewFolder->sFullName += '\\';
                pNewFolder->sFullName += pNewFolder->sName;

                pTarget = pNewFolder.get();
                pDestination->vChildFolders.push_back(std::move(pNewFolder));
            }

            IDirectoryTraverser::IIterator *pItr = pSourceDirectory->VOpenSubDir();
            try
            {
                _RawMapFolder(iModID, iSourceID, pItr, pTarget, bIsReqMod);
            }
            catch (const CRainmanException &e)
            {
                delete pItr;
                throw CRainmanException(e, __FILE__, __LINE__, "Raw map of child failed");
            }
            delete pItr;
        }

        if (pSourceDirectory->VNextItem() != IDirectoryTraverser::IIterator::E_OK)
            break;
    }

    std::sort(pDestination->vChildFiles.begin(), pDestination->vChildFiles.end(), _SortFiles);
    std::sort(pDestination->vChildFolders.begin(), pDestination->vChildFolders.end(), _SortFolds);
}
