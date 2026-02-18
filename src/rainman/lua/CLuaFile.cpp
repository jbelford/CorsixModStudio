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

#include "rainman/lua/CLuaFile.h"

#ifdef _MSC_VER
#include <intrin.h>
#endif

extern "C"
{
#include <lauxlib.h>
#include <lualib.h>
};
#include "rainman/lua/luax.h"
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include "rainman/module/CModuleFile.h"
#include "rainman/module/CFileMap.h"
#include "rainman/core/Exception.h"
#include <memory>
#include "rainman/core/memdebug.h"

extern "C"
{
    using ub4 = unsigned long int; /* unsigned 4-byte quantities */
    using ub1 = unsigned char;
    ub4 hash(ub1 *k, ub4 length, ub4 initval);
    ub4 hash3(ub1 *k, ub4 length, ub4 initval);
}

struct tLuaTableProtector
{
    void Init(lua_State *L, int iT, bool bOwn, tLuaTableProtector *parent = nullptr)
    {
        iMagic = 0x7291BEEF;
        lua_pushvalue(L, iT);
        lua_pushlightuserdata(L, (void *)this);
        lua_insert(L, -2);
        lua_settable(L, LUA_REGISTRYINDEX);
        iOwnIt = bOwn ? 1 : 0;
        pParent = parent;

        lua_newtable(L);
        lua_pushstring(L, "__newindex");    // push 1 : +1
        lua_pushcclosure(L, OnNewIndex, 0); // pop 2, push 1 : -1
        lua_settable(L, -3);                // pop 2 : -2
        lua_pushstring(L, "__index");       // push 1 : +1
        lua_pushcclosure(L, OnIndex, 0);    // pop 2, push 1 : -1
        lua_settable(L, -3);                // pop 2 : -2
        lua_setmetatable(L, -2);            // pop 1: -1
    }

    // o[k]
    static int OnIndex(lua_State *L)
    {
        auto *pThis = (tLuaTableProtector *)lua_touserdata(L, -2);
        if (pThis->iOwnIt)
        {
            lua_pushlightuserdata(L, (void *)pThis);
            lua_gettable(L, LUA_REGISTRYINDEX);
            lua_insert(L, -2);
            lua_gettable(L, -2);
        }
        else
        {
            lua_pushlightuserdata(L, (void *)pThis);
            lua_gettable(L, LUA_REGISTRYINDEX);
            SimpleTableDuplicate(L);
            lua_remove(L, -2);
            lua_replace(L, -3);
            lua_gettable(L, -2);
            lua_pushlightuserdata(L, (void *)pThis);
            lua_pushvalue(L, -3);
            lua_settable(L, LUA_REGISTRYINDEX);
            pThis->iOwnIt = 1;
        }
        lua_remove(L, -2);
        return 1;
    }

    // t
    static void SimpleTableDuplicate(lua_State *L)
    {
        lua_newtable(L);

        lua_pushnil(L);
        while (lua_next(L, -3) != 0)
        {
            lua_pushvalue(L, -2);
            if (lua_type(L, -2) == LUA_TTABLE)
            {
                auto *pWrap = (tLuaTableProtector *)lua_newuserdata(L, sizeof(tLuaTableProtector));
                pWrap->Init(L, -3, false);
            }
            else if (lua_type(L, -2) == LUA_TUSERDATA)
            {
                auto *pExisting = (tLuaTableProtector *)lua_touserdata(L, -2);
                if (pExisting->iMagic == 0x7291BEEF)
                {
                    lua_pushlightuserdata(L, (void *)pExisting);
                    lua_gettable(L, LUA_REGISTRYINDEX); // swap {w -1} with {t}
                    auto *pWrap = (tLuaTableProtector *)lua_newuserdata(L, sizeof(tLuaTableProtector));
                    pWrap->Init(L, -2, false, nullptr);
                    lua_remove(L, -2); // remove {v} so that {w -1}
                }
                else
                {
                    lua_pushvalue(L, -2);
                }
            }
            else
                lua_pushvalue(L, -2);
            lua_settable(L, -5);
            lua_pop(L, 1); // pop value
        }

        lua_newtable(L);
        lua_pushstring(L, "__newindex");               // push 1 : +1
        lua_pushboolean(L, 1);                         // push 1 : +1
        lua_pushcclosure(L, CLuaFile::_meta_index, 1); // pop 2, push 1 : -1
        lua_settable(L, -3);                           // pop 2 : -2
        lua_pushstring(L, "__updateindex");            // push 1 : +1
        lua_pushboolean(L, 0);                         // push 1 : +1
        lua_pushcclosure(L, CLuaFile::_meta_index, 1); // pop 2, push 1 : -1
        lua_settable(L, -3);                           // pop 2 : -2
        lua_setmetatable(L, -2);
    }

    // o[k] = v
    static int OnNewIndex(lua_State *L)
    {
        auto *pThis = (tLuaTableProtector *)lua_touserdata(L, -3);
        if (pThis->iOwnIt)
        {
            lua_pushlightuserdata(L, (void *)pThis);
            lua_gettable(L, LUA_REGISTRYINDEX);
            lua_insert(L, -3);
            lua_settable(L, -3);
        }
        else
        {
            lua_pushlightuserdata(L, (void *)pThis);
            lua_gettable(L, LUA_REGISTRYINDEX);
            SimpleTableDuplicate(L);
            lua_remove(L, -2);
            lua_replace(L, -4);
            lua_settable(L, -3);
            lua_pushlightuserdata(L, (void *)pThis);
            lua_insert(L, -2);
            lua_settable(L, LUA_REGISTRYINDEX);
            pThis->iOwnIt = 1;
        }
        return 0;
    }

    unsigned long iMagic;
    tLuaTableProtector *pParent;
    unsigned char iOwnIt;
};

CLuaFile::CLuaFile()
{
    m_pLua = nullptr;
    m_pRefQueue = nullptr;
    m_pCache = nullptr;
    m_bOwnCache = true;
    _Clean();
}

CLuaFile::~CLuaFile() { _Clean(); }

void CLuaFile::New()
{
    _Clean();
    if (!m_pCache)
    {
        m_pCache = new CLuaFileCache;
        m_bOwnCache = true;
    }

    m_pLua = m_pCache->MakeState();
    if (!m_pLua)
    {
        m_sLuaError = "Unable to create lua state";
        throw CRainmanException(__FILE__, __LINE__, "Unable to create lua state");
    }
    luaopen_base(m_pLua);
    luaopen_string(m_pLua);
    luaopen_math(m_pLua);

    luax_DeleteGlobal(m_pLua, "coroutine");
    luax_DeleteGlobal(m_pLua, "dofile");
    luax_DeleteGlobal(m_pLua, "getfenv");
    luax_DeleteGlobal(m_pLua, "setfenv");
    luax_DeleteGlobal(m_pLua, "getmetatable");
    luax_DeleteGlobal(m_pLua, "setmetatable");
    luax_DeleteGlobal(m_pLua, "loadlib");
    luax_DeleteGlobal(m_pLua, "loadfile");
    luax_DeleteGlobal(m_pLua, "print");
    luax_DeleteGlobal(m_pLua, "rawget");
    luax_DeleteGlobal(m_pLua, "rawset");
    luax_DeleteGlobal(m_pLua, "require");
}

/*
const char* CLuaFile::eValueToString(eValues e)
{
    switch(e)
    {
        // Errors
    case E_NoStream:
        return "invalid argument";

    case E_MemoryAllocateError:
        return "memory error";

    case E_ReadError:
        return "i/o error";

    case E_NoData:
        return "no GameData";

    case E_SyntaxError:
        return "syntax error";

    case E_LuaApiError:
        return "api error";

    case E_CircularReference:
        return "circular reference";

    case E_LuaRuntimeError:
        return "runtime error";

    case E_LuaProcessingError:
        return "processing error";

    case E_OK:
        return "ok";

    case E_UnknownError:
        return "error";
    }
    return "";
}*/

int CLuaFile::_Inherit(lua_State *L)
{
    if (!lua_isstring(L, -1))
    {
        lua_pushlightuserdata(L, new CRainmanException(__FILE__, __LINE__, "invalid argument to `_Inherit'"));
        lua_error(L);
    }

    auto *pFiles = (IFileStore *)lua_touserdata(L, lua_upvalueindex(1));
    lua_pushstring(L, "m_pCache");
    lua_gettable(L, LUA_REGISTRYINDEX);
    auto *pCache = (CLuaFileCache *)lua_touserdata(L, -1);
    lua_pop(L, 1);

    const char *sFile = lua_tostring(L, -1);
    CLuaFile *pLua;

    if (sFile[0] == 0)
    {
        lua_newtable(L);
        return 1;
    }

    try
    {
        pCache->GameDataToStack(sFile, L);
        tLuaTableProtector::SimpleTableDuplicate(L);
        lua_remove(L, -2);
        lua_pushstring(L, "$dow_mod_studio");
        lua_pushvalue(L, -1);
        luax_GetRawOrMakeTable(L, -3);
        lua_pushstring(L, "ref_type");
        lua_pushstring(L, "Reference");
        lua_settable(L, -3);

        lua_pushstring(L, "ref_name");
        lua_pushstring(L, sFile);
        lua_settable(L, -3);
        lua_rawset(L, -3);
        return 1;
    }
    IGNORE_EXCEPTIONS

    lua_pushstring(L, "m_pRefQueue");
    lua_gettable(L, LUA_REGISTRYINDEX);
    auto *pRefQueue = (std::deque<const char *> *)lua_touserdata(L, -1);
    lua_pop(L, 1);

    char *sFullPathFile = new char[strlen(sFile) + 32];
    if (!sFullPathFile)
    {
        lua_pushlightuserdata(L, new CRainmanException(__FILE__, __LINE__, "memory allocation error in `_Inherit'"));
        lua_error(L);
    }
    strcpy(sFullPathFile, "attrib\\attrib\\");
    strcat(sFullPathFile, sFile);
    std::unique_ptr<IFileStore::IStream> pStream;
    try
    {
        pStream = std::unique_ptr<IFileStore::IStream>(pFiles->VOpenStream(sFullPathFile));
    }
    catch (const CRainmanException &e)
    {
        strcpy(sFullPathFile, "data\\attrib\\");
        strcat(sFullPathFile, sFile);

        try
        {
            pStream = std::unique_ptr<IFileStore::IStream>(pFiles->VOpenStream(sFullPathFile));
        }
        catch (const CRainmanException &e)
        {
            delete[] sFullPathFile;
            sFullPathFile = nullptr;
            lua_pushlightuserdata(
                L, new CRainmanException(e, __FILE__, __LINE__, "unable to open file `%s' in `_Inherit'", sFile));
            lua_error(L);
        }
    }

    delete[] sFullPathFile;

    pLua = new CLuaFile;
    if (!pLua)
    {
        lua_pushlightuserdata(L, new CRainmanException(__FILE__, __LINE__, "memory allocation error in `_Inherit'"));
        lua_error(L);
    }
    pLua->m_pRefQueue = pRefQueue;
    pLua->m_pCache = pCache;
    pLua->m_bOwnCache = false;

    try
    {
        pLua->Load(pStream.get(), pFiles, sFile);
    }
    catch (const CRainmanException &e)
    {
        lua_pushlightuserdata(
            L, new CRainmanException(e, __FILE__, __LINE__, "error loading file `%s' in `_Inherit'", sFile));
        delete pLua;
        lua_error(L);
    }

    try
    {
        pLua->_CacheLua(sFile);
    }
    catch (const CRainmanException &e)
    {
        lua_pushlightuserdata(
            L, new CRainmanException(e, __FILE__, __LINE__, "error caching file `%s' in `_Inherit'", sFile));
        delete pLua;
        lua_error(L);
    }

    lua_State *pLuaLoadedState = pLua->m_pLua;

    try
    {
        luax_GetGameData(pLua->m_pLua);
    }
    catch (const CRainmanException &e)
    {
        delete pLua;
        lua_pushlightuserdata(L,
                              new CRainmanException(e, __FILE__, __LINE__, "no GameData in `%s' in `_Inherit'", sFile));
        lua_error(L);
    }

    lua_xmove(pLua->m_pLua, L, 1);
    tLuaTableProtector::SimpleTableDuplicate(L);
    lua_remove(L, -2);

    // My metadata
    lua_pushstring(L, "$dow_mod_studio");
    lua_pushvalue(L, -1);
    luax_GetRawOrMakeTable(L, -3);
    lua_pushstring(L, "ref_type");
    lua_pushstring(L, "Reference");
    lua_settable(L, -3);

    lua_pushstring(L, "ref_name");
    lua_pushstring(L, sFile);
    lua_settable(L, -3);
    lua_rawset(L, -3);
    delete pLua;
    return 1;
}

int CLuaFile::_InheritMeta(lua_State *L)
{
    if (!lua_isstring(L, -1))
    {
        lua_pushlightuserdata(L, new CRainmanException(__FILE__, __LINE__, "invalid argument to `_InheritMeta'"));
        lua_error(L);
    }

    auto *pFiles = (IFileStore *)lua_touserdata(L, lua_upvalueindex(1));
    lua_pushstring(L, "m_pCache");
    lua_gettable(L, LUA_REGISTRYINDEX);
    auto *pCache = (CLuaFileCache *)lua_touserdata(L, -1);
    lua_pop(L, 1);
    CLuaFile *pLua;
    const char *sFile = lua_tostring(L, -1);

    if (sFile[0] == 0)
    {
        lua_newtable(L);
        return 1;
    }
    try
    {
        pCache->MetaDataToStack(sFile, L);
        tLuaTableProtector::SimpleTableDuplicate(L);
        lua_remove(L, -2);
        lua_pushstring(L, "$dow_mod_studio");
        lua_pushvalue(L, -1);
        luax_GetRawOrMakeTable(L, -3);
        lua_pushstring(L, "ref_type");
        lua_pushstring(L, "Reference");
        lua_settable(L, -3);

        lua_pushstring(L, "ref_name");
        lua_pushstring(L, sFile);
        lua_settable(L, -3);
        lua_rawset(L, -3);
        return 1;
    }
    IGNORE_EXCEPTIONS

    lua_pushstring(L, "m_pRefQueue");
    lua_gettable(L, LUA_REGISTRYINDEX);
    auto *pRefQueue = (std::deque<const char *> *)lua_touserdata(L, -1);
    lua_pop(L, 1);

    char *sFullPathFile = new char[strlen(sFile) + 32];
    if (!sFullPathFile)
    {
        lua_pushlightuserdata(L,
                              new CRainmanException(__FILE__, __LINE__, "memory allocation error in `_InheritMeta'"));
        lua_error(L);
    }
    strcpy(sFullPathFile, "attrib\\attrib\\");
    strcat(sFullPathFile, sFile);
    std::unique_ptr<IFileStore::IStream> pStream;
    try
    {
        pStream = std::unique_ptr<IFileStore::IStream>(pFiles->VOpenStream(sFullPathFile));
    }
    catch (const CRainmanException &e)
    {
        strcpy(sFullPathFile, "data\\attrib\\");
        strcat(sFullPathFile, sFile);

        try
        {
            pStream = std::unique_ptr<IFileStore::IStream>(pFiles->VOpenStream(sFullPathFile));
        }
        catch (const CRainmanException &e)
        {
            delete[] sFullPathFile;
            sFullPathFile = nullptr;
            lua_pushlightuserdata(
                L, new CRainmanException(e, __FILE__, __LINE__, "unable to open file `%s' in `_InheritMeta'", sFile));
            lua_error(L);
        }
    }

    delete[] sFullPathFile;

    pLua = new CLuaFile;
    if (!pLua)
    {
        lua_pushlightuserdata(L,
                              new CRainmanException(__FILE__, __LINE__, "memory allocation error in `_InheritMeta'"));
        lua_error(L);
    }
    pLua->m_pRefQueue = pRefQueue;
    pLua->m_pCache = pCache;
    pLua->m_bOwnCache = false;

    try
    {
        pLua->Load(pStream.get(), pFiles, sFile);
    }
    catch (const CRainmanException &e)
    {
        lua_pushlightuserdata(
            L, new CRainmanException(e, __FILE__, __LINE__, "error loading file \'%s\' in `_InheritMeta'", sFile));
        delete pLua;
        lua_error(L);
    }

    try
    {
        pLua->_CacheLua(sFile);
    }
    catch (const CRainmanException &e)
    {
        lua_pushlightuserdata(
            L, new CRainmanException(e, __FILE__, __LINE__, "error caching file \'%s\' in `_InheritMeta'", sFile));
        delete pLua;
        lua_error(L);
    }

    lua_State *pLuaLoadedState = pLua->m_pLua;

    try
    {
        luax_GetMetaData(pLua->m_pLua);
    }
    catch (const CRainmanException &e)
    {
        delete pLua;
        lua_pushlightuserdata(L, new CRainmanException(e, __FILE__, __LINE__,
                                                       "no MetaData found in file \'%s\' in `_InheritMeta'", sFile));
        lua_error(L);
#ifndef RAINMAN_GNUC
        __nop();
#endif
    }

    lua_xmove(pLua->m_pLua, L, 1);
    tLuaTableProtector::SimpleTableDuplicate(L);
    lua_remove(L, -2);

    // My metadata
    lua_pushstring(L, "$dow_mod_studio");
    lua_pushvalue(L, -1);
    luax_GetRawOrMakeTable(L, -3);
    lua_pushstring(L, "ref_type");
    lua_pushstring(L, "Reference");
    lua_settable(L, -3);

    lua_pushstring(L, "ref_name");
    lua_pushstring(L, sFile);
    lua_settable(L, -3);
    lua_rawset(L, -3);
    delete pLua;
    return 1;
}

int CLuaFile::_Reference_Grabby(lua_State *L)
{
    lua_pushlightuserdata(L, (void *)strdup(lua_tostring(L, -1)));
    lua_error(L);
    return 0; // never happens
}

int CLuaFile::_Reference(lua_State *L)
{
    if (!lua_isstring(L, -1))
    {
        lua_pushlightuserdata(L, new CRainmanException(__FILE__, __LINE__, "invalid argument to `_Reference'"));
        lua_error(L);
    }

    auto *pFiles = (IFileStore *)lua_touserdata(L, lua_upvalueindex(1));
    lua_pushstring(L, "m_pCache");
    lua_gettable(L, LUA_REGISTRYINDEX);
    auto *pCache = (CLuaFileCache *)lua_touserdata(L, -1);
    lua_pop(L, 1);
    CLuaFile *pLua;
    const char *sFile = lua_tostring(L, -1);

    if (sFile[0] == 0)
    {
        lua_newtable(L);
        return 1;
    }
    try
    {
        pCache->GameDataToStack(sFile, L);
        tLuaTableProtector::SimpleTableDuplicate(L);
        lua_remove(L, -2);
        lua_pushstring(L, "$dow_mod_studio");
        lua_pushvalue(L, -1);
        luax_GetRawOrMakeTable(L, -3);
        lua_pushstring(L, "ref_type");
        lua_pushstring(L, "Reference");
        lua_settable(L, -3);

        lua_pushstring(L, "ref_name");
        lua_pushstring(L, sFile);
        lua_settable(L, -3);
        lua_rawset(L, -3);
        return 1;
    }
    IGNORE_EXCEPTIONS

    lua_pushstring(L, "m_pRefQueue");
    lua_gettable(L, LUA_REGISTRYINDEX);
    auto *pRefQueue = (std::deque<const char *> *)lua_touserdata(L, -1);
    lua_pop(L, 1);

    char *sFullPathFile = new char[strlen(sFile) + 32];
    if (!sFullPathFile)
    {
        lua_pushlightuserdata(L, new CRainmanException(__FILE__, __LINE__, "memory allocation error in `_Reference'"));
        lua_error(L);
    }
    strcpy(sFullPathFile, "attrib\\attrib\\");
    strcat(sFullPathFile, sFile);
    std::unique_ptr<IFileStore::IStream> pStream;
    try
    {
        pStream = std::unique_ptr<IFileStore::IStream>(pFiles->VOpenStream(sFullPathFile));
    }
    catch (const CRainmanException &e)
    {
        strcpy(sFullPathFile, "data\\attrib\\");
        strcat(sFullPathFile, sFile);

        try
        {
            pStream = std::unique_ptr<IFileStore::IStream>(pFiles->VOpenStream(sFullPathFile));
        }
        catch (const CRainmanException &e)
        {
            delete[] sFullPathFile;
            sFullPathFile = nullptr;
            lua_pushlightuserdata(
                L, new CRainmanException(e, __FILE__, __LINE__, "unable to open file `%s' in `_Reference'", sFile));
            lua_error(L);
        }
    }

    delete[] sFullPathFile;

    pLua = new CLuaFile;
    if (!pLua)
    {
        lua_pushlightuserdata(L, new CRainmanException(__FILE__, __LINE__, "memory allocation error in `_Reference'"));
        lua_error(L);
    }
    pLua->m_pRefQueue = pRefQueue;
    pLua->m_pCache = pCache;
    pLua->m_bOwnCache = false;

    try
    {
        pLua->Load(pStream.get(), pFiles, sFile);
    }
    catch (const CRainmanException &e)
    {
        lua_pushlightuserdata(
            L, new CRainmanException(e, __FILE__, __LINE__, "error loading file `%s' in `_Reference'", sFile));
        delete pLua;
        lua_error(L);
    }

    try
    {
        pLua->_CacheLua(sFile);
    }
    catch (const CRainmanException &e)
    {
        lua_pushlightuserdata(
            L, new CRainmanException(e, __FILE__, __LINE__, "error caching file `%s' in `_Reference'", sFile));
        delete pLua;
        lua_error(L);
    }

    lua_State *pLuaLoadedState = pLua->m_pLua;

    try
    {
        luax_GetGameData(pLua->m_pLua);
    }
    catch (const CRainmanException &e)
    {
        delete pLua;
        lua_pushlightuserdata(
            L, new CRainmanException(e, __FILE__, __LINE__, "no GameData in file `%s' to `_Reference'", sFile));
        lua_error(L);
    }

    lua_xmove(pLua->m_pLua, L, 1);
    tLuaTableProtector::SimpleTableDuplicate(L);
    lua_remove(L, -2);

    // My metadata
    lua_pushstring(L, "$dow_mod_studio");
    lua_pushvalue(L, -1);
    luax_GetRawOrMakeTable(L, -3);
    lua_pushstring(L, "ref_type");
    lua_pushstring(L, "Reference");
    lua_settable(L, -3);

    lua_pushstring(L, "ref_name");
    lua_pushstring(L, sFile);
    lua_settable(L, -3);
    lua_rawset(L, -3);
    delete pLua;
    return 1;
}

int CLuaFile::_Debug(lua_State *L)
{
    const char *s = lua_tostring(L, -1);
    luax_PrintGlobals(L, s);
    return 0;
}

// table[key] = value
int CLuaFile::_meta_index(lua_State *L)
{
    lua_pushstring(L, "m_pRefQueue");
    lua_gettable(L, LUA_REGISTRYINDEX);
    auto *pRefQueue = (std::deque<const char *> *)lua_touserdata(L, -1);
    lua_pop(L, 1);
    // Get $dow_mod_studio
    lua_pushstring(L, "$dow_mod_studio"); // push 1: +1
    luax_GetOrMakeTable(L, -4);           // pop 1, push 1: 0
    // Get $dow_mod_studio.Set
    lua_pushstring(L, "Set");   // push 1: +1
    luax_GetOrMakeTable(L, -2); // pop 1, push 1: 0
    // Set $dow_mod_studio.Set.key
    lua_pushvalue(L, -4);                      // push 1: +1
    lua_pushstring(L, (*pRefQueue->rbegin())); // push 1: +1
    lua_settable(L, -3);                       // pop 2: -2
    lua_pop(L, 2);

    // Hook child
    if (lua_istable(L, -1))
    {
        if (lua_getmetatable(L, -1) == 0) // push 1 : +1
        {
            lua_newtable(L);
            lua_setmetatable(L, -2);
            lua_getmetatable(L, -1);
        }
        lua_pushstring(L, "__newindex");     // push 1 : +1
        lua_pushboolean(L, 1);               // push 1 : +1
        lua_pushcclosure(L, _meta_index, 1); // pop 2, push 1 : -1
        lua_settable(L, -3);                 // pop 2 : -2
        lua_pushstring(L, "__updateindex");  // push 1 : +1
        lua_pushboolean(L, 0);               // push 1 : +1
        lua_pushcclosure(L, _meta_index, 1); // pop 2, push 1 : -1
        lua_settable(L, -3);                 // pop 2 : -2
        lua_pop(L, 1);
    }

    // Do actual set
    if (lua_isstring(L, -2) && (strcmp(lua_tostring(L, -2), "$dow_mod_studio") == 0))
        lua_pop(L, 2);
    else
        lua_rawset(L, -3);
    return 0;
}

void CLuaFile::_PopMyMetaFromNodeList()
{
    for (auto itr = m_lstGlobals.begin(); itr != m_lstGlobals.end(); ++itr)
    {
        if (((*itr)->eNameType == CLuaFile::_NodeLocator::NT_String) && ((*itr)->sName == "$dow_mod_studio"))
        {
            m_pDowModStudio = std::move(*itr);
            m_lstGlobals.erase(itr);
            break;
        }
    }
}

void CLuaFile::AssignCache(CLuaFileCache *pCache)
{
    m_pCache = pCache;
    m_bOwnCache = false;
}

char *CLuaFile::GetReferencedFile(IFileStore::IStream *pStream)
{
    _Clean();
    if (!pStream)
        throw CRainmanException(__FILE__, __LINE__, "No input stream");

    try
    {
        New();
    }
    catch (const CRainmanException &e)
    {
        throw CRainmanException(e, __FILE__, __LINE__, "New() failed");
    }

    long iDataLength;
    try
    {
        pStream->VSeek(0, IFileStore::IStream::SL_End);
        iDataLength = pStream->VTell();
    }
    catch (const CRainmanException &e)
    {
        _Clean();
        throw CRainmanException(e, __FILE__, __LINE__, "Seek/Tell failed");
    }
    auto sBuffer = std::make_unique<char[]>(iDataLength);

    try
    {
        pStream->VSeek(0, IFileStore::IStream::SL_Root);
        pStream->VRead(1, (unsigned long)iDataLength, sBuffer.get());
    }
    catch (const CRainmanException &e)
    {
        _Clean();
        throw CRainmanException(e, __FILE__, __LINE__, "Seek/Read failed");
    }

    // Register functions
    lua_pushstring(m_pLua, "Inherit");
    lua_pushcclosure(m_pLua, _Reference_Grabby, 0);
    lua_settable(m_pLua, LUA_GLOBALSINDEX);

    int iLuaError = luaL_loadbuffer(m_pLua, sBuffer.get(), iDataLength, "input stream");
    sBuffer.reset();
    iLuaError = iLuaError || lua_pcall(m_pLua, 0, 0, 0);

    if (iLuaError)
    {
        switch (lua_type(m_pLua, -1))
        {
        case LUA_TSTRING:
        {
            m_sLuaError = lua_tostring(m_pLua, -1);
            auto sLuaError = m_sLuaError;
            _Clean();
            throw CRainmanException(nullptr, __FILE__, __LINE__, "Lua error (%i): %s", iLuaError, sLuaError.c_str());
        }
        case LUA_TLIGHTUSERDATA:
        {
            char *sLuaError = (char *)lua_touserdata(m_pLua, -1);
            if (sLuaError == nullptr)
                throw CRainmanException(__FILE__, __LINE__, "Unable to attain Reference");
            _Clean();
            return sLuaError;
        }
        default:
        {
            m_sLuaError = "Unknown error";
            _Clean();
            throw CRainmanException(nullptr, __FILE__, __LINE__, "Lua unknown error (%i)", iLuaError);
        }
        };
    }
    return strdup("");
}

#define quick_fn_register(n, f)                                                                                        \
    lua_pushstring(m_pLua, n), lua_pushlightuserdata(m_pLua, (void *)pFiles), lua_pushcclosure(m_pLua, f, 1),          \
        lua_settable(m_pLua, LUA_GLOBALSINDEX);
void CLuaFile::Load(IFileStore::IStream *pStream, IFileStore *pFiles, const char *sFileName)
{
    RAINMAN_LOG_INFO("CLuaFile::Load() — parsing Lua file \"{}\"", sFileName ? sFileName : "(null)");
    _Clean();
    if (!pStream)
        throw CRainmanException(__FILE__, __LINE__, "No input stream");

    try
    {
        New();
    }
    catch (const CRainmanException &e)
    {
        throw CRainmanException(e, __FILE__, __LINE__, "New() failed");
    }

    long iDataLength;
    try
    {
        pStream->VSeek(0, IFileStore::IStream::SL_End);
        iDataLength = pStream->VTell();
    }
    catch (const CRainmanException &e)
    {
        _Clean();
        throw CRainmanException(e, __FILE__, __LINE__, "Seek/Tell failed");
    }
    auto sBuffer = std::make_unique<char[]>(iDataLength);

    try
    {
        pStream->VSeek(0, IFileStore::IStream::SL_Root);
        pStream->VRead(1, (unsigned long)iDataLength, sBuffer.get());
    }
    catch (const CRainmanException &e)
    {
        _Clean();
        throw CRainmanException(e, __FILE__, __LINE__, "Seek/Read failed");
    }

    // Register functions
    bool bOwnRefQueue = false;
    if (!m_pRefQueue)
    {
        m_pRefQueue = new std::deque<const char *>;
        bOwnRefQueue = true;

        if (strnicmp("generic", sFileName, 7) == 0)
        {
            auto *pMod = (CModuleFile *)pFiles;
            pMod->GetFileMap()->RewriteToC("attrib", "generic");
        }
    }

    quick_fn_register("Inherit", _Inherit);
    quick_fn_register("InheritMeta", _InheritMeta);
    quick_fn_register("Reference", _Reference);
    quick_fn_register("DebugThis", _Debug);

    lua_pushstring(m_pLua, "m_pCache");
    lua_pushlightuserdata(m_pLua, (void *)this->m_pCache); // push 1 : +1
    lua_settable(m_pLua, LUA_REGISTRYINDEX);
    lua_pushstring(m_pLua, "m_pRefQueue");
    lua_pushlightuserdata(m_pLua, (void *)this->m_pRefQueue); // push 1 : +1
    lua_settable(m_pLua, LUA_REGISTRYINDEX);

    if (lua_getmetatable(m_pLua, LUA_GLOBALSINDEX) == 0) // push 1 : +1
    {
        lua_newtable(m_pLua);
        lua_setmetatable(m_pLua, LUA_GLOBALSINDEX);
        lua_getmetatable(m_pLua, LUA_GLOBALSINDEX);
    }
    lua_pushstring(m_pLua, "__newindex");       // push 1 : +1
    lua_pushboolean(m_pLua, 1);                 // push 1 : +1
    lua_pushcclosure(m_pLua, _meta_index, 1);   // pop 2, push 1 : -1
    lua_settable(m_pLua, -3);                   // pop 2 : -2
    lua_pushstring(m_pLua, "__updateindex");    // push 1 : +1
    lua_pushboolean(m_pLua, 0);                 // push 1 : +1
    lua_pushcclosure(m_pLua, _meta_index, 1);   // pop 2, push 1 : -1
    lua_settable(m_pLua, -3);                   // pop 2 : -2
    lua_setmetatable(m_pLua, LUA_GLOBALSINDEX); // pop 1: -1

    sFileName = sFileName ? sFileName : "lua file";
    for (auto itr = m_pRefQueue->begin(); itr != m_pRefQueue->end(); ++itr)
    {
        if (stricmp(*itr, sFileName) == 0)
        {
            if (bOwnRefQueue)
                delete m_pRefQueue;
            _Clean();
            throw CRainmanException(nullptr, __FILE__, __LINE__, "Circular reference detected (%s)", sFileName);
        }
    }
    m_pRefQueue->push_back(sFileName);

    int iLuaError = luaL_loadbuffer(m_pLua, sBuffer.get(), iDataLength, sFileName);
    sBuffer.reset();
    iLuaError = iLuaError || lua_pcall(m_pLua, 0, 0, 0);

    m_pRefQueue->pop_back();
    if (bOwnRefQueue)
    {
        if (strnicmp("generic", sFileName, 7) == 0)
        {
            auto *pMod = (CModuleFile *)pFiles;
            pMod->GetFileMap()->UnrewriteToC("attrib");
        }
        delete m_pRefQueue;
    }

    if (iLuaError)
    {
        switch (lua_type(m_pLua, -1))
        {
        case LUA_TSTRING:
        {
            m_sLuaError = lua_tostring(m_pLua, -1);
            auto sLuaError = m_sLuaError;
            _Clean();
            throw CRainmanException(nullptr, __FILE__, __LINE__, "Lua error (%i): %s", iLuaError, sLuaError.c_str());
        }
        case LUA_TLIGHTUSERDATA:
        {
            auto *e = (CRainmanException *)lua_touserdata(m_pLua, -1);
            m_sLuaError = e->getMessage();
            _Clean();
            throw CRainmanException(*e, __FILE__, __LINE__, "Lua error (%i)", iLuaError);
        }
        default:
        {
            m_sLuaError = "Unknown error";
            _Clean();
            throw CRainmanException(nullptr, __FILE__, __LINE__, "Lua unknown error (%i)", iLuaError);
        }
        };
    }

    // Globals
    lua_pushstring(m_pLua, "_G");
    lua_gettable(m_pLua, LUA_GLOBALSINDEX);
    try
    {
        _TableToNodeList(m_lstGlobals, m_pLua, nullptr, true);
    }
    catch (const CRainmanException &e)
    {
        _Clean();
        throw CRainmanException(e, __FILE__, __LINE__, "Error processing \'%s\'", sFileName);
    }
    lua_pop(m_pLua, 1);
    _PopMyMetaFromNodeList();

    // End
}

#undef quick_fn_register

const char *CLuaFile::GetLuaError() { return m_sLuaError.empty() ? nullptr : m_sLuaError.c_str(); }

void CLuaFile::_Clean()
{
    if (m_pCache)
    {
        // m_pCache->FreeState(m_pLua);
        if (m_bOwnCache) // Yah, unowned caches are persistant through death
        {
            delete m_pCache;
            m_pCache = nullptr;
        }
    }

    m_pLua = nullptr;

    m_sLuaError.clear();

    m_lstGlobals.clear();
    m_pDowModStudio.reset();
}

void CLuaFile::_CacheLua(const char *sName)
{
    if (!m_pLua)
        throw CRainmanException(__FILE__, __LINE__, "No m_pLua");
    if (!m_pCache)
        throw CRainmanException(__FILE__, __LINE__, "No m_pCache");
    try
    {
        m_pCache->AddToCache(sName, m_pLua);
    }
    catch (const CRainmanException &e)
    {
        throw CRainmanException(e, __FILE__, __LINE__, "Could not cache \'%s\'", sName);
    }
}

// MetaTable
unsigned long CLuaFile::VGetChildCount() { return static_cast<unsigned long>(m_lstGlobals.size()); }

IMetaNode *CLuaFile::VGetChild(unsigned long iIndex)
{
    if (iIndex >= VGetChildCount())
        throw CRainmanException(nullptr, __FILE__, __LINE__, "Index %lu is beyong %lu", iIndex, VGetChildCount());
    return new CLuaFile::CMetaNode(m_lstGlobals[iIndex].get(), m_pLua);
}

IMetaNode::eDataTypes CLuaFile::VGetReferenceType() { return IMetaNode::DT_NoData; }

const char *CLuaFile::VGetReferenceString()
{
    throw CRainmanException(__FILE__, __LINE__, "_G cannot have a reference");
}

const wchar_t *CLuaFile::VGetReferenceWString()
{
    throw CRainmanException(__FILE__, __LINE__, "_G cannot have a unicode reference");
}

void CLuaFile::VSetReferenceType(IMetaNode::eDataTypes eType)
{
    if (eType != IMetaNode::DT_NoData)
        throw CRainmanException(__FILE__, __LINE__, "_G cannot have a reference assigned");
}

void CLuaFile::VSetReferenceString(const char *sValue)
{
    throw CRainmanException(__FILE__, __LINE__, "_G cannot have a reference assigned");
}

void CLuaFile::VSetReferenceWString(const wchar_t *wsValue)
{
    throw CRainmanException(__FILE__, __LINE__, "_G cannot have a reference assigned");
}

IMetaNode *CLuaFile::VAddChild(const char *sName)
{
    throw CRainmanException(__FILE__, __LINE__, "Lua files cannot be edited yet :/");
}

void CLuaFile::VDeleteChild(unsigned long iIndex)
{
    throw CRainmanException(__FILE__, __LINE__, "Lua files cannot be edited yet :/");
}

CLuaFile::_NodeLocator::_NodeLocator(const CLuaFile::_NodeLocator &Other)
    : pParent(Other.pParent), sName(Other.sName), fName(Other.fName), bName(Other.bName), eNameType(Other.eNameType)
{
}

CLuaFile::_NodeLocator::_NodeLocator() : pParent(nullptr), eNameType(NT_String) {}
CLuaFile::_NodeLocator::_NodeLocator(lua_State *L, int iIndex,
                                     const _NodeLocator *pParent) // init from key in stack + parent
{
    this->pParent = pParent;
    switch (lua_type(L, iIndex))
    {
    case LUA_TNONE:
    case LUA_TNIL:
        throw CRainmanException(__FILE__, __LINE__, "Invalid type");
        break;

    case LUA_TBOOLEAN:
        eNameType = NT_Boolean;
        bName = (lua_toboolean(L, iIndex)) ? true : false;
        break;

    case LUA_TLIGHTUSERDATA:
        throw CRainmanException(__FILE__, __LINE__, "Invalid type");
        break;

    case LUA_TNUMBER:
        eNameType = NT_Double;
        fName = lua_tonumber(L, iIndex);
        break;

    case LUA_TSTRING:
        eNameType = NT_String;
        sName = lua_tostring(L, iIndex);
        break;

    case LUA_TTABLE:
    case LUA_TFUNCTION:
    case LUA_TUSERDATA:
    case LUA_TTHREAD:
        throw CRainmanException(__FILE__, __LINE__, "Invalid type");
        break;
    default:
        break;
    }
}

// Stack required: 2 - no matter how deep (1 returned, 1 temp)
void CLuaFile::_NodeLocator::GetValue(lua_State *L) const
{
    if (eNameType == NT_String && sName.empty())
        throw CRainmanException(__FILE__, __LINE__, "Blank name"); // Stack change: 0
    if (L == nullptr)
        throw CRainmanException(__FILE__, __LINE__, "No state"); // Stack change: 0
    if (pParent)
    {
        try
        {
            pParent->GetValue(L); // Parent added to stack
        }
        catch (const CRainmanException &e)
        {
            throw CRainmanException(e, __FILE__, __LINE__, "Could not fetch parent"); // Stack change: 0
        }
    }
    switch (eNameType)
    {
    case NT_String:
        lua_pushstring(L, sName.c_str()); // Key added to stack
        break;

    case NT_Double:
        lua_pushnumber(L, fName); // Key added to stack
        break;

    case NT_Boolean:
        lua_pushboolean(L, bName ? 1 : 0); // Key added to stack
        break;
    }
    if (pParent)
    {
        lua_gettable(L, -2); // Key replaced with value
        lua_remove(L, -2);   // Parent removed from stack
    }
    else
    {
        lua_gettable(L, LUA_GLOBALSINDEX); // Key replaced with value
    }
    // Stack change: +1 (1 new item)
}

// Stack required: 2 - no matter how deep (0 returned, 2 temp)
void CLuaFile::_NodeLocator::SetValue(lua_State *L) const
{
    if (eNameType == NT_String && sName.empty())
        throw CRainmanException(__FILE__, __LINE__, "Invalid name"); // Stack change: 0
    if (L == nullptr)
        throw CRainmanException(__FILE__, __LINE__, "No state"); // Stack change: 0
    if (lua_type(L, -1) == LUA_TNONE)
        throw CRainmanException(__FILE__, __LINE__, "No value on stack"); // Stack change: 0

    if (pParent)
    {
        try
        {
            pParent->GetValue(L); // Parent added to stack
        }
        catch (const CRainmanException &e)
        {
            throw CRainmanException(e, __FILE__, __LINE__, "Could not get parent");
        }
        lua_insert(L, -2); // Move parent to below value
    }
    switch (eNameType)
    {
    case NT_String:
        lua_pushstring(L, sName.c_str()); // Key added to stack
        break;

    case NT_Double:
        lua_pushnumber(L, fName); // Key added to stack
        break;

    case NT_Boolean:
        lua_pushboolean(L, bName ? 1 : 0); // Key added to stack
        break;
    }
    lua_insert(L, -2); // Move key to below value and above table
    if (pParent)
    {
        lua_settable(L, -3); // Key and value removed from stack
        lua_pop(L, 1);       // Parent removed from stack
    }
    else
    {
        lua_settable(L, LUA_GLOBALSINDEX); // Key and value removed from stack
    }
    // Stack change: -1 (1 removed item)
}

static bool IsALLNumerical(const char *s)
{
    if (!s)
        return false;
    ++s;
    if (*s == 0)
        return false;
    while (*s)
    {
        if (*s < '0' || *s > '9')
            return false;
        ++s;
    }
    return true;
}

bool CLuaFile::_SortCMetaTableChildren(const std::unique_ptr<CLuaFile::_NodeLocator> &p1,
                                       const std::unique_ptr<CLuaFile::_NodeLocator> &p2)
{
    if (p1->eNameType != p2->eNameType)
        return ((int)p1->eNameType) < ((int)p2->eNameType);

    if (p1->eNameType == CLuaFile::_NodeLocator::NT_String)
    {
        if (p1->sName.empty() || p2->sName.empty())
        {
            if (p1->sName.empty() && !p2->sName.empty())
                return true;
            if (p2->sName.empty() && !p1->sName.empty())
                return false;
            return false;
        }

        auto pos1 = p1->sName.rfind('_');
        auto pos2 = p2->sName.rfind('_');
        if (pos1 != std::string::npos && pos2 != std::string::npos)
        {
            const char *sSuffix1 = p1->sName.c_str() + pos1;
            const char *sSuffix2 = p2->sName.c_str() + pos2;
            if (IsALLNumerical(sSuffix1) && IsALLNumerical(sSuffix2))
            {
                // Compare prefixes (before the underscore)
                std::string sPrefix1(p1->sName, 0, pos1);
                std::string sPrefix2(p2->sName, 0, pos2);
                if (stricmp(sPrefix1.c_str(), sPrefix2.c_str()) == 0)
                {
                    long l1 = atol(sSuffix1 + 1);
                    long l2 = atol(sSuffix2 + 1);
                    return (l1 < l2);
                }
                else
                {
                    return (stricmp(p1->sName.c_str(), p2->sName.c_str()) < 0);
                }
            }
        }
        return (stricmp(p1->sName.c_str(), p2->sName.c_str()) < 0);
    }
    else if (p1->eNameType == CLuaFile::_NodeLocator::NT_Double)
    {
        return p1->fName < p2->fName;
    }
    return false;
}

// Stack required: 2 - no matter how deep (0 returned, 2 temp)
void CLuaFile::_TableToNodeList(CLuaFile::_NodeList &lstNodeList, lua_State *L, const CLuaFile::_NodeLocator *pParent,
                                bool bSkip_G)
{
    if (L == nullptr)
        throw CRainmanException(__FILE__, __LINE__, "No state");
    if (lua_isuserdata(L, -1))
    {
        void *v = lua_touserdata(L, -1);
        lua_pushlightuserdata(L, v);
        lua_remove(L, -2);
        lua_gettable(L, LUA_REGISTRYINDEX);
    }

    lua_pushnil(L);              // push null key
    while (lua_next(L, -2) != 0) // pop last key, [push new key and value]
    {
        lua_pop(L, 1); // pop value
        if (bSkip_G && (lua_type(L, -1) == LUA_TSTRING) &&
            ((strcmp(lua_tostring(L, -1), "_G") == 0) || (strcmp(lua_tostring(L, -1), "DebugThis") == 0)))
        {
#ifdef RAINMAN_GNUC
            ;
#else
            __nop();
#endif
        }
        else
            lstNodeList.push_back(std::make_unique<_NodeLocator>(L, -1, pParent));
    }

    std::sort(lstNodeList.begin(), lstNodeList.end(), _SortCMetaTableChildren);
}

CLuaFile::CMetaNode::CMetaNode(const CLuaFile::_NodeLocator *pNode, lua_State *L) : m_pNode(pNode), m_pLua(L)
{
    switch (pNode->eNameType)
    {
    case CLuaFile::_NodeLocator::NT_String:
        m_sName = pNode->sName;
        break;

    case CLuaFile::_NodeLocator::NT_Boolean:
        m_sName = pNode->bName ? "true" : "false";
        break;

    case CLuaFile::_NodeLocator::NT_Double:
    {
        char buf[24];
        sprintf(buf, "%.16g", pNode->fName);
        m_sName = buf;
        break;
    }
    };

    m_iNameHash = (unsigned long)hash((ub1 *)m_sName.c_str(), (ub4)m_sName.size(), 0);
}

CLuaFile::CMetaNode::~CMetaNode() {}

CLuaFile::CMetaNode::eDataTypes CLuaFile::CMetaNode::VGetType()
{
    try
    {
        m_pNode->GetValue(m_pLua);
    }
    CATCH_THROW("Unable to get data")

    eDataTypes eRet = DT_NoData;
    switch (lua_type(m_pLua, -1))
    {
    case LUA_TBOOLEAN:
        eRet = DT_Bool;
        break;

    case LUA_TNUMBER:
        eRet = DT_Float;
        break;

    case LUA_TSTRING:
        eRet = DT_String;
        break;

    case LUA_TTABLE:
        eRet = DT_Table;
        break;

    case LUA_TUSERDATA:
    {
        auto *pExisting = (tLuaTableProtector *)lua_touserdata(m_pLua, -1);
        if (pExisting->iMagic == 0x7291BEEF)
            eRet = DT_Table;
        break;
    }
    default:
        break;
    };
    lua_pop(m_pLua, 1);
    return eRet;
}

const char *CLuaFile::CMetaNode::VGetName() { return m_sName.c_str(); }

unsigned long CLuaFile::CMetaNode::VGetNameHash() { return m_iNameHash; }

float CLuaFile::CMetaNode::VGetValueFloat()
{
    try
    {
        m_pNode->GetValue(m_pLua);
    }
    catch (const CRainmanException &e)
    {
        throw CRainmanException(e, __FILE__, __LINE__, "Could not get value");
    }
    if (!lua_isnumber(m_pLua, -1))
    {
        lua_pop(m_pLua, 1);
        throw CRainmanException(__FILE__, __LINE__, "Is not a number");
    }
    auto f = (float)lua_tonumber(m_pLua, -1);
    lua_pop(m_pLua, 1);
    return f;
}

unsigned long CLuaFile::CMetaNode::VGetValueInteger()
{
    throw CRainmanException(__FILE__, __LINE__, "Lua files cannot have integers; only floating point values");
}

bool CLuaFile::CMetaNode::VGetValueBool()
{
    try
    {
        m_pNode->GetValue(m_pLua);
    }
    catch (const CRainmanException &e)
    {
        throw CRainmanException(e, __FILE__, __LINE__, "Could not get value");
    }
    if (!lua_isboolean(m_pLua, -1))
    {
        lua_pop(m_pLua, 1);
        throw CRainmanException(__FILE__, __LINE__, "Is not a boolean");
    }
    bool b = (lua_toboolean(m_pLua, -1)) ? true : false;
    lua_pop(m_pLua, 1);
    return b;
}

const char *CLuaFile::CMetaNode::VGetValueString()
{
    try
    {
        m_pNode->GetValue(m_pLua);
    }
    catch (const CRainmanException &e)
    {
        throw CRainmanException(e, __FILE__, __LINE__, "Could not get value");
    }
    if (!lua_isstring(m_pLua, -1))
    {
        lua_pop(m_pLua, 1);
        throw CRainmanException(__FILE__, __LINE__, "Is not a string");
    }
    m_sValue = lua_tostring(m_pLua, -1);
    lua_pop(m_pLua, 1);
    return m_sValue.c_str();
}

const wchar_t *CLuaFile::CMetaNode::VGetValueWString()
{
    throw CRainmanException(__FILE__, __LINE__, "Lua files cannot contain unicode strings");
}

IMetaNode::IMetaTable *CLuaFile::CMetaNode::VGetValueMetatable()
{
    try
    {
        m_pNode->GetValue(m_pLua);
    }
    catch (const CRainmanException &e)
    {
        throw CRainmanException(e, __FILE__, __LINE__, "Could not get value");
    }
    if (lua_isuserdata(m_pLua, -1))
    {
        auto *pExisting = (tLuaTableProtector *)lua_touserdata(m_pLua, -1);
        if (pExisting->iMagic != 0x7291BEEF)
        {
            lua_pop(m_pLua, 1);
            throw CRainmanException(__FILE__, __LINE__, "Magic value is wrong");
        }
    }
    else
    {
        if (!lua_istable(m_pLua, -1))
        {
            lua_pop(m_pLua, 1);
            throw CRainmanException(__FILE__, __LINE__, "Not a table");
        }
    }
    auto *t = new CLuaFile::CMetaTable(m_pLua);
    if (!t)
    {
        lua_pop(m_pLua, 1);
        throw CRainmanException(__FILE__, __LINE__, "Memory allocate error");
    }
    try
    {
        t->_Init(m_pNode);
    }
    catch (const CRainmanException &e)
    {
        delete t;
        lua_pop(m_pLua, 1);
        throw CRainmanException(e, __FILE__, __LINE__, "Could not init meta table");
    }
    lua_pop(m_pLua, 1);
    return t;
}

void CLuaFile::CMetaNode::VSetType(IMetaNode::eDataTypes eType)
{
    throw CRainmanException(__FILE__, __LINE__, "Lua files cannot be edited yet :/");
}

void CLuaFile::CMetaNode::VSetName(const char *sName)
{
    throw CRainmanException(__FILE__, __LINE__, "Lua files cannot be edited yet :/");
}

void CLuaFile::CMetaNode::VSetNameHash(unsigned long iHash)
{
    throw CRainmanException(__FILE__, __LINE__, "Lua files cannot be edited yet :/");
}

void CLuaFile::CMetaNode::VSetValueFloat(float fValue)
{
    throw CRainmanException(__FILE__, __LINE__, "Lua files cannot be edited yet :/");
}

void CLuaFile::CMetaNode::VSetValueInteger(unsigned long)
{
    throw CRainmanException(__FILE__, __LINE__, "Lua files cannot be edited yet :/");
}

void CLuaFile::CMetaNode::VSetValueBool(bool bValue)
{
    throw CRainmanException(__FILE__, __LINE__, "Lua files cannot be edited yet :/");
}

void CLuaFile::CMetaNode::VSetValueString(const char *sValue)
{
    throw CRainmanException(__FILE__, __LINE__, "Lua files cannot be edited yet :/");
}

void CLuaFile::CMetaNode::VSetValueWString(const wchar_t *wsValue)
{
    throw CRainmanException(__FILE__, __LINE__, "Lua files cannot be edited yet :/");
}

CMemoryStore::COutStream *CLuaFile::CMetaNode::VGetNodeAsRainmanRgd()
{
    throw CRainmanException(__FILE__, __LINE__, "/todo ;)");
}

void CLuaFile::CMetaNode::SGetNodeFromRainmanRgd(IFileStore::IStream *pInput, bool bSetName)
{
    throw CRainmanException(__FILE__, __LINE__, "/todo ;)");
}

CLuaFile::CMetaTable::CMetaTable(lua_State *L) { m_pLua = L; }

void CLuaFile::CMetaTable::_Init(const CLuaFile::_NodeLocator *pParent)
{
    try
    {
        CLuaFile::_TableToNodeList(m_vecChildren, m_pLua, pParent);
    }
    catch (const CRainmanException &e)
    {
        throw CRainmanException(e, __FILE__, __LINE__, "Could not make node list");
    }
    for (auto itr = m_vecChildren.begin(); itr != m_vecChildren.end(); ++itr)
    {
        if (((*itr)->eNameType == CLuaFile::_NodeLocator::NT_String) && ((*itr)->sName == "$dow_mod_studio"))
        {
            m_pDowModStudio = std::move(*itr);
            m_vecChildren.erase(itr);

            try
            {
                m_pDowModStudio->GetValue(m_pLua); // push $dow_mod_studio table onto the stack
            }
            catch (const CRainmanException &e)
            {
                throw CRainmanException(e, __FILE__, __LINE__, "Could not fetch value");
            }

            lua_pushstring(m_pLua, "ref_name"); // push ref_name onto the stack for _NodeLocator
            m_pRef = std::make_unique<CLuaFile::_NodeLocator>(m_pLua, -1, m_pDowModStudio.get());
            lua_pop(m_pLua, 2); // pop ref_name and $dow_mod_studio

            break;
        }
    }
}

CLuaFile::CMetaTable::~CMetaTable() {}

unsigned long CLuaFile::CMetaTable::VGetChildCount() { return static_cast<unsigned long>(m_vecChildren.size()); }

IMetaNode *CLuaFile::CMetaTable::VGetChild(unsigned long iIndex)
{
    if (iIndex >= VGetChildCount())
        throw CRainmanException(nullptr, __FILE__, __LINE__, "Index %lu is beyond %lu", iIndex, VGetChildCount());
    return new CLuaFile::CMetaNode(m_vecChildren[iIndex].get(), m_pLua);
}

IMetaNode::eDataTypes CLuaFile::CMetaTable::VGetReferenceType()
{
    if (!m_pRef)
        return CLuaFile::CMetaNode::DT_NoData;
    if (m_pRef->sName.empty())
        throw CRainmanException(__FILE__, __LINE__, "Blank m_pRef name");
    return CLuaFile::CMetaNode::DT_String; // i set it; i know its a string
}

const char *CLuaFile::CMetaTable::VGetReferenceString()
{
    if (!m_pRef)
        throw CRainmanException(__FILE__, __LINE__, "No m_pRef");
    if (m_pRef->sName.empty())
        throw CRainmanException(__FILE__, __LINE__, "Blank m_pRef name");
    if (m_sRef.empty())
    {
        try
        {
            m_pRef->GetValue(m_pLua);
        }
        catch (const CRainmanException &e)
        {
            throw CRainmanException(e, __FILE__, __LINE__, "Could not get value");
        }
        m_sRef = lua_tostring(m_pLua, -1);
        lua_pop(m_pLua, 1);
    }
    return m_sRef.c_str();
}

const wchar_t *CLuaFile::CMetaTable::VGetReferenceWString()
{
    throw CRainmanException(__FILE__, __LINE__, "Lua files cannot have unicode strings");
}

void CLuaFile::CMetaTable::VSetReferenceType(IMetaNode::eDataTypes eType)
{
    throw CRainmanException(__FILE__, __LINE__, "Lua files cannot be edited yet :/");
}

void CLuaFile::CMetaTable::VSetReferenceString(const char *sValue)
{
    throw CRainmanException(__FILE__, __LINE__, "Lua files cannot be edited yet :/");
}

void CLuaFile::CMetaTable::VSetReferenceWString(const wchar_t *wsValue)
{
    throw CRainmanException(__FILE__, __LINE__, "Lua files cannot be edited yet :/");
}

IMetaNode *CLuaFile::CMetaTable::VAddChild(const char *sName)
{
    throw CRainmanException(__FILE__, __LINE__, "Lua files cannot be edited yet :/");
}

void CLuaFile::CMetaTable::VDeleteChild(unsigned long iIndex)
{
    throw CRainmanException(__FILE__, __LINE__, "Lua files cannot be edited yet :/");
}
