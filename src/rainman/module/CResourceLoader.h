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

#ifndef _C_RESOURCE_LOADER_H_
#define _C_RESOURCE_LOADER_H_

#include "rainman/core/Callbacks.h"
#include "rainman/module/CModuleFile.h"

class CSgaFile;

//! Orchestrates loading of resources (UCS files, folders, archives, engines)
//! into a CModuleFile's CFileMap. Extracted from CModuleFile to reduce its size.
class CResourceLoader
{
  public:
    static void Load(CModuleFile &module, unsigned long iReloadWhat, unsigned long iReloadWhatRequiredMods,
                     unsigned long iReloadWhatEngines, CALLBACK_ARG);

    // Exposed for use by the free-function ForEach callbacks declared as friends of CModuleFile
    static void DoLoadFolder(CModuleFile &module, const char *sFullPath, bool bIsDefaultWrite, unsigned short iNum,
                             const char *sTOC, const char *sUiName = nullptr, CALLBACK_ARG_OPT,
                             bool *bIsWritable = nullptr);
    static void DoLoadArchive(CModuleFile &module, const char *sFullPath, CSgaFile **ppSga, unsigned short iNum,
                              const char *sUiName, CALLBACK_ARG_OPT);

  private:
    static bool IsToBeLoaded(const CModuleFile &module, CModuleFile::CCohDataSource *pDataSource);
    static void LoadDataGeneric(CModuleFile &module, CALLBACK_ARG);
    static void LoadCohEngine(CModuleFile &module, const char *sFolder, const char *sUiName, unsigned long iLoadWhat,
                              unsigned short iNum, CALLBACK_ARG);
};

#endif
