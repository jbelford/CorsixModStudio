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

#ifndef _C_LUA_BURN_ACTION_H_
#define _C_LUA_BURN_ACTION_H_

#include "frmFiles.h"
#include "Construct.h"
#include "Utility.h"
#include "strconv.h"
#include "strings.h"
#include "Common.h"
#include <rainman/formats/CRgdFile.h>

class CLuaBurnAction : public frmFiles::IHandler
{
  public:
    static bool ConvertLuaFilenameToRgd(char *sFilename)
    {
        char *sTmp = strrchr(sFilename, '.');
        strcpy(sTmp, ".rgd");

        if (strnicmp(sFilename, "generic", 7) == 0)
        {
            sFilename[0] = 'd';
            sFilename[1] = 'a';
            sFilename[2] = 't';
            sFilename[3] = 'a';
            memmove(sFilename + 4, sFilename + 7, strlen(sFilename + 6));
            return true;
        }
        return false;
    }

    virtual wxString VGetExt() const { return wxT("lua"); }

    virtual wxString VGetAction() const { return wxT("Convert LUA to RGD"); }

    virtual void VHandle(wxString sFile, wxTreeItemId &oParent, wxTreeItemId &oFile)
    {
        UNUSED(oFile);
        long iRGDVersion = 1;
        if (TheConstruct->GetModuleService().GetModuleType() != CModuleFile::MT_DawnOfWar)
            iRGDVersion = 3;

        auto streamResult = TheConstruct->GetFileService().OpenStream(sFile);
        if (!streamResult)
        {
            ErrorBox("Cannot open file");
            return;
        }
        auto &stream = streamResult.value();
        char *saFile = wxStringToAscii(sFile);

        CLuaFile *pLua = CLuaAction::DoLoad(stream.get(), saFile);
        if (pLua)
        {
            CRgdFile *pRgd = new CRgdFile;
            if (pRgd)
            {
                pRgd->SetHashTable(TheConstruct->GetHashService().GetHashTable());
                try
                {
                    pRgd->Load(pLua, iRGDVersion);
                }
                catch (CRainmanException *pE)
                {
                    ErrorBoxE(pE);
                    goto after_rgd_loaded_cleany_code;
                }
                {
                    bool bMovedTOC = ConvertLuaFilenameToRgd(saFile);
                    BackupFile(TheConstruct->GetModuleService().GetModule(), AsciiTowxString(saFile));
                    auto outResult = TheConstruct->GetFileService().OpenOutputStream(AsciiTowxString(saFile), true);
                    if (outResult)
                    {
                        auto &outStream = outResult.value();
                        auto itrResult =
                            TheConstruct->GetFileService().Iterate(AsciiTowxString(saFile).BeforeLast('\\'));
                        IDirectoryTraverser::IIterator *pDir = itrResult ? itrResult.value().release() : nullptr;
                        frmFiles *pFiles = TheConstruct->GetFilesList();
                        if (bMovedTOC)
                            pFiles->UpdateDirectoryChildren(pFiles->FindFile(AsciiTowxString(saFile), true), pDir);
                        else
                            pFiles->UpdateDirectoryChildren(oParent, pDir);
                        delete pDir;
                        try
                        {
                            pRgd->Save(outStream.get());
                            wxMessageBox(AppStr(rgd_burngood), VGetAction(), wxICON_INFORMATION, TheConstruct);
                        }
                        catch (CRainmanException *pE)
                        {
                            ErrorBoxE(pE);
                            RestoreBackupFile(TheConstruct->GetModuleService().GetModule(), AsciiTowxString(saFile));
                        }
                    }
                    else
                    {
                        ErrorBoxAS(err_write);
                    }
                }
            after_rgd_loaded_cleany_code:
                delete pRgd;
            }
            else
            {
                ErrorBoxAS(err_memory);
            }
        }
        delete[] saFile;
        delete pLua;
    }
};

#endif
