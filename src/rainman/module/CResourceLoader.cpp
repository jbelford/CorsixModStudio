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
#include "rainman/core/Internal_Util.h"
#include "rainman/core/Exception.h"
#include <memory>

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

    bool bInSection;
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
    module._CleanResources();

    if (module.m_pFSS == nullptr)
        module.m_pFSS = new CFileSystemStore;
    if (module.m_pNewFileMap == nullptr)
        module.m_pNewFileMap = new CFileMap;

    if (iReloadWhat & CModuleFile::RR_LocalisedText)
    {
        char *sUcsPath;
        if (module.m_metadata.m_sLocFolder && *module.m_metadata.m_sLocFolder)
        {
            sUcsPath = new char[strlen(module.m_sApplicationPath) + strlen(module.m_metadata.m_sLocFolder) +
                                strlen(module.m_sLocale) + 2];
            sprintf(sUcsPath, "%s%s\\%s", module.m_sApplicationPath, module.m_metadata.m_sLocFolder, module.m_sLocale);
        }
        else
        {
            sUcsPath = new char[strlen(module.m_sApplicationPath) + strlen(module.m_metadata.m_sModFolder) +
                                strlen(module.m_sLocale) + 10];
            sprintf(sUcsPath, "%s%s\\Locale\\%s", module.m_sApplicationPath, module.m_metadata.m_sModFolder,
                    module.m_sLocale);
        }

        IDirectoryTraverser::IIterator *pItr = nullptr;
        try
        {
            pItr = module.m_pFSS->VIterate(sUcsPath);
        }
        IGNORE_EXCEPTIONS
        if (pItr != nullptr)
        {
            try
            {
                CallCallback(THE_CALLBACK, "Loading UCS files for mod \'%s\'", module.m_sFileMapName);
                Util_ForEach(pItr, CModuleFile_UcsForEach, static_cast<void *>(&module), false);
            }
            catch (CRainmanException *pE)
            {
                PAUSE_THROW(pE, __FILE__, __LINE__, "Error loading UCS from \'%s\'", sUcsPath);
                delete[] sUcsPath;
                delete pItr;
                UNPAUSE_THROW;
            }
            delete pItr;
        }
        delete[] sUcsPath;
    }
    if (iReloadWhat & CModuleFile::RR_DataFolders)
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

            for (auto *pFolder : module.m_vFolders)
            {
                char *sWithoutDynamics = module._DawnOfWarRemoveDynamics(pFolder->m_sName);
                char *sFolderPath = new char[strlen(module.m_sApplicationPath) +
                                             strlen(module.m_metadata.m_sModFolder) + strlen(sWithoutDynamics) + 2];
                sprintf(sFolderPath, "%s%s\\%s", module.m_sApplicationPath, module.m_metadata.m_sModFolder,
                        sWithoutDynamics);
                DoLoadFolder(module, sFolderPath, (sOutFolder == pFolder->m_sName), 15000 + pFolder->m_iNumber, "Data",
                             sWithoutDynamics, THE_CALLBACK);
                delete[] sFolderPath;
                delete[] sWithoutDynamics;
            }
        }
        else if (module.m_eModuleType == CModuleFile::MT_CompanyOfHeroesEarly)
        {
            char *sFolderPath =
                new char[strlen(module.m_sApplicationPath) + strlen(module.m_metadata.m_sModFolder) + 10];
            sprintf(sFolderPath, "%s%s\\Data", module.m_sApplicationPath, module.m_metadata.m_sModFolder);
            DoLoadFolder(module, sFolderPath, true, 15000, "Data", nullptr, THE_CALLBACK);
            sprintf(sFolderPath, "%s%s\\Movies", module.m_sApplicationPath, module.m_metadata.m_sModFolder);
            DoLoadFolder(module, sFolderPath, true, 15000, "Movies", nullptr, THE_CALLBACK);
            delete[] sFolderPath;
        }
        else if (module.m_eModuleType == CModuleFile::MT_CompanyOfHeroes)
        {
            for (auto *pDS : module.m_vDataSources)
            {
                pDS->m_bIsLoaded = IsToBeLoaded(module, pDS);
                pDS->m_bCanWriteToFolder = false;
                if (IsToBeLoaded(module, pDS) && pDS->m_sFolder && *pDS->m_sFolder)
                {
                    char *sFolderPath = new char[strlen(module.m_sApplicationPath) + strlen(pDS->m_sFolder) + 1];
                    sprintf(sFolderPath, "%s%s", module.m_sApplicationPath, pDS->m_sFolder);
                    bool bIsWrite = ((strcmp(pDS->m_sOption, "common") == 0) && (pDS->m_iNumber < 2));
                    DoLoadFolder(module, sFolderPath, bIsWrite, 15000 + pDS->m_iNumber, pDS->m_sToc, pDS->m_sFolder,
                                 THE_CALLBACK, &pDS->m_bCanWriteToFolder);
                    delete[] sFolderPath;
                }
            }
        }
    }
    if (iReloadWhat & CModuleFile::RR_DataArchives)
    {
        if (module.m_eModuleType == CModuleFile::MT_DawnOfWar)
        {
            for (auto *pArch : module.m_vArchives)
            {
                char *sWithoutDynamics = module._DawnOfWarRemoveDynamics(pArch->m_sName);
                char *sArchivePath = new char[strlen(module.m_sApplicationPath) +
                                              strlen(module.m_metadata.m_sModFolder) + strlen(sWithoutDynamics) + 2];
                sprintf(sArchivePath, "%s%s\\%s", module.m_sApplicationPath, module.m_metadata.m_sModFolder,
                        sWithoutDynamics);
                DoLoadArchive(module, sArchivePath, &pArch->m_pHandle, 15000 + pArch->m_iNumber, pArch->m_sName,
                              THE_CALLBACK);
                delete[] sArchivePath;
                delete[] sWithoutDynamics;
            }
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
                PAUSE_THROW(pE, __FILE__, __LINE__, "Error loading archives from \'%s\'", sArchivesPath);
                delete[] sArchivesPath;
                delete pItr;
                UNPAUSE_THROW;
            }
            delete pItr;
            delete[] sArchivesPath;
        }
        else if (module.m_eModuleType == CModuleFile::MT_CompanyOfHeroes)
        {
            for (auto *pDS : module.m_vDataSources)
            {
                pDS->m_bIsLoaded = IsToBeLoaded(module, pDS);
                if (IsToBeLoaded(module, pDS))
                {
                    for (auto *pArch : pDS->m_vArchives)
                    {
                        char *sArchivePath = new char[strlen(module.m_sApplicationPath) + strlen(pArch->m_sName) + 1];
                        sprintf(sArchivePath, "%s%s", module.m_sApplicationPath, pArch->m_sName);
                        DoLoadArchive(module, sArchivePath, &pArch->m_pHandle,
                                      15000 + (500 * pDS->m_iNumber) + pArch->m_iNumber, pArch->m_sName, THE_CALLBACK);
                        delete[] sArchivePath;
                    }
                }
            }
        }
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
    }
    if (iReloadWhat & CModuleFile::RR_Engines)
    {
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
    }
    if (iReloadWhat && CModuleFile::RR_DataGeneric)
    {
        LoadDataGeneric(module, THE_CALLBACK);
    }
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
        free(sActualModName);
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
            free(sActualModName);
        throw pE;
    }
    *ppSga = pSga;
    if (sActualModName)
        free(sActualModName);
}
