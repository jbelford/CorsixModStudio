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

#include "FormatService.h"
#include "common/strconv.h"
#include <rainman/core/Exception.h>
#include <rainman/formats/CRgdFile.h>
#include <rainman/formats/CChunkyFile.h>
#include <rainman/formats/CRgtFile.h>
#include <rainman/formats/CRgmFile.h>
#include <rainman/formats/CBfxFile.h>
#include <rainman/localization/CUcsFile.h>

// --- RGD ---

Result<void> FormatService::LoadRgd(CRgdFile *pRgd, IFileStore::IStream *pStream)
{
    try
    {
        pRgd->Load(pStream);
    }
    catch (CRainmanException *pE)
    {
        return ResultFromException(pE);
    }
    return Result<void>::Ok();
}

Result<void> FormatService::SaveRgd(CRgdFile *pRgd, IFileStore::IOutputStream *pStream)
{
    try
    {
        pRgd->Save(pStream);
    }
    catch (CRainmanException *pE)
    {
        return ResultFromException(pE);
    }
    return Result<void>::Ok();
}

// --- Chunky ---

Result<void> FormatService::LoadChunky(CChunkyFile *pChunky, IFileStore::IStream *pStream)
{
    try
    {
        pChunky->Load(pStream);
    }
    catch (CRainmanException *pE)
    {
        return ResultFromException(pE);
    }
    return Result<void>::Ok();
}

Result<void> FormatService::SaveChunky(CChunkyFile *pChunky, IFileStore::IOutputStream *pStream)
{
    try
    {
        pChunky->Save(pStream);
    }
    catch (CRainmanException *pE)
    {
        return ResultFromException(pE);
    }
    return Result<void>::Ok();
}

// --- RGT ---

Result<void> FormatService::LoadRgt(CRgtFile *pRgt, IFileStore::IStream *pStream)
{
    try
    {
        pRgt->Load(pStream);
    }
    catch (CRainmanException *pE)
    {
        return ResultFromException(pE);
    }
    return Result<void>::Ok();
}

Result<void> FormatService::SaveRgtAsGeneric(CRgtFile *pRgt, IFileStore::IOutputStream *pStream)
{
    try
    {
        pRgt->SaveGeneric(pStream);
    }
    catch (CRainmanException *pE)
    {
        return ResultFromException(pE);
    }
    return Result<void>::Ok();
}

Result<void> FormatService::SaveRgtAsTga(CRgtFile *pRgt, IFileStore::IOutputStream *pStream, bool bIncludeAlpha)
{
    try
    {
        pRgt->SaveTGA(pStream, bIncludeAlpha);
    }
    catch (CRainmanException *pE)
    {
        return ResultFromException(pE);
    }
    return Result<void>::Ok();
}

Result<void> FormatService::SaveRgtAsDds(CRgtFile *pRgt, IFileStore::IOutputStream *pStream, int iCompression,
                                         bool bMipLevels)
{
    try
    {
        pRgt->SaveDDS(pStream, iCompression, bMipLevels);
    }
    catch (CRainmanException *pE)
    {
        return ResultFromException(pE);
    }
    return Result<void>::Ok();
}

// --- RGM ---

Result<void> FormatService::LoadRgm(CRgmFile *pRgm, IFileStore::IStream *pStream)
{
    try
    {
        pRgm->Load(pStream);
    }
    catch (CRainmanException *pE)
    {
        return ResultFromException(pE);
    }
    return Result<void>::Ok();
}

Result<void> FormatService::SaveRgm(CRgmFile *pRgm, IFileStore::IOutputStream *pStream)
{
    try
    {
        pRgm->Save(pStream);
    }
    catch (CRainmanException *pE)
    {
        return ResultFromException(pE);
    }
    return Result<void>::Ok();
}

// --- BFX ---

Result<void> FormatService::LoadBfx(CBfxFile *pBfx, IFileStore::IStream *pStream)
{
    try
    {
        pBfx->Load(pStream);
    }
    catch (CRainmanException *pE)
    {
        return ResultFromException(pE);
    }
    return Result<void>::Ok();
}

Result<void> FormatService::SaveBfxAsLua(CBfxFile *pBfx, IFileStore::IOutputStream *pStream, lua_State *pLmap)
{
    try
    {
        pBfx->SaveAsBfxLua(pStream, pLmap);
    }
    catch (CRainmanException *pE)
    {
        return ResultFromException(pE);
    }
    return Result<void>::Ok();
}

// --- UCS ---

Result<void> FormatService::LoadUcs(CUcsFile *pUcs, IFileStore::IStream *pStream)
{
    try
    {
        pUcs->Load(pStream);
    }
    catch (CRainmanException *pE)
    {
        return ResultFromException(pE);
    }
    return Result<void>::Ok();
}

Result<void> FormatService::SaveUcs(CUcsFile *pUcs, const char *sFile)
{
    try
    {
        pUcs->Save(sFile);
    }
    catch (CRainmanException *pE)
    {
        return ResultFromException(pE);
    }
    return Result<void>::Ok();
}

Result<void> FormatService::SaveUcs(CUcsFile *pUcs, const wxString &sFile)
{
    auto s = wxStringToAscii(sFile);
    if (!s)
        return Result<void>::Err(wxT("Memory allocation error"));

    auto result = SaveUcs(pUcs, s.get());
    return result;
}
