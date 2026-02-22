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
#include <rainman/formats/CRgtFile.h>

class CDdsToRgtAction : public frmFiles::IHandler
{
  public:
    virtual wxString VGetExt() const { return wxT("dds"); }

    virtual wxString VGetAction() const { return wxT("Convert DDS to RGT"); }

    virtual void VHandle(wxString sFile, wxTreeItemId &oParent, wxTreeItemId &oFile)
    {
        // CFilesTreeItemData *pData = (CFilesTreeItemData*)TheConstruct->GetFilesList()->GetTree()->GetItemData(oFile);
        auto saFile = wxStringToAscii(sFile);
        wxString sConvertedTo;
        try
        {
            DoConvert(saFile.get());
        }
        catch (const CRainmanException &e)
        {
            ErrorBoxE(e);
            return;
        }

        char *saDir = strdup(saFile.get()), *pSlash;
        pSlash = strrchr(saDir, '\\');
        if (pSlash)
        {
            *pSlash = 0;
        }
        else
        {
            *saDir = 0;
        }
        auto itrResult = TheConstruct->GetFileService().Iterate(AsciiTowxString(saDir));
        free(saDir);
        IDirectoryTraverser::IIterator *pDir = itrResult ? itrResult.value().release() : nullptr;
        TheConstruct->GetFilesList()->UpdateDirectoryChildren(oParent, pDir);
        delete pDir;

        ThemeColours::ShowMessageBox(AppStr(rgt_makefromddsgood), VGetAction(), wxICON_INFORMATION, TheConstruct);
    }

    static void DoConvert(char *saFile)
    {
        IFileStore::IStream *pIn = 0;
        IFileStore::IOutputStream *pOut = 0;
        wxString sOutFormat;
        try
        {
            auto inResult = TheConstruct->GetFileService().OpenStream(AsciiTowxString(saFile));
            if (!inResult)
            {
                throw CModStudioException(0, __FILE__, __LINE__, "Cannot open input stream for \'%s\'", saFile);
            }
            pIn = inResult.value().release();
        }
        catch (const CRainmanException &e)
        {
            delete pIn;
            throw CModStudioException(e, __FILE__, __LINE__, "Cannot open input stream for \'%s\'", saFile);
        }

        CRgtFile oRgt;
        try
        {
            oRgt.LoadDDS(pIn);
        }
        catch (const CRainmanException &e)
        {
            delete pIn;
            throw CModStudioException(e, __FILE__, __LINE__, "Cannot load file \'%s\'", saFile);
        }

        char *saOutFile = strdup(saFile);
        if (!saOutFile)
        {
            throw CModStudioException(__FILE__, __LINE__, "Memory allocation error");
        }
        strcpy(strchr(saOutFile, '.'), ".rgt");

        try
        {
            auto outResult = TheConstruct->GetFileService().OpenOutputStream(AsciiTowxString(saOutFile), true);
            if (!outResult)
            {
                throw CModStudioException(0, __FILE__, __LINE__, "Cannot open output stream for \'%s\'", saFile);
            }
            pOut = outResult.value().release();
        }
        catch (const CRainmanException &e)
        {
            delete pIn;
            free(saOutFile);
            throw CModStudioException(e, __FILE__, __LINE__, "Cannot open output stream for \'%s\'", saFile);
        }

        try
        {
            oRgt.Save(pOut);
        }
        catch (const CRainmanException &e)
        {
            delete pIn;
            delete pOut;
            free(saOutFile);
            throw CModStudioException(e, __FILE__, __LINE__, "Cannot save file \'%s\'", saFile);
        }

        delete pIn;
        delete pOut;
        free(saOutFile);
    }
};
