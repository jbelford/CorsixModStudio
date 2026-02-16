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

#ifndef _C_SUA_ACTION_H_
#define _C_SUA_ACTION_H_

#include "frmFiles.h"
#include "frmRgdEditor.h"
#include "Construct.h"
#include "Utility.h"
#include "strconv.h"
#include "actions/ActionUtil.h"

class CSuaAction : public frmFiles::IHandler
{
  public:
    virtual wxString VGetExt() const { return wxT("sua"); }

    virtual wxString VGetAction() const { return wxT("View as LUA/SUA file"); }

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
        char *saFile = wxStringToAscii(sFile);
        CLuaFile *pLua = CLuaAction::DoLoad(stream.get(), saFile);
        delete[] saFile;
        if (pLua)
        {
            frmRGDEditor *pForm;
            TheConstruct->GetTabs()->AddPage(
                pForm = new frmRGDEditor(oParent, sFile, TheConstruct->GetTabs(), -1),
                wxString().Append(wxT("SUA")).Append(wxT(" [")).Append(OnlyFilename(sFile)).Append(wxT("]")), true);
            if (!pForm->FillFromMetaTable(pLua))
            {
                ErrorBox("Cannot load file");
                delete pLua;
                return;
            }
        }
    }
};

#endif
