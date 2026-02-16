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

#ifndef _C_BFXRGT_DEBURN_ACTION_H_
#define _C_BFXRGT_DEBURN_ACTION_H_

#include "frmFiles.h"
#include "Construct.h"
#include "Utility.h"
#include "strconv.h"
#include "strings.h"
#include "actions/CBfxToLuaDumpAction.h"
#include <rainman/formats/CRgtFile.h>
#include <rainman/core/RainmanLog.h>
#include <wx/progdlg.h>
#include <zlib.h>
extern "C"
{
#include <lauxlib.h>
#include <lualib.h>
}

class CBFXRGTDeBurnAction : public frmFiles::IHandler
{
  protected:
    wxProgressDialog *m_pProgress;

  public:
    CBFXRGTDeBurnAction() : m_pProgress(0) {}
    virtual wxString VGetExt() const { return wxT(""); }

    virtual wxString VGetAction() const { return wxT("Unburn all BFX/RGT files in this folder"); }

    virtual size_t Recurse(wxString sFolder, wxTreeItemId &oParent, bool bCountOnly, size_t iCountBase,
                           size_t iCountDiv)
    {
        size_t iCount = 0;
        wxTreeItemIdValue oCookie;
        wxTreeCtrl *pTree = TheConstruct->GetFilesList()->GetTree();
        wxTreeItemId oChild = pTree->GetFirstChild(oParent, oCookie);
        while (oChild.IsOk())
        {
            CFilesTreeItemData *pData = (CFilesTreeItemData *)pTree->GetItemData(oChild);
            if (pData->sMod)
            {
                wxString sFile = pTree->GetItemText(oChild);
                if (sFile.AfterLast('.').IsSameAs(wxT("bfx"), false) ||
                    sFile.AfterLast('.').IsSameAs(wxT("rgt"), false))
                {
                    ++iCount;
                    if (!bCountOnly)
                    {
                        int iPVal = (int)((iCountBase + iCount) / iCountDiv);
                        m_pProgress->Update(iPVal, sFile);

                        sFile = sFolder + wxT("\\") + pTree->GetItemText(oChild);
                        char *saFile = wxStringToAscii(sFile);

                        if (sFile.AfterLast('.').IsSameAs(wxT("bfx"), false))
                        {
                            CBfxToLuaDumpAction::DoConvert(saFile, L);
                        }
                        else
                        {
                            auto streamResult = TheConstruct->GetFileService().OpenStream(sFile);
                            if (!streamResult)
                            {
                                delete[] saFile;
                                ErrorBox("Cannot open file");
                                return iCount;
                            }
                            IFileStore::IStream *pStream = streamResult.value().release();

                            CRgtFile *pRgt = new CRgtFile;
                            pRgt->Load(pStream);

                            strcpy(strrchr(saFile, '.'), ".tga");
                            auto outResult =
                                TheConstruct->GetFileService().OpenOutputStream(AsciiTowxString(saFile), true);
                            IFileStore::IOutputStream *pOutStr = outResult ? outResult.value().release() : nullptr;
                            pRgt->SaveTGA(pOutStr);

                            delete pRgt;
                            delete pOutStr;
                            delete pStream;
                        }

                        delete[] saFile;
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

    lua_State *L;
    void load_lua()
    {
        L = lua_open();
        luaopen_base(L);

        FILE *f = _wfopen(AppStr(app_bfxmapfile), wxT("rb"));
        if (!f)
        {
            CDMS_LOG_WARN("Could not open BFX map file");
            return;
        }
        unsigned long iSizeComp, iSizeDecomp;
        fread(&iSizeDecomp, sizeof(unsigned long), 1, f);
        fread(&iSizeComp, sizeof(unsigned long), 1, f);
        unsigned char *pCompressed = new unsigned char[iSizeComp];
        fread(pCompressed, 1, iSizeComp, f);
        fclose(f);

        unsigned char *pUncompressed = new unsigned char[iSizeDecomp];
        uncompress(pUncompressed, &iSizeDecomp, pCompressed, iSizeComp);
        delete[] pCompressed;

        int iE = luaL_loadbuffer(L, (const char *)pUncompressed, iSizeDecomp, "map");
        delete[] pUncompressed;

        if (iE == 0)
        {
            iE = lua_pcall(L, 0, 0, 0);
            if (iE == 0)
                lua_getglobal(L, "map");
        }
        if (iE)
        {
            const char *sE = lua_tostring(L, -1);
            sE = sE;
        }
    }

    virtual void VHandle(wxString sFile, wxTreeItemId &oParent, wxTreeItemId &oFile)
    {
        UNUSED(oFile);
        load_lua();

        size_t iCount = Recurse(sFile, oParent, true, 0, 1);
        size_t iDiv = 1;
        while ((iCount / iDiv) > (size_t)0x200)
        {
            ++iDiv;
        }
        m_pProgress = new wxProgressDialog(VGetAction(), wxT(""), (int)(iCount / iDiv) + 1, TheConstruct,
                                           wxPD_SMOOTH | wxPD_AUTO_HIDE | wxPD_ELAPSED_TIME | wxPD_REMAINING_TIME);
        Recurse(sFile, oParent, false, 0, iDiv);
        wxMessageBox(AppStr(bfxrgt_convertgood), VGetAction(), wxICON_INFORMATION, TheConstruct);
        delete m_pProgress;

        lua_close(L);
    }
};

#endif
