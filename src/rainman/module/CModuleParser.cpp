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

#include "rainman/module/CModuleParser.h"
#include "rainman/core/gnuc_defines.h"
#include "rainman/core/Internal_Util.h"
#include "rainman/core/Exception.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

CModuleParseResult CModuleParser::Parse(const char *sFileName)
{
    CModuleParseResult result;

    FILE *fModule = fopen(sFileName, "rb");
    if (fModule == nullptr)
        throw CRainmanException(nullptr, __FILE__, __LINE__, "Unable to open \'%s\' for reading", sFileName);

    size_t iCurrentDS = SIZE_MAX;
    bool bInGlobal = false;
    while (!feof(fModule))
    {
        char *sLine = Util_fgetline(fModule);
        if (sLine == nullptr)
        {
            fclose(fModule);
            throw CRainmanException(__FILE__, __LINE__, "Error reading from file");
        }
        char *sCommentBegin = strchr(sLine, ';');
        if (sCommentBegin)
            *sCommentBegin = 0;
        char *sEqualsChar = strchr(sLine, '=');
        if (sEqualsChar)
        {
            // Key = Value
            char *sKey = sLine;
            char *sValue = sEqualsChar + 1;
            *sEqualsChar = 0;
            Util_TrimWhitespace(&sKey);
            Util_TrimWhitespace(&sValue);
            if (bInGlobal)
            {
                try
                {
                    if (stricmp(sKey, "UIName") == 0)
                    {
                        if (result.metadata.m_sUiName)
                            free(result.metadata.m_sUiName);
                        result.metadata.m_sUiName = CHECK_MEM(strdup(sValue));
                    }
                    else if (stricmp(sKey, "Name") == 0)
                    {
                        if (result.metadata.m_sName)
                            free(result.metadata.m_sName);
                        result.metadata.m_sName = CHECK_MEM(strdup(sValue));
                    }
                    else if (stricmp(sKey, "Description") == 0)
                    {
                        if (result.metadata.m_sDescription)
                            free(result.metadata.m_sDescription);
                        result.metadata.m_sDescription = CHECK_MEM(strdup(sValue));
                    }
                    else if (stricmp(sKey, "DllName") == 0)
                    {
                        if (result.metadata.m_sDllName)
                            free(result.metadata.m_sDllName);
                        result.metadata.m_sDllName = CHECK_MEM(strdup(sValue));
                    }
                    else if (stricmp(sKey, "ModFolder") == 0)
                    {
                        if (result.metadata.m_sModFolder)
                            free(result.metadata.m_sModFolder);
                        result.metadata.m_sModFolder = CHECK_MEM(strdup(sValue));
                    }
                    else if (stricmp(sKey, "TextureFE") == 0)
                    {
                        if (result.metadata.m_sTextureFe)
                            free(result.metadata.m_sTextureFe);
                        result.metadata.m_sTextureFe = CHECK_MEM(strdup(sValue));
                    }
                    else if (stricmp(sKey, "TextureIcon") == 0)
                    {
                        if (result.metadata.m_sTextureIcon)
                            free(result.metadata.m_sTextureIcon);
                        result.metadata.m_sTextureIcon = CHECK_MEM(strdup(sValue));
                    }
                    else if (stricmp(sKey, "PatcherUrl") == 0)
                    {
                        if (result.metadata.m_sPatcherUrl)
                            free(result.metadata.m_sPatcherUrl);
                        result.metadata.m_sPatcherUrl = CHECK_MEM(strdup(sValue));
                    }
                    else if (stricmp(sKey, "LocFolder") == 0)
                    {
                        if (result.metadata.m_sLocFolder)
                            free(result.metadata.m_sLocFolder);
                        result.metadata.m_sLocFolder = CHECK_MEM(strdup(sValue));
                    }
                    else if (stricmp(sKey, "ScenarioPackFolder") == 0)
                    {
                        if (result.metadata.m_sScenarioPackFolder)
                            free(result.metadata.m_sScenarioPackFolder);
                        result.metadata.m_sScenarioPackFolder = CHECK_MEM(strdup(sValue));
                    }
                    else if (stricmp(sKey, "ModVersion") == 0)
                    {
                        char *sDotChar = strchr(sValue, '.');
                        if (sDotChar)
                            *sDotChar = 0;
                        result.metadata.m_iModVersionMajor = atol(sValue);
                        if (sDotChar)
                        {
                            sValue = sDotChar + 1;
                            sDotChar = strchr(sValue, '.');
                            if (sDotChar)
                                *sDotChar = 0;
                            result.metadata.m_iModVersionMinor = atol(sValue);
                            if (sDotChar)
                            {
                                sValue = sDotChar + 1;
                                result.metadata.m_iModVersionRevision = atol(sValue);
                            }
                        }
                    }
                    else if (strnicmp(sKey, "DataFolder.", 11) == 0)
                    {
                        ParsedEntry entry;
                        entry.sName = sValue;
                        entry.iNumber = atol(sKey + 11);
                        result.folders.push_back(std::move(entry));
                    }
                    else if (strnicmp(sKey, "ArchiveFile.", 12) == 0)
                    {
                        ParsedEntry entry;
                        entry.sName = std::string(sValue) + ".sga";
                        entry.iNumber = atol(sKey + 12);
                        result.archives.push_back(std::move(entry));
                    }
                    else if (strnicmp(sKey, "RequiredMod.", 12) == 0)
                    {
                        ParsedEntry entry;
                        entry.sName = sValue;
                        entry.iNumber = atol(sKey + 12);
                        result.requireds.push_back(std::move(entry));
                    }
                    else if (strnicmp(sKey, "CompatableMod.", 14) == 0)
                    {
                        ParsedEntry entry;
                        entry.sName = sValue;
                        entry.iNumber = atol(sKey + 12);
                        result.compatibles.push_back(std::move(entry));
                    }
                }
                catch (const CRainmanException &e)
                {
                    delete[] sLine;
                    fclose(fModule);
                    throw e;
                }
            }
            else if (iCurrentDS != SIZE_MAX)
            {
                try
                {
                    if (stricmp(sKey, "folder") == 0)
                    {
                        result.dataSources[iCurrentDS].sFolder = sValue;
                    }
                    if (strnicmp(sKey, "archive.", 8) == 0)
                    {
                        ParsedEntry entry;
                        entry.sName = std::string(sValue) + ".sga";
                        entry.iNumber = atol(sKey + 8);
                        result.dataSources[iCurrentDS].archives.push_back(std::move(entry));
                    }
                }
                catch (const CRainmanException &e)
                {
                    delete[] sLine;
                    fclose(fModule);
                    throw e;
                }
            }
        }
        else
        {
            char *sLeftBraceChar = strchr(sLine, '[');
            char *sRightBraceChar = sLeftBraceChar ? strchr(sLeftBraceChar, ']') : nullptr;
            if (sLeftBraceChar && sRightBraceChar)
            {
                // [Section]
                *sRightBraceChar = 0;
                ++sLeftBraceChar;
                bInGlobal = false;
                iCurrentDS = SIZE_MAX;
                if (stricmp(sLeftBraceChar, "global") == 0)
                {
                    bInGlobal = true;
                }
                else
                {
                    char *sColonChar = strchr(sLeftBraceChar, ':');
                    if (sColonChar)
                    {
                        try
                        {
                            ParsedDataSource ds;
                            *sColonChar = 0;
                            ds.sToc = sLeftBraceChar;
                            sLeftBraceChar = sColonChar + 1;
                            sColonChar = strchr(sLeftBraceChar, ':');
                            if (sColonChar)
                                *sColonChar = 0;
                            ds.sOption = sLeftBraceChar;
                            if (sColonChar)
                                ds.iNumber = atol(sColonChar + 1);
                            result.dataSources.push_back(std::move(ds));
                            iCurrentDS = result.dataSources.size() - 1;
                        }
                        catch (const CRainmanException &e)
                        {
                            delete[] sLine;
                            fclose(fModule);
                            throw e;
                        }
                    }
                }
            }
        }
        delete[] sLine;
    }
    fclose(fModule);

    // Decide what kind of module file it is
    size_t iHeuristicDow = 0, iHeuristicCoh = 0, iHeuristicCohEarly = 0;
    if (result.metadata.m_sUiName && *result.metadata.m_sUiName)
        iHeuristicDow += 3;
    if (result.metadata.m_sName && *result.metadata.m_sName)
        iHeuristicCoh += 1, iHeuristicCohEarly += 1;
    if (result.metadata.m_sDescription && *result.metadata.m_sDescription)
        iHeuristicCoh += 1, iHeuristicCohEarly += 1, iHeuristicDow += 1;
    if (result.metadata.m_sDllName && *result.metadata.m_sDllName)
        iHeuristicCoh += 1, iHeuristicCohEarly += 1, iHeuristicDow += 1;
    if (result.metadata.m_sModFolder && *result.metadata.m_sModFolder)
        iHeuristicCoh += 1, iHeuristicCohEarly += 1, iHeuristicDow += 1;
    if (result.metadata.m_sTextureFe && *result.metadata.m_sTextureFe)
        iHeuristicDow += 3;
    if (result.metadata.m_sTextureIcon && *result.metadata.m_sTextureIcon)
        iHeuristicDow += 3;
    if (result.metadata.m_sPatcherUrl && *result.metadata.m_sPatcherUrl)
        iHeuristicCohEarly += 3;
    if (result.metadata.m_sLocFolder && *result.metadata.m_sLocFolder)
        iHeuristicCoh += 3;
    if (result.metadata.m_sScenarioPackFolder && *result.metadata.m_sScenarioPackFolder)
        iHeuristicCoh += 3;
    iHeuristicDow += (result.folders.size() * 3);
    iHeuristicDow += (result.archives.size() * 3);
    iHeuristicDow += (result.requireds.size() * 3);
    iHeuristicDow += (result.compatibles.size() * 3);
    iHeuristicCoh += (result.dataSources.size() * 3);
    if (result.dataSources.empty())
        iHeuristicDow += 5, iHeuristicCohEarly += 5;
    if (result.folders.empty() && result.archives.empty() && result.requireds.empty())
        iHeuristicCoh += 5, iHeuristicCohEarly += 5;

    if (iHeuristicDow == 0 && iHeuristicCoh == 0 && iHeuristicCohEarly == 0)
        throw CRainmanException(__FILE__, __LINE__, "File is not a valid .module file");

    if (iHeuristicDow > iHeuristicCoh && iHeuristicDow > iHeuristicCohEarly)
        result.iModuleType = 0; // MT_DawnOfWar
    else if (iHeuristicCoh > iHeuristicDow && iHeuristicCoh > iHeuristicCohEarly)
        result.iModuleType = 2; // MT_CompanyOfHeroes
    else if (iHeuristicCohEarly > iHeuristicDow && iHeuristicCohEarly > iHeuristicCoh)
        result.iModuleType = 1; // MT_CompanyOfHeroesEarly
    else
        throw CRainmanException(__FILE__, __LINE__, "Unable to determine what kind of .module file this is");

    return result;
}
