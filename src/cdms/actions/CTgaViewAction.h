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
#include "frmImage.h"
#include "Construct.h"
#include "Utility.h"
#include "actions/ActionUtil.h"
#include <rainman/formats/CRgtFile.h>

class CTgaViewAction : public frmFiles::IHandler
{
  public:
    virtual wxString VGetExt() const { return wxT("tga"); }

    virtual wxString VGetAction() const { return wxT("View TGA"); }

    virtual void VHandle(wxString sFile, wxTreeItemId &oParent, wxTreeItemId &oFile)
    {
        UNUSED(oFile);
        auto streamResult = TheConstruct->GetFileService().OpenStream(sFile);
        if (!streamResult)
        {
            ErrorBox("Cannot open file");
            return;
        }
        auto &stream = streamResult.value();

        bool bIs32 = true;
        CRgtFile *pRgt = new CRgtFile;
        pRgt->LoadTGA(stream.get(), false, &bIs32);

        frmImageViewer *pForm;
        TheConstruct->GetTabs()->AddPage(
            pForm = new frmImageViewer(oParent, sFile, TheConstruct->GetTabs(), -1, pRgt, true),
            wxString().Append(wxT("TGA")).Append(wxT(" [")).Append(OnlyFilename(sFile)).Append(wxT("]")), true);
        pForm->SetIsTga(!bIs32);
    }
};
