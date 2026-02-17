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

#include "CImagePresenter.h"
#include <rainman/core/Exception.h>
#include <rainman/io/CMemoryStore.h>

CImagePresenter::CImagePresenter(IImageView &view) : m_view(view) {}

wxString CImagePresenter::ComputeOutputPath(const wxString &sFilename, SaveFormat eTargetFormat)
{
    wxString sBase = sFilename.BeforeLast('.');
    if (sBase.empty())
        sBase = sFilename;

    switch (eTargetFormat)
    {
    case SaveFormat::RGT:
        return sBase + wxT(".rgt");
    case SaveFormat::DDS:
        return sBase + wxT(".dds");
    case SaveFormat::TGA:
        return sBase + wxT(".tga");
    }
    return sBase;
}

int CImagePresenter::CompressionToDxtLevel(int iCompressionIndex)
{
    // Indices 0 (RGB) and 1 (RGBA) are not DXT formats
    if (iCompressionIndex < 2)
        return -1;
    // Index 2 → DXT1 (level 1), Index 3 → DXT3 (level 3), Index 4 → DXT5 (level 5)
    return iCompressionIndex * 2 - 3;
}

bool CImagePresenter::WouldOverwrite(int iCurrentFormat, SaveFormat eTargetFormat)
{
    return iCurrentFormat == static_cast<int>(eTargetFormat);
}

bool CImagePresenter::SaveToStream(CRgtFile *pRgtFile, IFileStore::IOutputStream *pStream, SaveFormat eTargetFormat,
                                   int iCompressionIndex, bool bMipLevels)
{
    if (pRgtFile == nullptr || pStream == nullptr)
    {
        m_view.ShowError(wxT("Internal error: null image or output stream"));
        return false;
    }

    try
    {
        switch (eTargetFormat)
        {
        case SaveFormat::RGT:
        {
            if (iCompressionIndex == 1)
            {
                // RGT with RGBA (TGA intermediate)
                CMemoryStore::COutStream *pTempOut = CMemoryStore::OpenOutputStreamExt();
                pRgtFile->SaveTGA(pTempOut, true);
                pTempOut->VSeek(0, IFileStore::IStream::SL_Root);
                CRgtFile oRgtOut;
                oRgtOut.LoadTGA(pTempOut, bMipLevels);
                delete pTempOut;
                oRgtOut.Save(pStream);
            }
            else
            {
                // RGT with DXTC compression
                int iDxtLevel = CompressionToDxtLevel(iCompressionIndex);
                CMemoryStore::COutStream *pTempOut = CMemoryStore::OpenOutputStreamExt();
                pRgtFile->SaveDDS(pTempOut, iDxtLevel, bMipLevels);
                pTempOut->VSeek(0, IFileStore::IStream::SL_Root);
                CRgtFile oRgtOut;
                oRgtOut.LoadDDS(pTempOut);
                delete pTempOut;
                oRgtOut.Save(pStream);
            }
            break;
        }
        case SaveFormat::DDS:
        {
            int iDxtLevel = CompressionToDxtLevel(iCompressionIndex);
            pRgtFile->SaveDDS(pStream, iDxtLevel, bMipLevels);
            break;
        }
        case SaveFormat::TGA:
        {
            bool bWithAlpha = (iCompressionIndex == 1);
            pRgtFile->SaveTGA(pStream, bWithAlpha);
            break;
        }
        }
    }
    catch (CRainmanException *pE)
    {
        wxString sMsg = wxT("Failed to save image: ");
        sMsg += wxString::FromUTF8(pE->getMessage());
        pE->destroy();
        m_view.ShowError(sMsg);
        return false;
    }

    return true;
}
