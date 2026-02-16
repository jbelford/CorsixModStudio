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

#ifndef _C_EXTRACT_ACTION_H_
#define _C_EXTRACT_ACTION_H_

#include "frmFiles.h"
#include "Construct.h"
#include "Utility.h"
#include "strconv.h"
#include "strings.h"

class CExtractAction : public frmFiles::IHandler
{
  public:
    virtual wxString VGetExt() const { return wxT(""); }

    virtual wxString VGetAction() const { return wxT("Extract this file"); }

    virtual void VHandle(wxString sFile, wxTreeItemId &oParent, wxTreeItemId &oFile)
    {
        CFilesTreeItemData *pData = (CFilesTreeItemData *)TheConstruct->GetFilesList()->GetTree()->GetItemData(oFile);
        if (AsciiTowxString(pData->sMod).IsSameAs(TheConstruct->GetModuleService().GetFileMapName(), false))
        {
            const char *sTmp = strrchr(pData->sSource, '.');
            if (!sTmp || (stricmp(sTmp, ".sga") != 0))
            {
                wxMessageBox(AppStr(extract_already), VGetAction(), wxICON_INFORMATION, TheConstruct);
                return;
            }
        }
        char *saFile = wxStringToAscii(sFile);
        try
        {
            DoExtract(saFile);
        }
        catch (CRainmanException *pE)
        {
            ErrorBoxE(pE);
            delete[] saFile;
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
        delete[] saFile;
    }

    static void DoExtract(char *saFile, char *p4mbBuffer = 0)
    {
        char *pBuffer = p4mbBuffer;
        long iLen;
        IFileStore::IStream *pIn = 0;
        IFileStore::IOutputStream *pOut = 0;
        try
        {
            auto inResult = TheConstruct->GetFileService().OpenStream(AsciiTowxString(saFile));
            if (!inResult)
                throw new CModStudioException(0, __FILE__, __LINE__, "Unable to open streams for \'%s\'", saFile);
            pIn = inResult.value().release();
            auto outResult = TheConstruct->GetFileService().OpenOutputStream(AsciiTowxString(saFile), true);
            if (!outResult)
            {
                delete pIn;
                pIn = 0;
                throw new CModStudioException(0, __FILE__, __LINE__, "Unable to open streams for \'%s\'", saFile);
            }
            pOut = outResult.value().release();
        }
        catch (CRainmanException *pE)
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
        catch (CRainmanException *pE)
        {
            delete pIn;
            delete pOut;
            throw new CModStudioException(pE, __FILE__, __LINE__, "Seek/Tell problem on streams for \'%s\'", saFile);
        }
        if (!pBuffer)
            pBuffer = new char[4194304]; // 4mb
        if (!pBuffer)
        {
            delete pIn;
            delete pOut;
            throw new CModStudioException(__FILE__, __LINE__, "Cannot allocate memory");
        }
        while (iLen)
        {
            long iLen2 = iLen > 4194304 ? 4194304 : iLen;
            try
            {
                pIn->VRead(iLen2, 1, pBuffer);
                pOut->VWrite(iLen2, 1, pBuffer);
            }
            catch (CRainmanException *pE)
            {
                delete pIn;
                delete pOut;
                if (!p4mbBuffer)
                    delete[] pBuffer;
                throw new CModStudioException(pE, __FILE__, __LINE__, "Read/Write problem on streams for \'%s\'",
                                              saFile);
            }
            iLen -= iLen2;
        }

        if (!p4mbBuffer)
            delete[] pBuffer;
        delete pIn;
        delete pOut;
    }
};

#endif
