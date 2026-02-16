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

#ifndef _C_AI_ACTION_H_
#define _C_AI_ACTION_H_

#include "frmFiles.h"
#include "frmScarEditor.h"
#include "Construct.h"
#include "Utility.h"
#include "actions/ActionUtil.h"

class CAiAction : public frmFiles::IHandler
{
  public:
    virtual wxString VGetExt() const { return wxT("ai"); }

    virtual wxString VGetAction() const { return wxT("View as AI file"); }

    virtual void VHandle(wxString sFile, wxTreeItemId &oParent, wxTreeItemId &oFile)
    {
        UNUSED(oFile);
        frmScarEditor *pForm;
        TheConstruct->GetTabs()->AddPage(
            pForm = new frmScarEditor(oParent, sFile, TheConstruct->GetTabs(), -1, wxDefaultPosition, wxDefaultSize, 0),
            wxString().Append(wxT("AI")).Append(wxT(" [")).Append(OnlyFilename(sFile)).Append(wxT("]")), true);

        auto streamResult = TheConstruct->GetFileService().OpenStream(sFile);
        if (!streamResult)
        {
            ErrorBox("Cannot open file");
            return;
        }
        IFileStore::IStream *pStream = streamResult.value().release();

        pForm->Load(pStream);

        delete pStream;
    }
};

#endif
