/*
** Lua 5.1.2 API header (statically linked with lua51_ prefixed symbols).
**
** Lua 5.1.2 is compiled into lua512.lib with all symbols renamed via
** lua512_rename.h (applied as a force-include during compilation).
** This header declares those renamed symbols directly so that consumers
** do NOT need the rename macros — avoiding conflicts with Lua 5.0.2
** headers that are also used in this codebase.
**
** IMPORTANT: This header must NOT include any Lua source headers or the
** rename header.  Types shared with Lua 5.0.2 use compatible forward
** declarations; types unique to 5.1.2 use prefixed names (lua_Integer,
** lua_Alloc, lua_Reader, lua_Writer).
*/

#ifndef LUA51_H_
#define LUA51_H_

#include <stdarg.h>
#include <stddef.h>

/*
** Forward declarations / type definitions.
** lua_State, lua_Number, lua_CFunction, lua_Debug, and lua_Hook are
** also defined by Lua 5.0.2 headers.  C++ allows duplicate identical
** typedefs, so including both headers is safe.
*/
#ifndef LUA_NUMBER
#define LUA_NUMBER double
#endif
typedef struct lua_State lua_State;
typedef LUA_NUMBER lua_Number;
typedef int (*lua_CFunction) (lua_State *L);
typedef struct lua_Debug lua_Debug;
typedef void (*lua_Hook) (lua_State *L, lua_Debug *ar);

/* Types unique to Lua 5.1.2 (not in 5.0.2) */
typedef ptrdiff_t lua_Integer;
typedef void * (*lua_Alloc) (void *ud, void *ptr, size_t osize, size_t nsize);
typedef const char * (*lua_Reader) (lua_State *L, void *ud, size_t *sz);
typedef int (*lua_Writer) (lua_State *L, const void* p, size_t sz, void* ud);

/*
** 5.1 pseudo-indices (different values from 5.0.2)
*/
#define LUA51_REGISTRYINDEX  (-10000)
#define LUA51_ENVIRONINDEX   (-10001)
#define LUA51_GLOBALSINDEX   (-10002)
#define lua51_upvalueindex(i) (LUA51_GLOBALSINDEX-(i))

#ifdef __cplusplus
extern "C" {
#endif

/*
** State manipulation
*/
lua_State  *lua51_newstate (lua_Alloc f, void *ud);
void        lua51_close (lua_State *L);
lua_State  *lua51_newthread (lua_State *L);
lua_CFunction lua51_atpanic (lua_State *L, lua_CFunction panicf);

/*
** Stack manipulation
*/
int   lua51_gettop (lua_State *L);
void  lua51_settop (lua_State *L, int idx);
void  lua51_pushvalue (lua_State *L, int idx);
void  lua51_remove (lua_State *L, int idx);
void  lua51_insert (lua_State *L, int idx);
void  lua51_replace (lua_State *L, int idx);
int   lua51_checkstack (lua_State *L, int sz);
void  lua51_xmove (lua_State *from, lua_State *to, int n);

/*
** Access functions (stack -> C)
*/
int             lua51_isnumber (lua_State *L, int idx);
int             lua51_isstring (lua_State *L, int idx);
int             lua51_iscfunction (lua_State *L, int idx);
int             lua51_isuserdata (lua_State *L, int idx);
int             lua51_type (lua_State *L, int idx);
const char     *lua51_typename (lua_State *L, int tp);
int             lua51_equal (lua_State *L, int idx1, int idx2);
int             lua51_rawequal (lua_State *L, int idx1, int idx2);
int             lua51_lessthan (lua_State *L, int idx1, int idx2);
lua_Number      lua51_tonumber (lua_State *L, int idx);
lua_Integer     lua51_tointeger (lua_State *L, int idx);
int             lua51_toboolean (lua_State *L, int idx);
const char     *lua51_tolstring (lua_State *L, int idx, size_t *len);
size_t          lua51_objlen (lua_State *L, int idx);
lua_CFunction   lua51_tocfunction (lua_State *L, int idx);
void           *lua51_touserdata (lua_State *L, int idx);
lua_State      *lua51_tothread (lua_State *L, int idx);
const void     *lua51_topointer (lua_State *L, int idx);

/*
** Push functions (C -> stack)
*/
void  lua51_pushnil (lua_State *L);
void  lua51_pushnumber (lua_State *L, lua_Number n);
void  lua51_pushinteger (lua_State *L, lua_Integer n);
void  lua51_pushlstring (lua_State *L, const char *s, size_t l);
void  lua51_pushstring (lua_State *L, const char *s);
const char *lua51_pushvfstring (lua_State *L, const char *fmt, va_list argp);
const char *lua51_pushfstring (lua_State *L, const char *fmt, ...);
void  lua51_pushcclosure (lua_State *L, lua_CFunction fn, int n);
void  lua51_pushboolean (lua_State *L, int b);
void  lua51_pushlightuserdata (lua_State *L, void *p);
int   lua51_pushthread (lua_State *L);

/*
** Get functions (Lua -> stack)
*/
void  lua51_gettable (lua_State *L, int idx);
void  lua51_getfield (lua_State *L, int idx, const char *k);
void  lua51_rawget (lua_State *L, int idx);
void  lua51_rawgeti (lua_State *L, int idx, int n);
void  lua51_createtable (lua_State *L, int narr, int nrec);
void *lua51_newuserdata (lua_State *L, size_t sz);
int   lua51_getmetatable (lua_State *L, int objindex);
void  lua51_getfenv (lua_State *L, int idx);

/*
** Set functions (stack -> Lua)
*/
void  lua51_settable (lua_State *L, int idx);
void  lua51_setfield (lua_State *L, int idx, const char *k);
void  lua51_rawset (lua_State *L, int idx);
void  lua51_rawseti (lua_State *L, int idx, int n);
int   lua51_setmetatable (lua_State *L, int objindex);
int   lua51_setfenv (lua_State *L, int idx);

/*
** Load and call functions
*/
void  lua51_call (lua_State *L, int nargs, int nresults);
int   lua51_pcall (lua_State *L, int nargs, int nresults, int errfunc);
int   lua51_cpcall (lua_State *L, lua_CFunction func, void *ud);
int   lua51_load (lua_State *L, lua_Reader reader, void *dt, const char *chunkname);
int   lua51_dump (lua_State *L, lua_Writer writer, void *data);

/*
** Coroutine functions
*/
int  lua51_yield (lua_State *L, int nresults);
int  lua51_resume (lua_State *L, int narg);
int  lua51_status (lua_State *L);

/*
** GC and miscellaneous
*/
int   lua51_gc (lua_State *L, int what, int data);
int   lua51_error (lua_State *L);
int   lua51_next (lua_State *L, int idx);
void  lua51_concat (lua_State *L, int n);
lua_Alloc lua51_getallocf (lua_State *L, void **ud);
void  lua51_setallocf (lua_State *L, lua_Alloc f, void *ud);

/*
** Debug API — uses forward-declared lua_Debug (defined by Lua 5.0.2
** headers; layout-compatible for pointer usage)
*/
int  lua51_getstack (lua_State *L, int level, lua_Debug *ar);
int  lua51_getinfo (lua_State *L, const char *what, lua_Debug *ar);
const char *lua51_getlocal (lua_State *L, const lua_Debug *ar, int n);
const char *lua51_setlocal (lua_State *L, const lua_Debug *ar, int n);
const char *lua51_getupvalue (lua_State *L, int funcindex, int n);
const char *lua51_setupvalue (lua_State *L, int funcindex, int n);
int  lua51_sethook (lua_State *L, lua_Hook func, int mask, int count);
lua_Hook lua51_gethook (lua_State *L);
int  lua51_gethookmask (lua_State *L);
int  lua51_gethookcount (lua_State *L);

#ifdef __cplusplus
}
#endif

/*
** Convenience macros
*/
#define lua51_pop(L,n)              lua51_settop(L, -(n)-1)
#define lua51_newtable(L)           lua51_createtable(L, 0, 0)
#define lua51_register(L,n,f)       (lua51_pushcfunction(L, (f)), lua51_setglobal(L, (n)))
#define lua51_pushcfunction(L,f)    lua51_pushcclosure(L, (f), 0)
#define lua51_strlen(L,i)           lua51_objlen(L, (i))
#define lua51_isfunction(L,n)       (lua51_type(L, (n)) == LUA_TFUNCTION)
#define lua51_istable(L,n)          (lua51_type(L, (n)) == LUA_TTABLE)
#define lua51_islightuserdata(L,n)  (lua51_type(L, (n)) == LUA_TLIGHTUSERDATA)
#define lua51_isnil(L,n)            (lua51_type(L, (n)) == LUA_TNIL)
#define lua51_isboolean(L,n)        (lua51_type(L, (n)) == LUA_TBOOLEAN)
#define lua51_isthread(L,n)         (lua51_type(L, (n)) == LUA_TTHREAD)
#define lua51_isnone(L,n)           (lua51_type(L, (n)) == LUA_TNONE)
#define lua51_isnoneornil(L, n)     (lua51_type(L, (n)) <= 0)
#define lua51_pushliteral(L, s)     lua51_pushlstring(L, "" s, (sizeof(s)/sizeof(char))-1)
#define lua51_setglobal(L,s)        lua51_setfield(L, LUA51_GLOBALSINDEX, (s))
#define lua51_getglobal(L,s)        lua51_getfield(L, LUA51_GLOBALSINDEX, (s))
#define lua51_tostring(L,i)         lua51_tolstring(L, (i), NULL)
#define lua51_open()                lua51L_newstate()
#define lua51_getregistry(L)        lua51_pushvalue(L, LUA51_REGISTRYINDEX)
#define lua51_getgccount(L)         lua51_gc(L, LUA_GCCOUNT, 0)

/*
** Init function (no-op — Lua 5.1.2 is statically linked)
*/
bool Lua51_Load(const wchar_t* sDll = 0);

#endif /* LUA51_H_ */
