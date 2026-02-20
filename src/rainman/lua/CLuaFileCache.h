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

#pragma once
#include "rainman/core/gnuc_defines.h"
extern "C"
{
#include <lua.h>
};
#include "rainman/core/Api.h"
#include <memory>
#include <mutex>
#include <string>

//! Cache for lua_State objects
/*!

*/
class RAINMAN_API CLuaFileCache
{
  public:
    CLuaFileCache();
    ~CLuaFileCache();
    void Clear();

    //! Make a new lua state
    /*!
        Creates a new lua state that will be destroyed when
        this object is destroyed.
    */
    lua_State *MakeState();

    //! Free a lua state
    void FreeState(lua_State *L);

    //! Assign a name to a state
    /*!
        State must have been created with MakeState()
    */
    void AddToCache(const char *sName, lua_State *L);

    //! Find a state by name
    lua_State *Fetch(const char *sName);

    //! Copy a GameData table from a chached file to a lua state stack
    /*!
        L must be a state created with MakeState()
    */
    void GameDataToStack(const char *sRef, lua_State *L);

    //! Copy a MetaData table from a chached file to a lua state stack
    /*!
        L must be a state created with MakeState()
    */
    void MetaDataToStack(const char *sRef, lua_State *L);

  protected:
    mutable std::recursive_mutex m_mtx; //!< Guards all public methods for thread safety

    struct _tEntry
    {
        _tEntry(std::string name, lua_State *L, bool b = false);
        lua_State *L;
        std::string sName;
        std::unique_ptr<_tEntry> pNext;
        bool bUseful;
    };

    std::unique_ptr<lua_State, decltype(&lua_close)> m_pMother;
    std::unique_ptr<_tEntry> m_pHead;
    _tEntry *m_pTail;
};
