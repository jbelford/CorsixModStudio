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

#ifndef _C_MAKE_COPY_ACTION_H_
#define _C_MAKE_COPY_ACTION_H_

#include "frmFiles.h"
#include "Construct.h"
#include "Utility.h"
#include "strconv.h"

class CMakeCopyAction : public frmFiles::IHandler
{
  public:
    virtual wxString VGetExt() const { return wxT(""); }

    virtual wxString VGetAction() const { return wxT("Make a copy of this file"); }

    virtual void VHandle(wxString sFile, wxTreeItemId &oParent, wxTreeItemId &oFile)
    {
        // CFilesTreeItemData *pData = (CFilesTreeItemData*)TheConstruct->GetFilesList()->GetTree()->GetItemData(oFile);

        wxString sNewName = sFile.AfterLast('\\');
        sNewName = wxString(wxT("Copy of ")).Append(sNewName);
        sNewName = wxGetTextFromUser(wxT("New file will be called:"), VGetAction(), sNewName, TheConstruct,
                                     wxDefaultCoord, wxDefaultCoord, false);
        if (sNewName.IsEmpty())
            return;
        sNewName = sFile.BeforeLast('\\') + wxT("\\") + sNewName;

        char *saSrcFile = wxStringToAscii(sFile);
        char *saFile = wxStringToAscii(sNewName);
        try
        {
            DoExtract(saSrcFile, saFile);
        }
        catch (CRainmanException *pE)
        {
            ErrorBoxE(pE);
            delete[] saFile;
            delete[] saSrcFile;
            return;
        }

        {
            wxString sDir = sNewName.BeforeLast('\\');
            auto itrResult = TheConstruct->GetFileService().Iterate(sDir);
            IDirectoryTraverser::IIterator *pDir = itrResult ? itrResult.value().release() : nullptr;
            TheConstruct->GetFilesList()->UpdateDirectoryChildren(oParent, pDir);
            delete pDir;
            wxMessageBox(wxT("File copied"), VGetAction(), wxICON_INFORMATION, TheConstruct);
        }
        delete[] saFile;
        delete[] saSrcFile;
    }

    static void DoExtract(char *saFile, char *saDestFile, char *p4mbBuffer = 0)
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
            auto outResult = TheConstruct->GetFileService().OpenOutputStream(AsciiTowxString(saDestFile), true);
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
