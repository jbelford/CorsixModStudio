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

#ifndef _CDMS_FORMAT_SERVICE_H_
#define _CDMS_FORMAT_SERVICE_H_

#include <wx/string.h>
#include "Result.h"
#include <rainman/io/IFileStore.h>

class CRgdFile;
class CChunkyFile;
class CRgtFile;
class CRgmFile;
class CBfxFile;
class CLuaFile;
class CLuaFile2;
class CUcsFile;
struct lua_State;

//! Service adapter wrapping Rainman format parser load/save operations.
/*!
    Catches CRainmanException from parser Load()/Save() calls and
    returns Result<void> for caller-friendly error handling.

    Does NOT own the parser objects or the streams — callers manage those.
*/
class FormatService
{
  public:
    // --- RGD ---
    static Result<void> LoadRgd(CRgdFile *pRgd, IFileStore::IStream *pStream);
    static Result<void> SaveRgd(CRgdFile *pRgd, IFileStore::IOutputStream *pStream);

    // --- Chunky ---
    static Result<void> LoadChunky(CChunkyFile *pChunky, IFileStore::IStream *pStream);
    static Result<void> SaveChunky(CChunkyFile *pChunky, IFileStore::IOutputStream *pStream);

    // --- RGT (texture) ---
    static Result<void> LoadRgt(CRgtFile *pRgt, IFileStore::IStream *pStream);
    static Result<void> SaveRgtAsGeneric(CRgtFile *pRgt, IFileStore::IOutputStream *pStream);
    static Result<void> SaveRgtAsTga(CRgtFile *pRgt, IFileStore::IOutputStream *pStream, bool bIncludeAlpha = true);
    static Result<void> SaveRgtAsDds(CRgtFile *pRgt, IFileStore::IOutputStream *pStream, int iCompression,
                                     bool bMipLevels);

    // --- RGM (material) ---
    static Result<void> LoadRgm(CRgmFile *pRgm, IFileStore::IStream *pStream);
    static Result<void> SaveRgm(CRgmFile *pRgm, IFileStore::IOutputStream *pStream);

    // --- BFX (effects) ---
    static Result<void> LoadBfx(CBfxFile *pBfx, IFileStore::IStream *pStream);
    static Result<void> SaveBfxAsLua(CBfxFile *pBfx, IFileStore::IOutputStream *pStream, lua_State *pLmap = nullptr);

    // --- UCS ---
    static Result<void> LoadUcs(CUcsFile *pUcs, IFileStore::IStream *pStream);
    static Result<void> SaveUcs(CUcsFile *pUcs, const char *sFile);
    static Result<void> SaveUcs(CUcsFile *pUcs, const wxString &sFile);
};

#endif
