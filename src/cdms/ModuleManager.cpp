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

#include "ModuleManager.h"
#include "strconv.h"
#include "Utility.h"

ModuleManager::ModuleManager() : m_pModule(nullptr), m_pRgdHashTable(nullptr), m_sRgdHashCustomOut(nullptr) {}

ModuleManager::~ModuleManager()
{
    delete m_pModule;
    m_hashService.Shutdown();
    m_pRgdHashTable = nullptr;
    if (m_sRgdHashCustomOut)
        free(m_sRgdHashCustomOut);
}

void ModuleManager::SetModule(CModuleFile *pMod, const wxString &sModuleFile)
{
    if (m_pModule)
        delete m_pModule;
    m_pModule = pMod;
    m_sModuleFile = sModuleFile;
    m_moduleService.SetModule(pMod);
    if (pMod)
    {
        m_fileService.SetStore(pMod);
        m_fileService.SetTraverser(pMod);
    }
    else
    {
        m_fileService.SetStore(nullptr);
        m_fileService.SetTraverser(nullptr);
    }
}

void ModuleManager::ClearModule() { SetModule(nullptr, wxT("")); }

CRgdHashTable *ModuleManager::GetRgdHashTable(const wxString &dictionariesPath)
{
    if (m_pRgdHashTable)
        return m_pRgdHashTable;

    auto result = m_hashService.Initialize(dictionariesPath);
    if (!result.ok())
    {
        throw new CModStudioException(__FILE__, __LINE__, wxStringToAscii(result.error()).get());
    }

    m_pRgdHashTable = m_hashService.GetHashTable();
    if (m_hashService.GetCustomOutPath())
        m_sRgdHashCustomOut = strdup(m_hashService.GetCustomOutPath());

    return m_pRgdHashTable;
}
