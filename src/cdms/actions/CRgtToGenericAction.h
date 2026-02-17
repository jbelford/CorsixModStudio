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
#include <rainman/formats/CRgtFile.h>

class CRgtToGenericAction : public frmFiles::IHandler
{
  public:
    virtual wxString VGetExt() const { return wxT("rgt"); }

    virtual wxString VGetAction() const { return wxT("Dump RGT to TGA/DDS"); }

    virtual void VHandle(wxString sFile, wxTreeItemId &oParent, wxTreeItemId &oFile)
    {
        // CFilesTreeItemData *pData = (CFilesTreeItemData*)TheConstruct->GetFilesList()->GetTree()->GetItemData(oFile);
        auto saFile = wxStringToAscii(sFile);
        wxString sConvertedTo;
        try
        {
            sConvertedTo = DoConvert(saFile.get());
        }
        catch (CRainmanException *pE)
        {
            ErrorBoxE(pE);
            return;
        }

        char *saDir = strdup(saFile.get()), *pSlash;
        pSlash = strrchr(saDir, '\\');
        if (pSlash)
            *pSlash = 0;
        else
            *saDir = 0;
        auto itrResult = TheConstruct->GetFileService().Iterate(AsciiTowxString(saDir));
        free(saDir);
        IDirectoryTraverser::IIterator *pDir = itrResult ? itrResult.value().release() : nullptr;
        TheConstruct->GetFilesList()->UpdateDirectoryChildren(oParent, pDir);
        delete pDir;

        wxMessageBox(wxString(wxT("")).Append(AppStr(rgt_convertgood)).Append(sConvertedTo), VGetAction(),
                     wxICON_INFORMATION, TheConstruct);
    }

    /*!
        \return Returns a string describing the type of type created, eg. "DDS DXT1", "TGA 32bit RGBA"
    */
    static wxString DoConvert(char *saFile)
    {
        IFileStore::IStream *pIn = 0;
        IFileStore::IOutputStream *pOut = 0;
        wxString sOutFormat;
        try
        {
            auto inResult = TheConstruct->GetFileService().OpenStream(AsciiTowxString(saFile));
            if (!inResult)
                throw new CModStudioException(0, __FILE__, __LINE__, "Cannot open input stream for \'%s\'", saFile);
            pIn = inResult.value().release();
        }
        catch (CRainmanException *pE)
        {
            delete pIn;
            throw new CModStudioException(pE, __FILE__, __LINE__, "Cannot open input stream for \'%s\'", saFile);
        }

        CRgtFile oRgt;
        try
        {
            oRgt.Load(pIn);
        }
        catch (CRainmanException *pE)
        {
            delete pIn;
            throw new CModStudioException(pE, __FILE__, __LINE__, "Cannot load file \'%s\'", saFile);
        }

        char *saOutFile = strdup(saFile);
        if (!saOutFile)
            throw new CModStudioException(__FILE__, __LINE__, "Memory allocation error");
        switch (oRgt.GetImageFormat())
        {
        case CRgtFile::IF_Tga:
            strcpy(strchr(saOutFile, '.'), ".tga");
            sOutFormat = wxT("TGA 32bit RGBA");
            break;
        case CRgtFile::IF_Dxtc:
            strcpy(strchr(saOutFile, '.'), ".dds");
            sOutFormat = wxT("DDS ");
            switch (oRgt.GetProperty(CRgtFile::IP_CompressionLevel))
            {
            case 1:
                sOutFormat.Append(wxT("DXT1"));
                break;

            case 2:
                sOutFormat.Append(wxT("DXT2"));
                break;

            case 3:
                sOutFormat.Append(wxT("DXT3"));
                break;

            case 4:
                sOutFormat.Append(wxT("DXT4"));
                break;

            case 5:
                sOutFormat.Append(wxT("DXT5"));
                break;

            default:
                sOutFormat.Append(wxT("unknown compression"));
                break;
            };
            break;
        default:
            delete pIn;
            free(saOutFile);
            throw new CModStudioException(0, __FILE__, __LINE__, "Uknown image format of \'%s\'", saFile);
        }
        if (oRgt.GetProperty(CRgtFile::IP_MipLevelCount) > 1)
        {
            sOutFormat.Append(wxT(" with mip levels"));
        }

        try
        {
            auto outResult = TheConstruct->GetFileService().OpenOutputStream(AsciiTowxString(saOutFile), true);
            if (!outResult)
                throw new CModStudioException(0, __FILE__, __LINE__, "Cannot open output stream for \'%s\'", saFile);
            pOut = outResult.value().release();
        }
        catch (CRainmanException *pE)
        {
            delete pIn;
            free(saOutFile);
            throw new CModStudioException(pE, __FILE__, __LINE__, "Cannot open output stream for \'%s\'", saFile);
        }

        try
        {
            // oRgt.SaveTGA(pOut);
            oRgt.SaveGeneric(pOut);
        }
        catch (CRainmanException *pE)
        {
            delete pIn;
            delete pOut;
            free(saOutFile);
            throw new CModStudioException(pE, __FILE__, __LINE__, "Cannot save generic of \'%s\'", saFile);
        }

        delete pIn;
        delete pOut;
        free(saOutFile);
        return sOutFormat;
    }
};
