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

#ifndef _MODULE_MANAGER_H_
#define _MODULE_MANAGER_H_

#include <rainman/module/CModuleFile.h>
#include <rainman/formats/CRgdHashTable.h>
#include "services/ModuleService.h"
#include "services/FileService.h"
#include "services/HashService.h"
#include <wx/string.h>
#include <memory>

class ModuleManager
{
  public:
    ModuleManager();
    ~ModuleManager();

    // Module lifecycle
    void SetModule(CModuleFile *pMod, const wxString &sModuleFile);
    void ClearModule();
    CModuleFile *GetModule() const { return m_pModule; }
    const wxString &GetModuleFile() const { return m_sModuleFile; }
    bool HasModule() const { return m_pModule != nullptr; }

    // Service accessors
    ModuleService &GetModuleService() { return m_moduleService; }
    FileService &GetFileService() { return m_fileService; }
    HashService &GetHashService() { return m_hashService; }

    // Hash table management (may throw CRainmanException)
    CRgdHashTable *GetRgdHashTable(const wxString &dictionariesPath);

  private:
    CModuleFile *m_pModule;
    wxString m_sModuleFile;

    ModuleService m_moduleService;
    FileService m_fileService;
    HashService m_hashService;

    CRgdHashTable *m_pRgdHashTable;
    char *m_sRgdHashCustomOut;
};

#endif
