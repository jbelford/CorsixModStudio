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
#include "views/frmRgmMaterialEditor.h"
#include "frame/Construct.h"
#include "common/Utility.h"
#include "actions/ActionUtil.h"
#include <rainman/formats/CRgmFile.h>

class CRGMMaterialAction : public frmFiles::IHandler
{
  public:
    virtual wxString VGetExt() const { return wxT("rgm"); }

    virtual wxString VGetAction() const { return wxT("Set RGM textures"); }

    virtual void VHandle(wxString sFile, wxTreeItemId &oParent, wxTreeItemId &oFile)
    {
        UNUSED(oFile);
        frmRgmMaterialEditor *pForm;
        TheConstruct->GetTabs()->AddPage(
            pForm = new frmRgmMaterialEditor(oParent, sFile, TheConstruct->GetTabs(), -1),
            wxString().Append(wxT("RGM")).Append(wxT(" [")).Append(OnlyFilename(sFile)).Append(wxT("]")), true);

        auto streamResult = TheConstruct->GetFileService().OpenStream(sFile);
        if (!streamResult)
        {
            ErrorBox("Cannot open file");
            return;
        }
        auto &stream = streamResult.value();

        CRgmFile *pRgm = new CRgmFile;
        pRgm->Load(stream.get());

        pForm->SetObject(pRgm, true);
    }
};
