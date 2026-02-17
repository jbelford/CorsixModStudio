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
#include "frame/Construct.h"
#include "common/Utility.h"
#include "common/strconv.h"
#include "common/strings.h"

class CSmfWavAction : public frmFiles::IHandler
{
  public:
    virtual wxString VGetExt() const { return wxT("smf"); }

    virtual wxString VGetAction() const { return wxT("Convert SMF to WAV file"); }

    virtual void VHandle(wxString sFile, wxTreeItemId &oParent, wxTreeItemId &oFile)
    {
        // CFilesTreeItemData *pData = (CFilesTreeItemData*)TheConstruct->GetFilesList()->GetTree()->GetItemData(oFile);
        auto saFile = wxStringToAscii(sFile);
        try
        {
            DoExtract(saFile.get());
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
            wxMessageBox(AppStr(extract_good), VGetAction(), wxICON_INFORMATION, TheConstruct);
        }
    }

    static void DoExtract(char *saFile, char *p4mbBuffer = 0)
    {
        UNUSED(saFile);
        UNUSED(p4mbBuffer);
        /*
        char* pBuffer = p4mbBuffer;
        long iLen;
        IFileStore::IStream *pIn = 0;
        IFileStore::IOutputStream *pOut = 0;
        CModuleFile* pMod = TheConstruct->GetModule();
        try
        {
            pIn = pMod->VOpenStream(saFile);
            char *sDotPos
            pOut = pMod->VOpenOutputStream(saFile, true);
        }
        catch(CRainmanException *pE)
        {
            delete pIn;
            delete pOut;
            throw new CModStudioException(pE, __FILE__, __LINE__, "Unable to open streams for \'%s\'", saFile);
        }
        try
        {
            pIn->VSeek(0, IFileStore::IStream::SL_End);
            iLen = pIn->VTell();
            pIn->VSeek(0, IFileStore::IStream::SL_Root);
        }
        catch(CRainmanException *pE)
        {
            delete pIn;
            delete pOut;
            throw new CModStudioException(pE, __FILE__, __LINE__, "Seek/Tell problem on streams for \'%s\'", saFile);
        }
        if(!pBuffer) pBuffer = new char[4194304]; //4mb
        if(!pBuffer)
        {
            delete pIn;
            delete pOut;
            throw new CModStudioException(__FILE__, __LINE__, "Cannot allocate memory");
        }
        while(iLen)
        {
            long iLen2 = iLen > 4194304 ? 4194304 : iLen;
            try
            {
                pIn->VRead(iLen2, 1, pBuffer);
                pOut->VWrite(iLen2, 1, pBuffer);
            }
            catch(CRainmanException *pE)
            {
                delete pIn;
                delete pOut;
                if(!p4mbBuffer) delete[] pBuffer;
                throw new CModStudioException(pE, __FILE__, __LINE__, "Read/Write problem on streams for \'%s\'",
        saFile);
            }
            iLen -= iLen2;
        }

        if(!p4mbBuffer) delete[] pBuffer;
        delete pIn;
        delete pOut;
        */
    }
};
