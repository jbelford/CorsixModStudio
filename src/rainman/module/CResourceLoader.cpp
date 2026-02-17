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

#include "rainman/module/CResourceLoader.h"
#include "rainman/module/CModuleFile.h"
#include "rainman/archive/CSgaFile.h"
#include "rainman/localization/CUcsFile.h"
#include "rainman/module/CFileMap.h"
#include "rainman/io/CFileSystemStore.h"
#include "rainman/core/CThreadPool.h"
#include "rainman/core/Internal_Util.h"
#include "rainman/core/Exception.h"
#include "rainman/core/RainmanLog.h"
#include <future>
#include <chrono>
#include <memory>
#include <string>

// -- Parallel archive preloading infrastructure (Phase J) --

//! Metadata for one archive to be loaded in parallel.
struct CResourceLoader::ArchiveTask
{
    std::string archivePath;
    std::string uiName;
    unsigned short iNum;
    CSgaFile **ppSgaOut; // Where to store the loaded CSgaFile*
};

//! Result of preloading an SGA file on a worker thread.
struct CResourceLoader::SgaPreloadResult
{
    CSgaFile *pSga = nullptr;
    bool bIsThisMod = true;
    char *sActualModName = nullptr; // malloc'd if set; consumer must free

    SgaPreloadResult() = default;
    ~SgaPreloadResult()
    {
        if (sActualModName)
            free(sActualModName); // NOLINT(clang-analyzer-unix.MismatchedDeallocator)
    }
    SgaPreloadResult(SgaPreloadResult &&o) noexcept
        : pSga(o.pSga), bIsThisMod(o.bIsThisMod), sActualModName(o.sActualModName)
    {
        o.pSga = nullptr;
        o.sActualModName = nullptr;
    }
    SgaPreloadResult &operator=(SgaPreloadResult &&o) noexcept
    {
        if (this != &o)
        {
            pSga = o.pSga;
            bIsThisMod = o.bIsThisMod;
            sActualModName = o.sActualModName;
            o.pSga = nullptr;
            o.sActualModName = nullptr;
        }
        return *this;
    }
    SgaPreloadResult(const SgaPreloadResult &) = delete;
    SgaPreloadResult &operator=(const SgaPreloadResult &) = delete;
};

//! Loads and initializes a single SGA file without registering it in the file map.
//! Safe to call concurrently for different archive paths — each call creates
//! independent CSgaFile and IStream instances.
CResourceLoader::SgaPreloadResult CResourceLoader::PreloadSga(CFileSystemStore *pFSS, const char *sFullPath,
                                                              CModuleFile::eModuleType eModuleType,
                                                              const char *sCohThisModFolder,
                                                              const char *sApplicationPath,
                                                              const char *saScenarioPackRootFolder)
{
    CResourceLoader::SgaPreloadResult result;
    auto *pSga = CHECK_MEM(new CSgaFile);
    std::unique_ptr<IFileStore::IStream> inputStream;
    try
    {
        inputStream = std::unique_ptr<IFileStore::IStream>(pFSS->VOpenStream(sFullPath));
    }
    catch (CRainmanException *pE)
    {
        auto guard = std::unique_ptr<CRainmanException, ExceptionDeleter>(pE);
        delete pSga;
        return result; // File not found — return empty result
    }

    // CoH-specific: determine if archive belongs to this mod or a dependency
    if (eModuleType == CModuleFile::MT_CompanyOfHeroes)
    {
        if (strnicmp(sCohThisModFolder, sFullPath, strlen(sCohThisModFolder)) != 0)
        {
            if (saScenarioPackRootFolder &&
                strnicmp(saScenarioPackRootFolder, sFullPath, strlen(saScenarioPackRootFolder)) == 0)
            {
                result.bIsThisMod = false;
                result.sActualModName = strdup(sFullPath + strlen(saScenarioPackRootFolder) + 1);
                char *sSlash = strrchr(result.sActualModName, '\\');
                if (sSlash)
                    *sSlash = 0;
            }
            else
            {
                result.bIsThisMod = false;
                result.sActualModName = strdup(sFullPath + strlen(sApplicationPath));
                char *sSlash = strrchr(result.sActualModName, '\\');
                if (sSlash)
                    *sSlash = 0;
                sSlash = strrchr(result.sActualModName, '\\');
                if (sSlash)
                    *sSlash = 0;
            }
        }
    }

    try
    {
        pSga->Load(inputStream.get(), pFSS->VGetLastWriteTime(sFullPath));
        pSga->VInit(inputStream.get());
    }
    catch (CRainmanException *pE)
    {
        pE = new CRainmanException(pE, __FILE__, __LINE__, "(rethrow for \'%s\')", sFullPath);
        delete pSga;
        if (result.sActualModName)
        {
            free(result.sActualModName); // NOLINT(clang-analyzer-unix.MismatchedDeallocator)
            result.sActualModName = nullptr;
        }
        throw pE;
    }

    (void)inputStream.release(); // CSgaFile takes ownership of the stream
    result.pSga = pSga;
    return result;
}

//! Registers a preloaded SGA into the file map (must be called on the main thread).
void CResourceLoader::RegisterPreloadedSga(SgaPreloadResult &result, CModuleFile &module, unsigned short iNum,
                                           const char *sSlashChar, const char *sFullPath)
{
    IDirectoryTraverser::IIterator *pDirItr = nullptr;
    try
    {
        pDirItr = result.pSga->VIterate(result.pSga->VGetEntryPoint(0));
        if (pDirItr)
        {
            void *pSrc;
            if (result.bIsThisMod)
                pSrc = module.m_pNewFileMap->RegisterSource(module.m_iFileMapModNumber, true, iNum,
                                                            module.GetFileMapName(), sSlashChar, result.pSga,
                                                            result.pSga, false, false);
            else
                pSrc = module.m_pNewFileMap->RegisterSource(15000, true, iNum, result.sActualModName, sSlashChar,
                                                            result.pSga, result.pSga, false, false);
            module.m_pNewFileMap->MapSga(pSrc, result.pSga);
        }
        delete pDirItr;
    }
    catch (CRainmanException *pE)
    {
        pE = new CRainmanException(pE, __FILE__, __LINE__, "(rethrow for \'%s\')", sFullPath);
        delete pDirItr;
        delete result.pSga;
        result.pSga = nullptr;
        if (result.sActualModName)
        {
            free(result.sActualModName); // NOLINT(clang-analyzer-unix.MismatchedDeallocator)
            result.sActualModName = nullptr;
        }
        throw pE;
    }
    if (result.sActualModName)
    {
        free(result.sActualModName); // NOLINT(clang-analyzer-unix.MismatchedDeallocator)
        result.sActualModName = nullptr;
    }
}

//! Loads archives in parallel (Phase 1: async preload, Phase 2: sequential registration).
void CResourceLoader::LoadArchivesParallel(std::vector<ArchiveTask> &tasks, CModuleFile &module, CALLBACK_ARG)
{
    if (tasks.empty())
        return;

    auto tStart = std::chrono::steady_clock::now();

    // Phase 1: Launch parallel SGA preloads via thread pool
    auto &pool = CThreadPool::Instance();
    std::vector<std::future<SgaPreloadResult>> futures;
    futures.reserve(tasks.size());
    for (auto &task : tasks)
    {
        futures.push_back(pool.Submit(&CResourceLoader::PreloadSga, module.m_pFSS, task.archivePath.c_str(),
                                      module.m_eModuleType, module.m_sCohThisModFolder, module.m_sApplicationPath,
                                      module.m_saScenarioPackRootFolder));
    }

    // Phase 2: Collect results and register sequentially
    CRainmanException *pFirstError = nullptr;
    for (size_t i = 0; i < futures.size(); ++i)
    {
        SgaPreloadResult result;
        try
        {
            result = futures[i].get();
        }
        catch (CRainmanException *pE)
        {
            if (!pFirstError)
                pFirstError = pE;
            else
                pE->destroy();
            continue;
        }

        if (!result.pSga)
        {
            *tasks[i].ppSgaOut = nullptr;
            continue;
        }

        try
        {
            CallCallback(THE_CALLBACK, "Registering archive \'%s\' for mod \'%s\'", tasks[i].uiName.c_str(),
                         module.m_sFileMapName);
            RegisterPreloadedSga(result, module, tasks[i].iNum, tasks[i].uiName.c_str(), tasks[i].archivePath.c_str());
        }
        catch (CRainmanException *pE)
        {
            if (!pFirstError)
                pFirstError = pE;
            else
                pE->destroy();
            *tasks[i].ppSgaOut = nullptr;
            continue;
        }

        *tasks[i].ppSgaOut = result.pSga;
        result.pSga = nullptr; // Ownership transferred
    }

    auto tEnd = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(tEnd - tStart).count();
    RAINMAN_LOG_INFO("Parallel archive loading: {} archives in {}ms", tasks.size(), ms);

    if (pFirstError)
        throw pFirstError;
}

// -- Free-function callbacks used with Util_ForEach --
// These are declared as friends of the relevant nested handler classes in CModuleFile.h,
// so they can access protected members directly.

void CModuleFile_ArchiveForEach(IDirectoryTraverser::IIterator *pItr, void *pModuleFile)
{
    auto *pThis = static_cast<CModuleFile *>(pModuleFile);

    auto *pArchEntry = new CModuleFile::CArchiveHandler;
    pArchEntry->m_iNumber = -7291;
    pArchEntry->m_sName = strdup(pItr->VGetName());
    pThis->m_vArchives.push_back(pArchEntry);

    CResourceLoader::DoLoadArchive(*pThis, pItr->VGetFullPath(), &pArchEntry->m_pHandle, 15000, pArchEntry->m_sName);
}

void CModuleFile_ArchiveForEachNoErrors(IDirectoryTraverser::IIterator *pItr, void *pModuleFile)
{
    auto *pThis = static_cast<CModuleFile *>(pModuleFile);

    CSgaFile *pFile = nullptr;
    char *s = strdup(pItr->VGetName());
    try
    {
        CResourceLoader::DoLoadArchive(*pThis, pItr->VGetFullPath(), &pFile, 15000, s);
    }
    catch (CRainmanException *pE)
    {
        auto guard = std::unique_ptr<CRainmanException, ExceptionDeleter>(pE);
        free(s);
        return;
    }

    auto *pArchEntry = new CModuleFile::CArchiveHandler;
    pArchEntry->m_iNumber = -7291;
    pArchEntry->m_sName = s;
    pArchEntry->m_pHandle = pFile;
    pThis->m_vArchives.push_back(pArchEntry);
}

void CModuleFile_UcsForEach(IDirectoryTraverser::IIterator *pItr, void *pModuleFile)
{
    auto *pThis = static_cast<CModuleFile *>(pModuleFile);
    const char *sName = pItr->VGetName();
    const char *sDot = strrchr(sName, '.');
    if (sDot && ((stricmp(sDot, ".ucs") == 0) || (stricmp(sDot, ".dat") == 0)))
    {
        auto *pUcsEntry = new CModuleFile::CUcsHandler;
        pUcsEntry->m_sName = strdup(sName);
        pUcsEntry->m_pHandle = new CUcsFile;

        std::unique_ptr<IFileStore::IStream> stream;
        try
        {
            stream = std::unique_ptr<IFileStore::IStream>(pItr->VOpenFile());
            if (stricmp(sDot, ".ucs") == 0)
                pUcsEntry->m_pHandle->Load(stream.get());
            else
                pUcsEntry->m_pHandle->LoadDat(stream.get());
        }
        catch (CRainmanException *pE)
        {
            delete pUcsEntry->m_pHandle;
            free(pUcsEntry->m_sName);
            delete pUcsEntry;
            throw new CRainmanException(pE, __FILE__, __LINE__, "Error loading UCS file \'%s\'", sName);
        }

        pThis->m_vLocaleTexts.push_back(pUcsEntry);
    }
}

// -- Parallel UCS loading infrastructure --

//! Metadata for one UCS/DAT file to be loaded in parallel.
struct CResourceLoader::UcsFileInfo
{
    std::string fullPath;
    std::string name;
    bool isUcs; // true = .ucs, false = .dat
};

void CResourceLoader::LoadUcsFilesParallel(CModuleFile &module, IDirectoryTraverser::IIterator *pItr)
{
    // Phase 1 (serial): Collect file info from the directory iterator
    std::vector<UcsFileInfo> files;
    while (pItr->VGetType() != IDirectoryTraverser::IIterator::T_Nothing)
    {
        if (pItr->VGetType() == IDirectoryTraverser::IIterator::T_File)
        {
            const char *sName = pItr->VGetName();
            const char *sDot = strrchr(sName, '.');
            if (sDot && ((stricmp(sDot, ".ucs") == 0) || (stricmp(sDot, ".dat") == 0)))
            {
                UcsFileInfo info;
                info.fullPath = pItr->VGetFullPath();
                info.name = sName;
                info.isUcs = (stricmp(sDot, ".ucs") == 0);
                files.push_back(std::move(info));
            }
        }
        if (pItr->VNextItem() != IDirectoryTraverser::IIterator::E_OK)
            break;
    }

    if (files.empty())
        return;

    auto startTime = std::chrono::steady_clock::now();

    // Phase 2 (parallel): Load CUcsFile instances via thread pool
    struct UcsLoadResult
    {
        CUcsFile *pHandle = nullptr;
        std::string name;
        CRainmanException *pError = nullptr;
    };

    auto &pool = CThreadPool::Instance();
    std::vector<std::future<UcsLoadResult>> futures;
    futures.reserve(files.size());

    for (const auto &fileInfo : files)
    {
        futures.push_back(pool.Submit(
            [&fileInfo, pFSS = module.m_pFSS]() -> UcsLoadResult
            {
                UcsLoadResult result;
                result.name = fileInfo.name;
                result.pHandle = new CUcsFile;
                try
                {
                    std::unique_ptr<IFileStore::IStream> stream(pFSS->VOpenStream(fileInfo.fullPath.c_str()));
                    if (fileInfo.isUcs)
                        result.pHandle->Load(stream.get());
                    else
                        result.pHandle->LoadDat(stream.get());
                }
                catch (CRainmanException *pE)
                {
                    delete result.pHandle;
                    result.pHandle = nullptr;
                    result.pError = pE;
                }
                return result;
            }));
    }

    // Phase 3 (serial): Collect results and register in module
    CRainmanException *pFirstError = nullptr;
    std::string sFirstErrorFile;

    for (auto &fut : futures)
    {
        UcsLoadResult result = fut.get();
        if (result.pError != nullptr)
        {
            if (pFirstError == nullptr)
            {
                pFirstError = result.pError;
                sFirstErrorFile = result.name;
            }
            else
            {
                result.pError->destroy();
            }
            continue;
        }

        auto *pUcsEntry = new CModuleFile::CUcsHandler;
        pUcsEntry->m_sName = strdup(result.name.c_str());
        pUcsEntry->m_pHandle = result.pHandle;
        module.m_vLocaleTexts.push_back(pUcsEntry);
    }

    auto endTime = std::chrono::steady_clock::now();
    auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    RAINMAN_LOG_INFO("Parallel UCS loading: {} files in {} ms", files.size(), elapsedMs);

    if (pFirstError != nullptr)
    {
        throw new CRainmanException(pFirstError, __FILE__, __LINE__, "Error loading UCS file '%s'",
                                    sFirstErrorFile.c_str());
    }
}

// -- Parallel folder pre-scanning infrastructure --

//! Metadata for one folder to be pre-scanned in parallel.
struct CResourceLoader::FolderTask
{
    std::string folderPath;
    std::string uiName;
    std::string tocName;
    unsigned short iNum;
    bool bIsDefaultWrite;
    // CoH-specific: data source pointer (null for DoW/CoHEarly)
    CModuleFile::CCohDataSource *pDataSource = nullptr;
};

CFileMap::DirEntry CResourceLoader::ScanDirectory(IDirectoryTraverser::IIterator *pItr)
{
    CFileMap::DirEntry root;
    root.isFile = false;
    root.lastWriteTime = 0;
    if (pItr == nullptr)
        return root;

    root.directoryPath = pItr->VGetDirectoryPath();
    root.children.reserve(32);

    auto eType = pItr->VGetType();
    while (eType != IDirectoryTraverser::IIterator::T_Nothing)
    {
        CFileMap::DirEntry entry;

        if (eType == IDirectoryTraverser::IIterator::T_File)
        {
            entry.name = pItr->VGetName();
            entry.isFile = true;
            entry.lastWriteTime = pItr->VGetLastWriteTime();
        }
        else
        {
            std::string sName = pItr->VGetName();
            entry.isFile = false;
            entry.lastWriteTime = 0;
            auto *pSubItr = pItr->VOpenSubDir();
            if (pSubItr)
            {
                entry = ScanDirectory(pSubItr);
                delete pSubItr;
            }
            entry.name = std::move(sName);
        }
        root.children.push_back(std::move(entry));

        if (pItr->VNextItem() != IDirectoryTraverser::IIterator::E_OK)
            break;
        eType = pItr->VGetType();
    }
    return root;
}

void CResourceLoader::LoadFoldersParallel(std::vector<FolderTask> &tasks, CModuleFile &module, CALLBACK_ARG)
{
    if (tasks.empty())
        return;

    auto startTime = std::chrono::steady_clock::now();

    // Phase 1 (parallel): Pre-scan directory trees into memory
    struct ScanResult
    {
        CFileMap::DirEntry snapshot;
        bool hasIterator = false;
    };

    auto &pool = CThreadPool::Instance();
    std::vector<std::future<ScanResult>> futures;
    futures.reserve(tasks.size());

    for (const auto &task : tasks)
    {
        futures.push_back(pool.Submit(
            [&task, pFSS = module.m_pFSS]() -> ScanResult
            {
                ScanResult result;
                IDirectoryTraverser::IIterator *pDirItr = nullptr;
                try
                {
                    pDirItr = pFSS->VIterate(task.folderPath.c_str());
                }
                catch (CRainmanException *pE)
                {
                    pE->destroy();
                    return result;
                }
                if (pDirItr)
                {
                    result.snapshot = ScanDirectory(pDirItr);
                    result.hasIterator = true;
                    delete pDirItr;
                }
                return result;
            }));
    }

    // Phase 2 (serial): Register sources and replay snapshots into CFileMap
    for (size_t i = 0; i < tasks.size(); ++i)
    {
        ScanResult result = futures[i].get();
        if (!result.hasIterator)
            continue;

        const auto &task = tasks[i];

        const char *sSlashChar = task.uiName.empty() ? nullptr : task.uiName.c_str();
        if (!sSlashChar)
        {
            sSlashChar = strrchr(task.folderPath.c_str(), '\\');
            if (!sSlashChar)
                sSlashChar = strrchr(task.folderPath.c_str(), '/');
            if (sSlashChar)
                ++sSlashChar;
            else
                sSlashChar = task.folderPath.c_str();
        }

        CallCallback(THE_CALLBACK, "Registering data folder '%s' for mod '%s'", sSlashChar, module.m_sFileMapName);

        bool bIsThisMod = true;
        char *sActualModName = nullptr;
        if (module.m_eModuleType == CModuleFile::MT_CompanyOfHeroes)
        {
            if (strnicmp(module.m_sCohThisModFolder, task.folderPath.c_str(), strlen(module.m_sCohThisModFolder)) != 0)
            {
                bIsThisMod = false;
                sActualModName = strdup(task.folderPath.c_str() + strlen(module.m_sApplicationPath));
                char *sSlash = strrchr(sActualModName, '\\');
                if (sSlash)
                    *sSlash = 0;
            }
        }

        void *pSrc;
        if (bIsThisMod)
        {
            pSrc = module.m_pNewFileMap->RegisterSource(
                module.m_iFileMapModNumber, false, task.iNum, module.GetFileMapName(), sSlashChar, module.m_pFSS,
                module.m_pFSS, module.m_pParentModule ? false : true, task.bIsDefaultWrite);
            if (task.pDataSource)
                task.pDataSource->m_bCanWriteToFolder = module.m_pParentModule ? false : true;
        }
        else
        {
            pSrc = module.m_pNewFileMap->RegisterSource(15000, false, task.iNum, sActualModName, sSlashChar,
                                                        module.m_pFSS, module.m_pFSS, false, false);
        }

        module.m_pNewFileMap->MapSnapshot(pSrc, task.tocName.c_str(), result.snapshot);

        if (sActualModName)
            free(sActualModName);
    }

    auto endTime = std::chrono::steady_clock::now();
    auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    RAINMAN_LOG_INFO("Parallel folder loading: {} folders in {} ms", tasks.size(), elapsedMs);
}

// -- Private helper methods --

bool CResourceLoader::IsToBeLoaded(const CModuleFile &module, CModuleFile::CCohDataSource *pDataSource)
{
    return ((strcmp(pDataSource->m_sOption, "common") == 0) || (strcmp(pDataSource->m_sOption, "sound_high") == 0) ||
            (strcmp(pDataSource->m_sOption, "art_high") == 0) ||
            (stricmp(pDataSource->m_sOption, module.m_sLocale) == 0));
}

void CResourceLoader::LoadDataGeneric(CModuleFile &module, CALLBACK_ARG)
{
    char *sDataGenericValue = nullptr;
    char *sPipelineFile = new char[strlen(module.m_sApplicationPath) + 16];
    sprintf(sPipelineFile, "%spipeline.ini", module.m_sApplicationPath);
    FILE *fModule = fopen(sPipelineFile, "rb");
    if (fModule == nullptr)
    {
        delete[] sPipelineFile;
        return;
    }
    CallCallback(THE_CALLBACK, "Parsing file \'pipeline.ini\'");
    delete[] sPipelineFile;

    char *sSectionName = new char[16 + strlen(module.m_metadata.m_sModFolder)];
    sprintf(sSectionName, "project:%s", module.m_metadata.m_sModFolder);

    bool bInSection = false;
    while (!feof(fModule))
    {
        char *sLine = Util_fgetline(fModule);
        if (sLine == nullptr)
        {
            fclose(fModule);
            delete[] sSectionName;
            if (sDataGenericValue)
                free(sDataGenericValue);
            throw new CRainmanException(__FILE__, __LINE__, "Error reading from file");
        }
        char *sCommentBegin = strchr(sLine, ';');
        if (sCommentBegin)
            *sCommentBegin = 0;
        char *sEqualsChar = strchr(sLine, '=');
        if (sEqualsChar)
        {
            char *sKey = sLine;
            char *sValue = sEqualsChar + 1;
            *sEqualsChar = 0;
            Util_TrimWhitespace(&sKey);
            Util_TrimWhitespace(&sValue);
            if (bInSection)
            {
                try
                {
                    if (stricmp(sKey, "DataGeneric") == 0)
                    {
                        if (sDataGenericValue)
                            free(sDataGenericValue);
                        sDataGenericValue = CHECK_MEM(strdup(sValue));
                    }
                }
                catch (CRainmanException *pE)
                {
                    delete[] sLine;
                    delete[] sSectionName;
                    if (sDataGenericValue)
                        free(sDataGenericValue);
                    fclose(fModule);
                    throw pE;
                }
            }
        }
        else
        {
            char *sLeftBraceChar = strchr(sLine, '[');
            char *sRightBraceChar = sLeftBraceChar ? strchr(sLeftBraceChar, ']') : nullptr;
            if (sLeftBraceChar && sRightBraceChar)
            {
                *sRightBraceChar = 0;
                ++sLeftBraceChar;
                bInSection = false;
                if (stricmp(sLeftBraceChar, sSectionName) == 0)
                {
                    bInSection = true;
                }
            }
        }
        delete[] sLine;
    }
    delete[] sSectionName;
    fclose(fModule);

    if (sDataGenericValue)
    {
        char *sDataGenericFullPath = new char[strlen(module.m_sApplicationPath) + strlen(sDataGenericValue) + 1];
        sprintf(sDataGenericFullPath, "%s%s", module.m_sApplicationPath, sDataGenericValue);
        CResourceLoader::DoLoadFolder(module, sDataGenericFullPath, (module.m_pParentModule == nullptr), 99, "Generic",
                                      "DataGeneric", THE_CALLBACK);
        delete[] sDataGenericFullPath;
        // NOLINTNEXTLINE(clang-analyzer-unix.MismatchedDeallocator) -- strdup uses malloc; free() is correct
        free(sDataGenericValue);
    }
}

void CResourceLoader::LoadCohEngine(CModuleFile &module, const char *sFolder, const char *sUiName,
                                    unsigned long iLoadWhat, unsigned short iNum, CALLBACK_ARG)
{
    CModuleFile *pEngine = CHECK_MEM(new CModuleFile);

    pEngine->m_eModuleType = CModuleFile::MT_CompanyOfHeroesEarly;
    pEngine->m_metadata.m_sUiName = strdup(sUiName);
    pEngine->m_metadata.m_sDescription = strdup(sUiName);
    pEngine->m_sFileMapName = strdup(sUiName);
    pEngine->m_metadata.m_sModFolder = strdup(sFolder);

    free(pEngine->m_sLocale);
    pEngine->m_sLocale = module.m_sLocale;
    pEngine->m_pParentModule = &module;
    pEngine->m_pNewFileMap = module.m_pNewFileMap;
    pEngine->m_pFSS = module.m_pFSS;
    pEngine->m_sApplicationPath = module.m_sApplicationPath;
    pEngine->m_iFileMapModNumber = iNum;

    module.m_vEngines.push_back(pEngine);

    pEngine->ReloadResources(iLoadWhat & (~CModuleFile::RR_Engines), 0, 0, THE_CALLBACK);
}

// -- Public entry point --

void CResourceLoader::Load(CModuleFile &module, unsigned long iReloadWhat, unsigned long iReloadWhatRequiredMods,
                           unsigned long iReloadWhatEngines, CALLBACK_ARG)
{
    auto tLoadStart = std::chrono::steady_clock::now();
    module._CleanResources();

    if (module.m_pFSS == nullptr)
        module.m_pFSS = new CFileSystemStore;
    if (module.m_pNewFileMap == nullptr)
        module.m_pNewFileMap = new CFileMap;

    // === Cross-phase parallelism: UCS, Folders, Archives ===
    // These three phases are independent:
    //   UCS writes to m_vLocaleTexts (no CFileMap interaction)
    //   Folders and Archives write to m_pNewFileMap (mutex-protected)
    // Run them concurrently when 2+ are active for maximum I/O overlap.

    bool bRunUcs = (iReloadWhat & CModuleFile::RR_LocalisedText) != 0;
    bool bRunFolders = (iReloadWhat & CModuleFile::RR_DataFolders) != 0;
    bool bRunArchives = (iReloadWhat & CModuleFile::RR_DataArchives) != 0;

    // Pre-compute CoH data source load flags to avoid write races between phases
    if ((bRunFolders || bRunArchives) && module.m_eModuleType == CModuleFile::MT_CompanyOfHeroes)
    {
        for (auto *pDS : module.m_vDataSources)
        {
            pDS->m_bIsLoaded = IsToBeLoaded(module, pDS);
            pDS->m_bCanWriteToFolder = false;
        }
    }

    // --- UCS phase lambda ---
    auto ucsPhase = [&]() -> CRainmanException *
    {
        if (!bRunUcs)
            return nullptr;
        auto tPhaseStart = std::chrono::steady_clock::now();

        std::string sUcsPath;
        if (module.m_metadata.m_sLocFolder && *module.m_metadata.m_sLocFolder)
        {
            sUcsPath =
                std::string(module.m_sApplicationPath) + module.m_metadata.m_sLocFolder + "\\" + module.m_sLocale;
        }
        else
        {
            sUcsPath = std::string(module.m_sApplicationPath) + module.m_metadata.m_sModFolder + "\\Locale\\" +
                       module.m_sLocale;
        }

        IDirectoryTraverser::IIterator *pItr = nullptr;
        try
        {
            pItr = module.m_pFSS->VIterate(sUcsPath.c_str());
        }
        IGNORE_EXCEPTIONS

        if (pItr != nullptr)
        {
            try
            {
                CallCallback(THE_CALLBACK, "Loading UCS files for mod \'%s\'", module.m_sFileMapName);
                LoadUcsFilesParallel(module, pItr);
            }
            catch (CRainmanException *pE)
            {
                delete pItr;
                return new CRainmanException(pE, __FILE__, __LINE__, "Error loading UCS from \'%s\'", sUcsPath.c_str());
            }
            delete pItr;
        }

        auto tPhaseEnd = std::chrono::steady_clock::now();
        RAINMAN_LOG_INFO("Load phase: UCS files = {} ms",
                         std::chrono::duration_cast<std::chrono::milliseconds>(tPhaseEnd - tPhaseStart).count());
        return nullptr;
    };

    // --- Folders phase lambda ---
    auto foldersPhase = [&]() -> CRainmanException *
    {
        if (!bRunFolders)
            return nullptr;
        auto tPhaseStart = std::chrono::steady_clock::now();

        try
        {
            if (module.m_eModuleType == CModuleFile::MT_DawnOfWar)
            {
                char *sOutFolder = nullptr;
                for (auto *pFolder : module.m_vFolders)
                {
                    if (stricmp(pFolder->m_sName, "data") == 0)
                    {
                        sOutFolder = pFolder->m_sName;
                        break;
                    }
                }
                if (!sOutFolder)
                {
                    for (auto *pFolder : module.m_vFolders)
                    {
                        if (strchr(pFolder->m_sName, '%') == nullptr)
                        {
                            sOutFolder = pFolder->m_sName;
                            break;
                        }
                    }
                }

                std::vector<FolderTask> folderTasks;
                for (auto *pFolder : module.m_vFolders)
                {
                    char *sWithoutDynamics = module._DawnOfWarRemoveDynamics(pFolder->m_sName);
                    FolderTask task;
                    task.folderPath = std::string(module.m_sApplicationPath) + module.m_metadata.m_sModFolder + "\\" +
                                      sWithoutDynamics;
                    task.uiName = sWithoutDynamics;
                    task.tocName = "Data";
                    task.iNum = static_cast<unsigned short>(15000 + pFolder->m_iNumber);
                    task.bIsDefaultWrite = (sOutFolder == pFolder->m_sName);
                    folderTasks.push_back(std::move(task));
                    delete[] sWithoutDynamics;
                }
                LoadFoldersParallel(folderTasks, module, THE_CALLBACK);
            }
            else if (module.m_eModuleType == CModuleFile::MT_CompanyOfHeroesEarly)
            {
                std::string sBasePath = std::string(module.m_sApplicationPath) + module.m_metadata.m_sModFolder;
                std::vector<FolderTask> folderTasks;
                folderTasks.push_back({sBasePath + "\\Data", "", "Data", 15000, true});
                folderTasks.push_back({sBasePath + "\\Movies", "", "Movies", 15000, true});
                LoadFoldersParallel(folderTasks, module, THE_CALLBACK);
            }
            else if (module.m_eModuleType == CModuleFile::MT_CompanyOfHeroes)
            {
                std::vector<FolderTask> folderTasks;
                for (auto *pDS : module.m_vDataSources)
                {
                    // m_bIsLoaded already pre-computed above
                    if (pDS->m_bIsLoaded && pDS->m_sFolder && *pDS->m_sFolder)
                    {
                        FolderTask task;
                        task.folderPath = std::string(module.m_sApplicationPath) + pDS->m_sFolder;
                        task.uiName = pDS->m_sFolder;
                        task.tocName = pDS->m_sToc;
                        task.iNum = static_cast<unsigned short>(15000 + pDS->m_iNumber);
                        task.bIsDefaultWrite = ((strcmp(pDS->m_sOption, "common") == 0) && (pDS->m_iNumber < 2));
                        task.pDataSource = pDS;
                        folderTasks.push_back(std::move(task));
                    }
                }
                LoadFoldersParallel(folderTasks, module, THE_CALLBACK);
            }
        }
        catch (CRainmanException *pE)
        {
            auto tPhaseEnd = std::chrono::steady_clock::now();
            RAINMAN_LOG_INFO("Load phase: Data folders = {} ms (FAILED)",
                             std::chrono::duration_cast<std::chrono::milliseconds>(tPhaseEnd - tPhaseStart).count());
            return pE;
        }

        auto tPhaseEnd = std::chrono::steady_clock::now();
        RAINMAN_LOG_INFO("Load phase: Data folders = {} ms",
                         std::chrono::duration_cast<std::chrono::milliseconds>(tPhaseEnd - tPhaseStart).count());
        return nullptr;
    };

    // --- Archives phase lambda ---
    auto archivesPhase = [&]() -> CRainmanException *
    {
        if (!bRunArchives)
            return nullptr;
        auto tPhaseStart = std::chrono::steady_clock::now();

        try
        {
            if (module.m_eModuleType == CModuleFile::MT_DawnOfWar)
            {
                std::vector<ArchiveTask> tasks;
                for (auto *pArch : module.m_vArchives)
                {
                    char *sWithoutDynamics = module._DawnOfWarRemoveDynamics(pArch->m_sName);
                    ArchiveTask task;
                    task.archivePath = std::string(module.m_sApplicationPath) + module.m_metadata.m_sModFolder + "\\" +
                                       sWithoutDynamics;
                    task.uiName = pArch->m_sName;
                    task.iNum = static_cast<unsigned short>(15000 + pArch->m_iNumber);
                    task.ppSgaOut = &pArch->m_pHandle;
                    tasks.push_back(std::move(task));
                    delete[] sWithoutDynamics;
                }
                CallCallback(THE_CALLBACK, "Loading data archives for mod \'%s\'", module.m_sFileMapName);
                LoadArchivesParallel(tasks, module, THE_CALLBACK);
            }
            else if (module.m_eModuleType == CModuleFile::MT_CompanyOfHeroesEarly)
            {
                char *sArchivesPath =
                    new char[strlen(module.m_sApplicationPath) + strlen(module.m_metadata.m_sModFolder) + 10];
                sprintf(sArchivesPath, "%s%s\\Archives", module.m_sApplicationPath, module.m_metadata.m_sModFolder);
                IDirectoryTraverser::IIterator *pItr = nullptr;
                try
                {
                    pItr = module.m_pFSS->VIterate(sArchivesPath);
                    CallCallback(THE_CALLBACK, "Loading data archives for mod \'%s\'", module.m_sFileMapName);
                    Util_ForEach(pItr, CModuleFile_ArchiveForEach, static_cast<void *>(&module), false);
                }
                catch (CRainmanException *pE)
                {
                    auto *pWrapped = new CRainmanException(pE, __FILE__, __LINE__, "Error loading archives from \'%s\'",
                                                           sArchivesPath);
                    delete[] sArchivesPath;
                    delete pItr;
                    throw pWrapped;
                }
                delete pItr;
                delete[] sArchivesPath;
            }
            else if (module.m_eModuleType == CModuleFile::MT_CompanyOfHeroes)
            {
                std::vector<ArchiveTask> tasks;
                for (auto *pDS : module.m_vDataSources)
                {
                    // m_bIsLoaded already pre-computed above
                    if (pDS->m_bIsLoaded)
                    {
                        for (auto *pArch : pDS->m_vArchives)
                        {
                            ArchiveTask task;
                            task.archivePath = std::string(module.m_sApplicationPath) + pArch->m_sName;
                            task.uiName = pArch->m_sName;
                            task.iNum = static_cast<unsigned short>(15000 + (500 * pDS->m_iNumber) + pArch->m_iNumber);
                            task.ppSgaOut = &pArch->m_pHandle;
                            tasks.push_back(std::move(task));
                        }
                    }
                }
                CallCallback(THE_CALLBACK, "Loading data archives for mod \'%s\'", module.m_sFileMapName);
                LoadArchivesParallel(tasks, module, THE_CALLBACK);
            }
        }
        catch (CRainmanException *pE)
        {
            auto tPhaseEnd = std::chrono::steady_clock::now();
            RAINMAN_LOG_INFO("Load phase: Data archives = {} ms (FAILED)",
                             std::chrono::duration_cast<std::chrono::milliseconds>(tPhaseEnd - tPhaseStart).count());
            return pE;
        }

        auto tPhaseEnd = std::chrono::steady_clock::now();
        RAINMAN_LOG_INFO("Load phase: Data archives = {} ms",
                         std::chrono::duration_cast<std::chrono::milliseconds>(tPhaseEnd - tPhaseStart).count());
        return nullptr;
    };

    // Execute phases — run concurrently if 2+ are active
    {
        int parallelCount = (bRunUcs ? 1 : 0) + (bRunFolders ? 1 : 0) + (bRunArchives ? 1 : 0);
        CRainmanException *pUcsError = nullptr;
        CRainmanException *pFoldersError = nullptr;
        CRainmanException *pArchivesError = nullptr;

        if (parallelCount >= 2)
        {
            RAINMAN_LOG_INFO("Cross-phase parallelism: launching {} phases concurrently", parallelCount);
            auto tParStart = std::chrono::steady_clock::now();

            auto &crossPool = CThreadPool::Instance();
            std::future<CRainmanException *> ucsFut, foldersFut, archivesFut;
            if (bRunUcs)
                ucsFut = crossPool.Submit(ucsPhase);
            if (bRunFolders)
                foldersFut = crossPool.Submit(foldersPhase);
            if (bRunArchives)
                archivesFut = crossPool.Submit(archivesPhase);

            if (bRunUcs)
                pUcsError = ucsFut.get();
            if (bRunFolders)
                pFoldersError = foldersFut.get();
            if (bRunArchives)
                pArchivesError = archivesFut.get();

            auto tParEnd = std::chrono::steady_clock::now();
            RAINMAN_LOG_INFO("Cross-phase parallel: all {} phases completed in {} ms", parallelCount,
                             std::chrono::duration_cast<std::chrono::milliseconds>(tParEnd - tParStart).count());
        }
        else
        {
            pUcsError = ucsPhase();
            pFoldersError = foldersPhase();
            pArchivesError = archivesPhase();
        }

        // Propagate first error, destroy the rest
        CRainmanException *pErrors[] = {pUcsError, pFoldersError, pArchivesError};
        CRainmanException *pFirstError = nullptr;
        for (auto *pErr : pErrors)
        {
            if (pErr != nullptr)
            {
                if (pFirstError == nullptr)
                    pFirstError = pErr;
                else
                    pErr->destroy();
            }
        }
        if (pFirstError != nullptr)
            throw pFirstError;
    }
    if (iReloadWhat & CModuleFile::RR_MapArchives)
    {
        if (module.m_sScenarioPackRootFolder && *module.m_sScenarioPackRootFolder &&
            module.m_metadata.m_sScenarioPackFolder && *module.m_metadata.m_sScenarioPackFolder)
        {
            auto *sArchivesPath = new wchar_t[wcslen(module.m_sScenarioPackRootFolder) +
                                              strlen(module.m_metadata.m_sScenarioPackFolder) + 2];
            swprintf(sArchivesPath, L"%s\\%S", module.m_sScenarioPackRootFolder,
                     module.m_metadata.m_sScenarioPackFolder);
            IDirectoryTraverser::IIterator *pItr = nullptr;
            try
            {
                pItr = module.m_pFSS->IterateW(sArchivesPath);
            }
            catch (CRainmanException *pE)
            {
                auto guard = std::unique_ptr<CRainmanException, ExceptionDeleter>(pE);
                pItr = nullptr;
            }
            if (pItr)
            {
                try
                {
                    CallCallback(THE_CALLBACK, "Loading map archives for mod \'%s\'", module.m_sFileMapName);
                    Util_ForEach(pItr, CModuleFile_ArchiveForEachNoErrors, static_cast<void *>(&module), false);
                }
                catch (CRainmanException *pE)
                {
                    PAUSE_THROW(pE, __FILE__, __LINE__, "Error loading map archives from \'%S\'", sArchivesPath);
                    delete pItr;
                    delete[] sArchivesPath;
                    UNPAUSE_THROW;
                }
                delete pItr;
            }
            delete[] sArchivesPath;
        }
    }
    if (iReloadWhat & CModuleFile::RR_RequiredMods)
    {
        auto tPhaseStart = std::chrono::steady_clock::now();
        if (module.m_eModuleType == CModuleFile::MT_DawnOfWar)
        {
            for (auto *pReqHandler : module.m_vRequireds)
            {
                CModuleFile *pReq = CHECK_MEM(new CModuleFile);
                free(pReq->m_sLocale);
                pReq->m_sLocale = module.m_sLocale;

                char *sFilePath = new char[strlen(module.m_sApplicationPath) + strlen(pReqHandler->m_sName) + 8];
                sprintf(sFilePath, "%s%s.module", module.m_sApplicationPath, pReqHandler->m_sName);
                pReq->LoadModuleFile(sFilePath);
                delete[] sFilePath;

                pReq->m_pParentModule = &module;
                pReq->m_pNewFileMap = module.m_pNewFileMap;
                pReq->m_pFSS = module.m_pFSS;
                free(pReq->m_sApplicationPath);
                pReq->m_sApplicationPath = module.m_sApplicationPath;
                pReq->m_iFileMapModNumber = static_cast<unsigned short>(15000 + pReqHandler->m_iNumber);

                pReqHandler->m_pHandle = pReq;

                pReq->ReloadResources(iReloadWhatRequiredMods & (~CModuleFile::RR_Engines) &
                                          (~CModuleFile::RR_RequiredMods),
                                      0, 0, THE_CALLBACK);
            }
        }
        auto tPhaseEnd = std::chrono::steady_clock::now();
        RAINMAN_LOG_INFO("Load phase: Required mods = {} ms",
                         std::chrono::duration_cast<std::chrono::milliseconds>(tPhaseEnd - tPhaseStart).count());
    }
    if (iReloadWhat & CModuleFile::RR_Engines)
    {
        auto tPhaseStart = std::chrono::steady_clock::now();
        if (module.m_eModuleType == CModuleFile::MT_DawnOfWar)
        {
            CModuleFile *pEngine = CHECK_MEM(new CModuleFile);

            pEngine->m_eModuleType = CModuleFile::MT_DawnOfWar;
            pEngine->m_metadata.m_sUiName = strdup("(Engine)");
            pEngine->m_sFileMapName = strdup("(Engine)");
            pEngine->m_metadata.m_sDescription = strdup("(Engine)");
            pEngine->m_metadata.m_sModFolder = strdup("Engine");

            free(pEngine->m_sLocale);
            pEngine->m_sLocale = module.m_sLocale;
            pEngine->m_pParentModule = &module;
            pEngine->m_pNewFileMap = module.m_pNewFileMap;
            pEngine->m_pFSS = module.m_pFSS;
            pEngine->m_sApplicationPath = module.m_sApplicationPath;
            pEngine->m_iFileMapModNumber = static_cast<unsigned short>(30001);

            {
                auto *pObject = CHECK_MEM(new CModuleFile::CFolderHandler);
                pEngine->m_vFolders.push_back(pObject);
                pObject->m_sName = CHECK_MEM(strdup("Data"));
                pObject->m_iNumber = 1;
            }
            {
                auto *pObject = CHECK_MEM(new CModuleFile::CArchiveHandler);
                pEngine->m_vArchives.push_back(pObject);
                pObject->m_sName = CHECK_MEM(strdup("Engine.sga"));
                pObject->m_iNumber = 1;
            }

            module.m_vEngines.push_back(pEngine);

            pEngine->ReloadResources(iReloadWhatEngines & (~CModuleFile::RR_Engines), 0, 0, THE_CALLBACK);
        }
        else if (module.m_eModuleType == CModuleFile::MT_CompanyOfHeroesEarly)
        {
            LoadCohEngine(module, "Engine", "(Engine)", iReloadWhatEngines, 30001, THE_CALLBACK);
            LoadCohEngine(module, "RelicOnline", "(Relic Online)", iReloadWhatEngines, 30000, THE_CALLBACK);
        }
        // NOLINTNEXTLINE(bugprone-branch-clone) distinct module type, same engine loading
        else if (module.m_eModuleType == CModuleFile::MT_CompanyOfHeroes)
        {
            LoadCohEngine(module, "Engine", "(Engine)", iReloadWhatEngines, 30001, THE_CALLBACK);
            LoadCohEngine(module, "RelicOnline", "(Relic Online)", iReloadWhatEngines, 30000, THE_CALLBACK);
        }
        auto tPhaseEnd = std::chrono::steady_clock::now();
        RAINMAN_LOG_INFO("Load phase: Engines = {} ms",
                         std::chrono::duration_cast<std::chrono::milliseconds>(tPhaseEnd - tPhaseStart).count());
    }
    if (iReloadWhat && CModuleFile::RR_DataGeneric)
    {
        LoadDataGeneric(module, THE_CALLBACK);
    }
    auto tLoadEnd = std::chrono::steady_clock::now();
    RAINMAN_LOG_INFO("Load TOTAL for '{}' = {} ms", module.m_sFileMapName ? module.m_sFileMapName : "(unknown)",
                     std::chrono::duration_cast<std::chrono::milliseconds>(tLoadEnd - tLoadStart).count());
}

void CResourceLoader::DoLoadFolder(CModuleFile &module, const char *sFullPath, bool bIsDefaultWrite,
                                   unsigned short iNum, const char *sTOC, const char *sUiName, CALLBACK_ARG,
                                   bool *bIsWritable)
{
    if (bIsWritable != nullptr)
        *bIsWritable = false;
    const char *sSlashChar;
    if (sUiName)
    {
        sSlashChar = sUiName;
    }
    else
    {
        sSlashChar = strrchr(sFullPath, '\\');
        if (!sSlashChar)
            sSlashChar = strrchr(sFullPath, '/');
        ++sSlashChar;
    }

    bool bIsThisMod = true;
    char *sActualModName = nullptr;
    if (module.m_eModuleType == CModuleFile::MT_CompanyOfHeroes)
    {
        if (strnicmp(module.m_sCohThisModFolder, sFullPath, strlen(module.m_sCohThisModFolder)) != 0)
        {
            bIsThisMod = false;
            sActualModName = strdup(sFullPath + strlen(module.m_sApplicationPath));
            char *sSlash = strrchr(sActualModName, '\\');
            if (sSlash)
                *sSlash = 0;
        }
    }

    IDirectoryTraverser::IIterator *pDirItr = nullptr;
    try
    {
        CallCallback(THE_CALLBACK, "Loading data folder \'%s\' for mod \'%s\'", sSlashChar, module.m_sFileMapName);
        pDirItr = module.m_pFSS->VIterate(sFullPath);
        if (pDirItr)
        {
            void *pSrc;
            if (bIsThisMod)
            {
                pSrc = module.m_pNewFileMap->RegisterSource(
                    module.m_iFileMapModNumber, false, iNum, module.GetFileMapName(), sSlashChar, module.m_pFSS,
                    module.m_pFSS, module.m_pParentModule ? false : true, bIsDefaultWrite);
                if (bIsWritable != nullptr)
                    *bIsWritable = module.m_pParentModule ? false : true;
            }
            else
                pSrc = module.m_pNewFileMap->RegisterSource(15000, false, iNum, sActualModName, sSlashChar,
                                                            module.m_pFSS, module.m_pFSS, false, false);

            module.m_pNewFileMap->MapIterator(pSrc, sTOC, pDirItr);
        }
    }
    IGNORE_EXCEPTIONS
    if (pDirItr)
        delete pDirItr;
    if (sActualModName)
        free(sActualModName); // NOLINT(clang-analyzer-unix.MismatchedDeallocator) -- strdup uses malloc; free() is
                              // correct
}

void CResourceLoader::DoLoadArchive(CModuleFile &module, const char *sFullPath, CSgaFile **ppSga, unsigned short iNum,
                                    const char *sUiName, CALLBACK_ARG)
{
    const char *sSlashChar;
    if (sUiName)
    {
        sSlashChar = sUiName;
    }
    else
    {
        sSlashChar = strrchr(sFullPath, '\\');
        if (!sSlashChar)
            sSlashChar = strrchr(sFullPath, '/');
        ++sSlashChar;
    }

    CSgaFile *pSga = CHECK_MEM(new CSgaFile);
    std::unique_ptr<IFileStore::IStream> inputStream;
    IDirectoryTraverser::IIterator *pDirItr = nullptr;
    try
    {
        inputStream = std::unique_ptr<IFileStore::IStream>(module.m_pFSS->VOpenStream(sFullPath));
    }
    catch (CRainmanException *pE)
    {
        auto guard = std::unique_ptr<CRainmanException, ExceptionDeleter>(pE);
        delete pSga;
        pSga = nullptr;
        return;
    }

    bool bIsThisMod = true;
    char *sActualModName = nullptr;
    if (module.m_eModuleType == CModuleFile::MT_CompanyOfHeroes)
    {
        if (strnicmp(module.m_sCohThisModFolder, sFullPath, strlen(module.m_sCohThisModFolder)) != 0)
        {
            if (module.m_saScenarioPackRootFolder &&
                strnicmp(module.m_saScenarioPackRootFolder, sFullPath, strlen(module.m_saScenarioPackRootFolder)) == 0)
            {
                bIsThisMod = false;
                sActualModName = strdup(sFullPath + strlen(module.m_saScenarioPackRootFolder) + 1);
                char *sSlash = strrchr(sActualModName, '\\');
                if (sSlash)
                    *sSlash = 0;
            }
            else
            {
                bIsThisMod = false;
                sActualModName = strdup(sFullPath + strlen(module.m_sApplicationPath));
                char *sSlash = strrchr(sActualModName, '\\');
                if (sSlash)
                    *sSlash = 0;
                sSlash = strrchr(sActualModName, '\\');
                if (sSlash)
                    *sSlash = 0;
            }
        }
    }

    try
    {
        CallCallback(THE_CALLBACK, "Loading data archive \'%s\' for mod \'%s\'", sSlashChar, module.m_sFileMapName);
        pSga->Load(inputStream.get(), module.m_pFSS->VGetLastWriteTime(sFullPath));
        pSga->VInit(inputStream.get());
        pDirItr = pSga->VIterate(pSga->VGetEntryPoint(0));
        if (pDirItr)
        {
            void *pSrc;
            if (bIsThisMod)
                pSrc =
                    module.m_pNewFileMap->RegisterSource(module.m_iFileMapModNumber, true, iNum,
                                                         module.GetFileMapName(), sSlashChar, pSga, pSga, false, false);
            else
                pSrc = module.m_pNewFileMap->RegisterSource(15000, true, iNum, sActualModName, sSlashChar, pSga, pSga,
                                                            false, false);
            module.m_pNewFileMap->MapSga(pSrc, pSga);
        }
        delete pDirItr;
        (void)inputStream.release();
    }
    catch (CRainmanException *pE)
    {
        pE = new CRainmanException(pE, __FILE__, __LINE__, "(rethrow for \'%s\')", sFullPath);
        delete pDirItr;
        delete pSga;
        if (sActualModName)
            free(sActualModName); // NOLINT(clang-analyzer-unix.MismatchedDeallocator) -- strdup uses malloc
        throw pE;
    }
    *ppSga = pSga;
    if (sActualModName)
        free(sActualModName); // NOLINT(clang-analyzer-unix.MismatchedDeallocator) -- strdup uses malloc
}
