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

#include "rainman/lua/CLuaFileCache.h"
#include "rainman/core/memdebug.h"
#include "rainman/lua/luax.h"
#include <cstring>
#include <utility>
#include "rainman/core/Exception.h"
extern "C"
{
#include <lauxlib.h>
};

CLuaFileCache::CLuaFileCache() : m_pMother(lua_open(), &lua_close), m_pTail(nullptr) {}

CLuaFileCache::_tEntry::_tEntry(std::string name, lua_State *L, bool b) : L(L), sName(std::move(name)), bUseful(b) {}

lua_State *CLuaFileCache::MakeState()
{
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    auto *pMother = m_pMother.get();
    lua_State *L = lua_newthread(pMother);     // {L -1}
    lua_newtable(L);                           // {L -2} {T -1}
    lua_replace(L, LUA_GLOBALSINDEX);          // {L -1}
    lua_pushlightuserdata(pMother, (void *)L); // {L -2} {U -1}
    lua_insert(pMother, -2);                   // {U -2} {L -1}
    lua_settable(pMother, LUA_REGISTRYINDEX);  //

    auto pEntry = std::make_unique<_tEntry>(std::string{}, L);
    auto *pRaw = pEntry.get();
    if (m_pTail)
        m_pTail->pNext = std::move(pEntry);
    else
        m_pHead = std::move(pEntry);
    m_pTail = pRaw;
    return L;
}

void CLuaFileCache::FreeState(lua_State *L)
{
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    _tEntry *pPrev = nullptr;
    _tEntry *pItr = m_pHead.get();
    bool found = false;
    while (pItr)
    {
        if (pItr->L == L)
        {
            if (pItr->bUseful)
                throw CRainmanException(__FILE__, __LINE__, "Cannot delete state as it is useful");
            // Unlink the node via its owning unique_ptr
            std::unique_ptr<_tEntry> &owningPtr = pPrev ? pPrev->pNext : m_pHead;
            auto removed = std::move(owningPtr);
            owningPtr = std::move(removed->pNext);
            if (m_pTail == removed.get())
                m_pTail = pPrev;
            found = true;
            break;
        }
        pPrev = pItr;
        pItr = pItr->pNext.get();
    }
    if (!found)
        throw CRainmanException(__FILE__, __LINE__, "State not found");
    auto *pMother = m_pMother.get();
    lua_pushlightuserdata(pMother, (void *)L); // {U -1}
    lua_pushnil(pMother);                      // {U -2} {N -1}
    lua_settable(pMother, LUA_REGISTRYINDEX);  //
#ifdef LUA_GCCOLLECT
    lua_gc(pMother, LUA_GCCOLLECT, 0);
#else
    lua_setgcthreshold(pMother, 0);
#endif
}

void CLuaFileCache::Clear()
{
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    m_pHead.reset();
    m_pTail = nullptr;
    m_pMother.reset(lua_open());
}

CLuaFileCache::~CLuaFileCache()
{
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    // Entries must be cleared before m_pMother is closed,
    // since lua_State threads are children of m_pMother.
    m_pHead.reset();
    m_pTail = nullptr;
}

void CLuaFileCache::AddToCache(const char *sName, lua_State *L)
{
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    RAINMAN_LOG_DEBUG("CLuaFileCache::AddToCache(\"{}\")", sName ? sName : "(null)");
    _tEntry *pItr = m_pHead.get();
    while (pItr)
    {
        if (pItr->L == L)
        {
            pItr->sName = sName;
            return;
        }
        pItr = pItr->pNext.get();
    }
    throw CRainmanException(__FILE__, __LINE__, "State not found");
}

lua_State *CLuaFileCache::Fetch(const char *sName)
{
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    RAINMAN_LOG_DEBUG("CLuaFileCache::Fetch(\"{}\")", sName ? sName : "(null)");
    _tEntry *pItr = m_pHead.get();
    while (pItr)
    {
        if (!pItr->sName.empty() && stricmp(pItr->sName.c_str(), sName) == 0)
        {
            pItr->bUseful = true;
            return pItr->L;
        }
        pItr = pItr->pNext.get();
    }
    return nullptr;
}

void CLuaFileCache::GameDataToStack(const char *sRef, lua_State *L)
{
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    lua_State *Ls = Fetch(sRef);
    if (!Ls)
        throw CRainmanException(nullptr, __FILE__, __LINE__, "No state found with name \'%s\'", sRef);
    lua_pushstring(Ls, "GameData");
    lua_gettable(Ls, LUA_GLOBALSINDEX);
    lua_pushvalue(Ls, -1);
    lua_xmove(Ls, L, 1);
    lua_pop(Ls, 1);
}

void CLuaFileCache::MetaDataToStack(const char *sRef, lua_State *L)
{
    std::lock_guard<std::recursive_mutex> lock(m_mtx);
    lua_State *Ls = Fetch(sRef);
    if (!Ls)
        throw CRainmanException(nullptr, __FILE__, __LINE__, "No state found with name \'%s\'", sRef);
    lua_pushstring(Ls, "MetaData");
    lua_gettable(Ls, LUA_GLOBALSINDEX);
    lua_xmove(Ls, L, 1);
}
