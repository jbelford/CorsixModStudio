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
#include "actions/CRgdToLuaDumpAction.h"
#include <wx/progdlg.h>

class CLuaDumpFolder : public frmFiles::IHandler
{
  protected:
    wxProgressDialog *m_pProgress;

  public:
    CLuaDumpFolder() : m_pProgress(0) {}
    virtual wxString VGetExt() const { return wxT(""); }

    virtual wxString VGetAction() const { return wxT("Dump all RGDs in this folder to Lua"); }

    virtual size_t Recurse(wxString sFolder, wxTreeItemId &oParent, bool bCountOnly, size_t iCountBase,
                           size_t iCountDiv)
    {
        size_t iCount = 0;
        wxTreeItemIdValue oCookie;
        wxTreeCtrl *pTree = TheConstruct->GetFilesList()->GetTree();
        wxTreeItemId oChild = pTree->GetFirstChild(oParent, oCookie);
        // const char* sThisMod = TheConstruct->GetModule()->GetFileMapName();
        while (oChild.IsOk())
        {
            CFilesTreeItemData *pData = (CFilesTreeItemData *)pTree->GetItemData(oChild);
            if (pData->sMod)
            {
                wxString sFile = pTree->GetItemText(oChild);
                if (sFile.AfterLast('.').IsSameAs(wxT("rgd"), false))
                {
                    ++iCount;
                    if (!bCountOnly)
                    {
                        int iPVal = (int)((iCountBase + iCount) / iCountDiv);
                        m_pProgress->Update(iPVal, sFile);

                        sFile = sFolder + wxT("\\") + pTree->GetItemText(oChild);
                        auto saFile = wxStringToAscii(sFile);
                        try
                        {
                            CRgdToLuaDumpAction::DoConvert(saFile.get());
                        }
                        catch (const CRainmanException &e)
                        {
                            ErrorBoxE(e);
                        }
                    }
                }
            }
            else
            {
                wxString sNewPath = sFolder;
                sNewPath.Append('\\');
                sNewPath.Append(pTree->GetItemText(oChild));
                iCount += Recurse(sNewPath, oChild, bCountOnly, iCountBase + iCount, iCountDiv);
            }
            oChild = pTree->GetNextChild(oChild, oCookie);
        }
        if (!bCountOnly)
        {
            if (pTree->IsExpanded(oParent))
            {
                auto itrResult = TheConstruct->GetFileService().Iterate(sFolder);
                IDirectoryTraverser::IIterator *pDir = itrResult ? itrResult.value().release() : nullptr;
                TheConstruct->GetFilesList()->UpdateDirectoryChildren(oParent, pDir);
                delete pDir;
            }
        }
        return iCount;
    }

    virtual void VHandle(wxString sFile, wxTreeItemId &oParent, wxTreeItemId &oFile)
    {
        UNUSED(oFile);
        size_t iCount = Recurse(sFile, oParent, true, 0, 1);
        size_t iDiv = 1;
        while ((iCount / iDiv) > (size_t)0x200)
        {
            ++iDiv;
        }
        m_pProgress = new wxProgressDialog(VGetAction(), wxT(""), (int)(iCount / iDiv) + 1, TheConstruct,
                                           wxPD_SMOOTH | wxPD_AUTO_HIDE | wxPD_ELAPSED_TIME | wxPD_REMAINING_TIME);
        Recurse(sFile, oParent, false, 0, iDiv);
        ThemeColours::ShowMessageBox(wxT("Batch dump complete"), VGetAction(), wxICON_INFORMATION, TheConstruct);
        delete m_pProgress;
    }
};
