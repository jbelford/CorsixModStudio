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

#include "frmFiles.h"
#include "Construct.h"
#include "Utility.h"
#include "strconv.h"
#include "strings.h"
#include <rainman/formats/CChunkyFile.h>

class CRGODeBurnAction : public frmFiles::IHandler
{
  public:
    virtual wxString VGetExt() const { return wxT("rgo"); }

    virtual wxString VGetAction() const { return wxT("Convert RGO to DataGeneric-RGO file"); }

    virtual void VHandle(wxString sFile, wxTreeItemId &oParent, wxTreeItemId &oFile)
    {
        // CFilesTreeItemData *pData = (CFilesTreeItemData*)TheConstruct->GetFilesList()->GetTree()->GetItemData(oFile);
        auto saFile = wxStringToAscii(sFile);
        try
        {
            DoDeBurn(saFile.get());
        }
        catch (CRainmanException *pE)
        {
            ErrorBoxE(pE);
            return;
        }

        {
            wxString sDir = sFile.BeforeLast('\\');
            auto itrResult = TheConstruct->GetFileService().Iterate(sDir);
            IDirectoryTraverser::IIterator *pDir = itrResult ? itrResult.value().release() : nullptr;
            TheConstruct->GetFilesList()->UpdateDirectoryChildren(oParent, pDir);
            delete pDir;
            wxMessageBox(AppStr(rgodeburn_good), VGetAction(), wxICON_INFORMATION, TheConstruct);
        }
    }

    static void DoDeBurn(char *saFile)
    {
        IFileStore::IStream *pIn = 0;
        try
        {
            auto inResult = TheConstruct->GetFileService().OpenStream(AsciiTowxString(saFile));
            if (!inResult)
                throw new CModStudioException(0, __FILE__, __LINE__, "Unable to open streams for \'%s\'", saFile);
            pIn = inResult.value().release();
        }
        catch (CRainmanException *pE)
        {
            delete pIn;
            throw new CModStudioException(pE, __FILE__, __LINE__, "Unable to open streams for \'%s\'", saFile);
        }
        CChunkyFile oChunky;
        try
        {
            oChunky.Load(pIn);
        }
        catch (CRainmanException *pE)
        {
            delete pIn;
            throw new CModStudioException(pE, __FILE__, __LINE__, "Unable to load \'%s\' as chunky", saFile);
        }
        delete pIn;
        pIn = 0;
        // 1 : find and remove FOLDMODL\DATAINFO
        CChunkyFile::CChunk *pModl = oChunky.GetChildByName("MODL", CChunkyFile::CChunk::T_Folder);
        if (pModl)
        {
            size_t iL = pModl->GetChildCount();
            for (size_t i = 0; i < iL; ++i)
            {
                CChunkyFile::CChunk *pChild = pModl->GetChild(i);
                if (pChild->GetType() == CChunkyFile::CChunk::T_Data && strcmp(pChild->GetName(), "INFO") == 0)
                {
                    pModl->RemoveChild(i);
                    break;
                }
            }
        }
        // 2: find and remove DATAFBIF
        size_t iL = oChunky.GetChildCount();
        for (size_t i = 0; i < iL; ++i)
        {
            CChunkyFile::CChunk *pChild = oChunky.GetChild(i);
            if (pChild->GetType() == CChunkyFile::CChunk::T_Data && strcmp(pChild->GetName(), "FBIF") == 0)
            {
                oChunky.RemoveChild(i);
                break;
            }
        }
        // 3 : save
        IFileStore::IOutputStream *pOut = 0;
        try
        {
            auto outResult = TheConstruct->GetFileService().OpenOutputStream(AsciiTowxString(saFile), true);
            if (!outResult)
                throw new CModStudioException(0, __FILE__, __LINE__, "Unable to open output stream for \'%s\'", saFile);
            pOut = outResult.value().release();
        }
        catch (CRainmanException *pE)
        {
            delete pOut;
            throw new CModStudioException(pE, __FILE__, __LINE__, "Unable to open output stream for \'%s\'", saFile);
        }
        try
        {
            oChunky.Save(pOut);
        }
        catch (CRainmanException *pE)
        {
            delete pOut;
            throw new CModStudioException(pE, __FILE__, __LINE__, "Unable to save \'%s\' as chunky", saFile);
        }
        delete pOut;
    }
};
