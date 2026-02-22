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

#pragma once

#include "views/frmFiles.h"
#include "common/ThemeColours.h"
#include "frame/Construct.h"
#include "common/Utility.h"
#include "common/strconv.h"
#include "common/strings.h"
#include "common/Common.h"
#include <rainman/formats/CRgdFile.h>
#include <memory>

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
        {
            iRGDVersion = 3;
        }

        auto streamResult = TheConstruct->GetFileService().OpenStream(sFile);
        if (!streamResult)
        {
            ErrorBox("Cannot open file");
            return;
        }
        auto &stream = streamResult.value();
        auto saFile = wxStringToAscii(sFile);

        auto pLua = std::unique_ptr<CLuaFile>(CLuaAction::DoLoad(stream.get(), saFile.get()));
        if (pLua)
        {
            auto pRgd = std::make_unique<CRgdFile>();
            if (pRgd)
            {
                pRgd->SetHashTable(TheConstruct->GetRgdHashTable());
                try
                {
                    pRgd->Load(pLua.get(), iRGDVersion);
                }
                catch (const CRainmanException &e)
                {
                    ErrorBoxE(e);
                    goto after_rgd_loaded_cleany_code;
                }
                {
                    bool bMovedTOC = ConvertLuaFilenameToRgd(saFile.get());
                    BackupFile(TheConstruct->GetModuleService().GetModule(), AsciiTowxString(saFile.get()));
                    auto outResult =
                        TheConstruct->GetFileService().OpenOutputStream(AsciiTowxString(saFile.get()), true);
                    if (outResult)
                    {
                        auto &outStream = outResult.value();
                        auto itrResult =
                            TheConstruct->GetFileService().Iterate(AsciiTowxString(saFile.get()).BeforeLast('\\'));
                        auto pDir = itrResult
                                        ? std::unique_ptr<IDirectoryTraverser::IIterator>(itrResult.value().release())
                                        : nullptr;
                        frmFiles *pFiles = TheConstruct->GetFilesList();
                        if (bMovedTOC)
                        {
                            pFiles->UpdateDirectoryChildren(pFiles->FindFile(AsciiTowxString(saFile.get()), true),
                                                            pDir.get());
                        }
                        else
                        {
                            pFiles->UpdateDirectoryChildren(oParent, pDir.get());
                        }
                        try
                        {
                            pRgd->Save(outStream.get());
                            ThemeColours::ShowMessageBox(AppStr(rgd_burngood), VGetAction(), wxICON_INFORMATION,
                                                         TheConstruct);
                        }
                        catch (const CRainmanException &e)
                        {
                            ErrorBoxE(e);
                            RestoreBackupFile(TheConstruct->GetModuleService().GetModule(),
                                              AsciiTowxString(saFile.get()));
                        }
                    }
                    else
                    {
                        ErrorBoxAS(err_write);
                    }
                }
            after_rgd_loaded_cleany_code:;
            }
            else
            {
                ErrorBoxAS(err_memory);
            }
        }
    }
};
