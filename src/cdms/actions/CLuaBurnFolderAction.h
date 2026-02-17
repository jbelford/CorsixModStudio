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
#include "frame/Construct.h"
#include "common/Utility.h"
#include "common/strconv.h"
#include "common/strings.h"
#include "common/Common.h"
#include "actions/CLuaBurnAction.h"
#include <rainman/formats/CRgdFile.h>
#include <wx/progdlg.h>

class CLuaBurnFolderAction : public frmFiles::IHandler
{
  protected:
    wxProgressDialog *m_pProgress;
    CLuaFileCache *m_pCache;
    long m_iRGDVersion;
    bool m_bCheckSkip;

  public:
    CLuaBurnFolderAction() : m_pProgress(0)
    {
        m_bCheckSkip = true;
        m_iRGDVersion = 1;
        if (TheConstruct->GetModuleService().GetModuleType() != CModuleFile::MT_DawnOfWar)
            m_iRGDVersion = 3;
    }
    virtual wxString VGetExt() const { return wxT(""); }

    virtual wxString VGetAction() const
    {
        return wxT("Convert all LUAs (But not those in ReqiuredMods) in this folder to RGD");
    }

    virtual size_t Recurse(wxString sFolder, wxTreeItemId &oParent, bool bCountOnly, size_t iCountBase,
                           size_t iCountDiv)
    {
        size_t iCount = 0;
        wxTreeItemIdValue oCookie;
        wxTreeCtrl *pTree = TheConstruct->GetFilesList()->GetTree();
        wxTreeItemId oChild = pTree->GetFirstChild(oParent, oCookie);
        wxString sThisMod = TheConstruct->GetModuleService().GetFileMapName();
        bool bAnyMovedToC = false;
        while (oChild.IsOk())
        {
            CFilesTreeItemData *pData = (CFilesTreeItemData *)pTree->GetItemData(oChild);
            if (pData->sMod && (AsciiTowxString(pData->sMod).IsSameAs(sThisMod)))
            {
                wxString sFile = pTree->GetItemText(oChild);
                if (sFile.AfterLast('.').IsSameAs(wxT("lua"), false))
                {
                    ++iCount;
                    if (!bCountOnly)
                    {
                        if (((iCountBase + iCount) % 256) == 0)
                            m_pCache->Clear();
                        int iPVal = (int)((iCountBase + iCount) / iCountDiv);
                        m_pProgress->Update(iPVal, sFile);

                        sFile = sFolder + wxT("\\") + pTree->GetItemText(oChild);
                        auto saFile = wxStringToAscii(sFile);
                        char *saRgd = strdup(saFile.get());
                        bAnyMovedToC = CLuaBurnAction::ConvertLuaFilenameToRgd(saRgd) || bAnyMovedToC;

                        if (m_bCheckSkip)
                        {
                            tLastWriteTime oLuaWriteTime =
                                TheConstruct->GetModuleService().GetModule()->VGetLastWriteTime(saFile.get());
                            if (IsValidWriteTime(oLuaWriteTime))
                            {
                                try
                                {
                                    tLastWriteTime oRgdWriteTime =
                                        TheConstruct->GetModuleService().GetModule()->VGetLastWriteTime(saRgd);
                                    if (IsValidWriteTime(oRgdWriteTime))
                                    {
                                        if (IsWriteTimeNewer(oRgdWriteTime, oLuaWriteTime))
                                        {
                                            free(saRgd);
                                            goto rgd_is_newer;
                                        }
                                    }
                                }
                                IGNORE_EXCEPTIONS
                            }
                        }

                        auto streamResult = TheConstruct->GetFileService().OpenStream(sFile);
                        wxString sError = wxT("");
                        if (!streamResult)
                        {
                            free(saRgd);
                            sError = wxT("Cannot open file");
                        }
                        else
                        {
                            auto &stream = streamResult.value();
                            CLuaFile *pLua = CLuaAction::DoLoad(stream.get(), saFile.get(), m_pCache);
                            if (pLua)
                            {
                                CRgdFile *pRgd = new CRgdFile;
                                if (pRgd)
                                {
                                    pRgd->SetHashTable(TheConstruct->GetHashService().GetHashTable());
                                    try
                                    {
                                        pRgd->Load(pLua, m_iRGDVersion);
                                    }
                                    catch (CRainmanException *pE)
                                    {
                                        ErrorBoxE(pE);
                                        goto skip_recurse_lua_load_ok_code;
                                    }
                                    BackupFile(TheConstruct->GetModuleService().GetModule(), AsciiTowxString(saRgd));
                                    {
                                        std::unique_ptr<IFileStore::IOutputStream> outStream;
                                        try
                                        {
                                            auto outResult = TheConstruct->GetFileService().OpenOutputStream(
                                                AsciiTowxString(saRgd), true);
                                            if (outResult)
                                                outStream = std::move(outResult.value());
                                        }
                                        catch (CRainmanException *pE)
                                        {
                                            ErrorBoxE(pE);
                                        }
                                        if (outStream)
                                        {
                                            try
                                            {
                                                pRgd->Save(outStream.get());
                                            }
                                            catch (CRainmanException *pE)
                                            {
                                                ErrorBoxE(pE);
                                                RestoreBackupFile(TheConstruct->GetModuleService().GetModule(),
                                                                  AsciiTowxString(saRgd));
                                            }
                                        }
                                        else
                                        {
                                            sError = AppStr(err_write);
                                        }
                                    }
                                skip_recurse_lua_load_ok_code:
                                    delete pRgd;
                                }
                                else
                                {
                                    sError = AppStr(err_memory);
                                }
                            }
                            free(saRgd);
                            if (pLua)
                                m_pCache->FreeState(pLua->m_pLua);
                            delete pLua;
                        }
                        if (sError != wxT(""))
                        {
                            sError.Append(wxT("\n"));
                            sError.Append(sFile);
                            ErrorBoxS(sError);
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
        rgd_is_newer:
            oChild = pTree->GetNextChild(oChild, oCookie);
        }
        if (!bCountOnly)
        {
            if (bAnyMovedToC)
            {
                sFolder.Replace(sFolder.Left(7), wxT("Data"), false);
                oParent = TheConstruct->GetFilesList()->FindFile(sFolder, false);
            }
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
        m_bCheckSkip = (wxMessageBox(wxT("Skip burning a LUA if it\'s RGD is newer?"), VGetAction(), wxYES_NO,
                                     TheConstruct) == wxYES);

        size_t iCount = Recurse(sFile, oParent, true, 0, 1);
        size_t iDiv = 1;
        while ((iCount / iDiv) > (size_t)0x200)
        {
            ++iDiv;
        }
        m_pProgress = new wxProgressDialog(VGetAction(), wxT(""), (int)(iCount / iDiv) + 1, TheConstruct,
                                           wxPD_SMOOTH | wxPD_AUTO_HIDE | wxPD_ELAPSED_TIME | wxPD_REMAINING_TIME);
        m_pCache = new CLuaFileCache;
        Recurse(sFile, oParent, false, 0, iDiv);
        delete m_pCache;
        wxMessageBox(AppStr(rgd_massburngood), VGetAction(), wxICON_INFORMATION, TheConstruct);
        delete m_pProgress;
    }
};
