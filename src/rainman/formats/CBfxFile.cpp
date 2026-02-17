/*
Rainman Library
Copyright (C) 2006 Corsix <corsix@gmail.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "rainman/formats/CBfxFile.h"
#include <algorithm>
#include <cstdio>
#include <cstring>

// Forward declaration — defined in CRgdFile.cpp, declared as friend in CRgdFile
bool _SortCMetaTableChildren(CRgdFile::_RgdEntry *p1, CRgdFile::_RgdEntry *p2);

CBfxFile::CBfxFile() { m_bConvertTableIntToTable = false; }

void CBfxFile::SaveAsBfxLua(IFileStore::IOutputStream *pStream, lua_State *Lmap)
{
    RAINMAN_LOG_INFO("CBfxFile::SaveAsBfxLua() — writing BFX Lua data");
    size_t iKeyCount = m_pDataChunk->RootEntry.Data.t->size();
    for (size_t i = 0; i < iKeyCount; ++i)
    {
        _SaveRaw(pStream, m_pDataChunk->RootEntry.Data.t->at(i), Lmap, false, 0);
    }
}

namespace
{
//! Check if any character in the string requires Lua escaping.
bool NeedsLuaEscape(const char *s, size_t iLen)
{
    for (size_t i = 0; i < iLen; ++i)
    {
        if (s[i] == '\\' || s[i] == '\'' || s[i] == '\"')
            return true;
    }
    return false;
}
} // namespace

void CBfxFile::_SaveRaw(IFileStore::IOutputStream *pStream, _RgdEntry *pSource, lua_State *Lmap, bool bNumericID,
                        size_t iIndentLvl)
{
    for (size_t i = 0; i < iIndentLvl; ++i)
        pStream->VWriteString("\t");

    if (!bNumericID && pSource->sName)
    {
        pStream->VWriteString(pSource->sName);
        pStream->VWriteString(" = ");
    }

    char sBuf[64];

    switch (pSource->Type)
    {
    case DT_Float:
    {
        // Use %.5g for compact representation; if result has a decimal point,
        // switch to %.5f for consistent fixed-decimal output.
        std::snprintf(sBuf, sizeof(sBuf), "%.5g", pSource->Data.f);
        if (std::strchr(sBuf, '.'))
            std::snprintf(sBuf, sizeof(sBuf), "%.5f", pSource->Data.f);
        pStream->VWriteString(sBuf);
        pStream->VWriteString(",\r\n");
        break;
    }

    case DT_Integer:
        std::snprintf(sBuf, sizeof(sBuf), "%lu", pSource->Data.i);
        pStream->VWriteString(sBuf);
        pStream->VWriteString(",\r\n");
        break;

    case DT_Bool:
        pStream->VWriteString(pSource->Data.b ? "true,\r\n" : "false,\r\n");
        break;

    case DT_String:
    {
        const char *sToPrint = pSource->Data.s;
        if (Lmap)
        {
            lua_pushstring(Lmap, pSource->Data.s);
            lua_gettable(Lmap, -2);
            sToPrint = lua_tostring(Lmap, -1);
        }

        size_t iL = std::strlen(sToPrint);

        pStream->VWriteString("\"");
        if (NeedsLuaEscape(sToPrint, iL))
        {
            for (size_t i = 0; i < iL; ++i)
            {
                if (sToPrint[i] == '\\' || sToPrint[i] == '\'' || sToPrint[i] == '\"')
                    pStream->VWriteString("\\");
                pStream->VWrite(1, 1, sToPrint + i);
            }
        }
        else
        {
            pStream->VWriteString(sToPrint);
        }
        pStream->VWriteString("\",\r\n");

        if (Lmap)
            lua_pop(Lmap, 1);
        break;
    }

    case DT_WString:
        pStream->VWriteString("nil -- unicode string\r\n");
        break;

    case DT_Table:
    case sk_TableInt:
    {
        pStream->VWriteString(" \r\n");
        for (size_t i = 0; i < iIndentLvl; ++i)
            pStream->VWriteString("\t");
        pStream->VWriteString("{\r\n");

        std::vector<_RgdEntry *> vEntries(pSource->Data.t->begin(), pSource->Data.t->end());

        bool bIsNumericT = true;
        for (const auto *p : vEntries)
        {
            if (p->sName == nullptr)
            {
                bIsNumericT = false;
                break;
            }
            for (const char *sT = p->sName; *sT; ++sT)
            {
                if (*sT < '0' || *sT > '9')
                {
                    bIsNumericT = false;
                    break;
                }
            }
            if (!bIsNumericT)
                break;
        }
        std::sort(vEntries.begin(), vEntries.end(), bIsNumericT ? _SortOutEntriesNum : _SortCMetaTableChildren);

        for (auto *pEntry : vEntries)
        {
            _SaveRaw(pStream, pEntry, Lmap, bIsNumericT, iIndentLvl + 1);
        }
        for (size_t i = 0; i < iIndentLvl; ++i)
            pStream->VWriteString("\t");
        if (iIndentLvl == 0)
            pStream->VWriteString("}\r\n\r\n");
        else
            pStream->VWriteString("},\r\n");
        break;
    }

    case DT_NoData:
        pStream->VWriteString("nil -- no data\r\n");
        break;
    };
}
