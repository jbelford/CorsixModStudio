/*
** Lua 5.1.2 consumer header (statically linked with lua51_ prefixed symbols).
**
** Declares the full Lua 5.1.2 API: core, auxiliary library, standard library
** openers, and convenience macros — all with lua51_ / lua51L_ / lua51open_
** prefixes to avoid linker conflicts with the vendored Lua 5.0.2 library.
**
** Lua 5.1.2 is compiled into lua512.lib with all symbols renamed via
** lua512_rename.h (applied as a force-include during compilation).
** This header declares those renamed symbols directly so that consumers
** do NOT need the rename macros.
**
** IMPORTANT: This header must NOT include any Lua source headers or the
** rename header.  Types shared with Lua 5.0.2 use compatible forward
** declarations; types unique to 5.1.2 are declared here (lua_Integer,
** lua_Alloc, lua_Reader, lua_Writer).
*/

#ifndef LUA51_H_
#define LUA51_H_

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

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
typedef int (*lua_CFunction)(lua_State *L);
typedef struct lua_Debug lua_Debug;
typedef void (*lua_Hook)(lua_State *L, lua_Debug *ar);

/* Types unique to Lua 5.1.2 (not in 5.0.2) */
typedef ptrdiff_t lua_Integer;
typedef void *(*lua_Alloc)(void *ud, void *ptr, size_t osize, size_t nsize);
typedef const char *(*lua_Reader)(lua_State *L, void *ud, size_t *sz);
typedef int (*lua_Writer)(lua_State *L, const void *p, size_t sz, void *ud);

/*
** 5.1 pseudo-indices (different values from 5.0.2)
*/
#define LUA51_REGISTRYINDEX (-10000)
#define LUA51_ENVIRONINDEX (-10001)
#define LUA51_GLOBALSINDEX (-10002)
#define lua51_upvalueindex(i) (LUA51_GLOBALSINDEX - (i))

#ifdef __cplusplus
extern "C"
{
#endif

	/*
	** State manipulation
	*/
	lua_State *lua51_newstate(lua_Alloc f, void *ud);
	void lua51_close(lua_State *L);
	lua_State *lua51_newthread(lua_State *L);
	lua_CFunction lua51_atpanic(lua_State *L, lua_CFunction panicf);

	/*
	** Stack manipulation
	*/
	int lua51_gettop(lua_State *L);
	void lua51_settop(lua_State *L, int idx);
	void lua51_pushvalue(lua_State *L, int idx);
	void lua51_remove(lua_State *L, int idx);
	void lua51_insert(lua_State *L, int idx);
	void lua51_replace(lua_State *L, int idx);
	int lua51_checkstack(lua_State *L, int sz);
	void lua51_xmove(lua_State *from, lua_State *to, int n);

	/*
	** Access functions (stack -> C)
	*/
	int lua51_isnumber(lua_State *L, int idx);
	int lua51_isstring(lua_State *L, int idx);
	int lua51_iscfunction(lua_State *L, int idx);
	int lua51_isuserdata(lua_State *L, int idx);
	int lua51_type(lua_State *L, int idx);
	const char *lua51_typename(lua_State *L, int tp);
	int lua51_equal(lua_State *L, int idx1, int idx2);
	int lua51_rawequal(lua_State *L, int idx1, int idx2);
	int lua51_lessthan(lua_State *L, int idx1, int idx2);
	lua_Number lua51_tonumber(lua_State *L, int idx);
	lua_Integer lua51_tointeger(lua_State *L, int idx);
	int lua51_toboolean(lua_State *L, int idx);
	const char *lua51_tolstring(lua_State *L, int idx, size_t *len);
	size_t lua51_objlen(lua_State *L, int idx);
	lua_CFunction lua51_tocfunction(lua_State *L, int idx);
	void *lua51_touserdata(lua_State *L, int idx);
	lua_State *lua51_tothread(lua_State *L, int idx);
	const void *lua51_topointer(lua_State *L, int idx);

	/*
	** Push functions (C -> stack)
	*/
	void lua51_pushnil(lua_State *L);
	void lua51_pushnumber(lua_State *L, lua_Number n);
	void lua51_pushinteger(lua_State *L, lua_Integer n);
	void lua51_pushlstring(lua_State *L, const char *s, size_t l);
	void lua51_pushstring(lua_State *L, const char *s);
	const char *lua51_pushvfstring(lua_State *L, const char *fmt, va_list argp);
	const char *lua51_pushfstring(lua_State *L, const char *fmt, ...);
	void lua51_pushcclosure(lua_State *L, lua_CFunction fn, int n);
	void lua51_pushboolean(lua_State *L, int b);
	void lua51_pushlightuserdata(lua_State *L, void *p);
	int lua51_pushthread(lua_State *L);

	/*
	** Get functions (Lua -> stack)
	*/
	void lua51_gettable(lua_State *L, int idx);
	void lua51_getfield(lua_State *L, int idx, const char *k);
	void lua51_rawget(lua_State *L, int idx);
	void lua51_rawgeti(lua_State *L, int idx, int n);
	void lua51_createtable(lua_State *L, int narr, int nrec);
	void *lua51_newuserdata(lua_State *L, size_t sz);
	int lua51_getmetatable(lua_State *L, int objindex);
	void lua51_getfenv(lua_State *L, int idx);

	/*
	** Set functions (stack -> Lua)
	*/
	void lua51_settable(lua_State *L, int idx);
	void lua51_setfield(lua_State *L, int idx, const char *k);
	void lua51_rawset(lua_State *L, int idx);
	void lua51_rawseti(lua_State *L, int idx, int n);
	int lua51_setmetatable(lua_State *L, int objindex);
	int lua51_setfenv(lua_State *L, int idx);

	/*
	** Load and call functions
	*/
	void lua51_call(lua_State *L, int nargs, int nresults);
	int lua51_pcall(lua_State *L, int nargs, int nresults, int errfunc);
	int lua51_cpcall(lua_State *L, lua_CFunction func, void *ud);
	int lua51_load(lua_State *L, lua_Reader reader, void *dt, const char *chunkname);
	int lua51_dump(lua_State *L, lua_Writer writer, void *data);

	/*
	** Coroutine functions
	*/
	int lua51_yield(lua_State *L, int nresults);
	int lua51_resume(lua_State *L, int narg);
	int lua51_status(lua_State *L);

	/*
	** GC and miscellaneous
	*/
	int lua51_gc(lua_State *L, int what, int data);
	int lua51_error(lua_State *L);
	int lua51_next(lua_State *L, int idx);
	void lua51_concat(lua_State *L, int n);
	lua_Alloc lua51_getallocf(lua_State *L, void **ud);
	void lua51_setallocf(lua_State *L, lua_Alloc f, void *ud);

	/*
	** Debug API — uses forward-declared lua_Debug (defined by Lua 5.0.2
	** headers; layout-compatible for pointer usage)
	*/
	int lua51_getstack(lua_State *L, int level, lua_Debug *ar);
	int lua51_getinfo(lua_State *L, const char *what, lua_Debug *ar);
	const char *lua51_getlocal(lua_State *L, const lua_Debug *ar, int n);
	const char *lua51_setlocal(lua_State *L, const lua_Debug *ar, int n);
	const char *lua51_getupvalue(lua_State *L, int funcindex, int n);
	const char *lua51_setupvalue(lua_State *L, int funcindex, int n);
	int lua51_sethook(lua_State *L, lua_Hook func, int mask, int count);
	lua_Hook lua51_gethook(lua_State *L);
	int lua51_gethookmask(lua_State *L);
	int lua51_gethookcount(lua_State *L);

#ifdef __cplusplus
}
#endif

/*
** Convenience macros (core API)
*/
#define lua51_pop(L, n) lua51_settop(L, -(n) - 1)
#define lua51_newtable(L) lua51_createtable(L, 0, 0)
#define lua51_register(L, n, f) (lua51_pushcfunction(L, (f)), lua51_setglobal(L, (n)))
#define lua51_pushcfunction(L, f) lua51_pushcclosure(L, (f), 0)
#define lua51_strlen(L, i) lua51_objlen(L, (i))
#define lua51_isfunction(L, n) (lua51_type(L, (n)) == LUA_TFUNCTION)
#define lua51_istable(L, n) (lua51_type(L, (n)) == LUA_TTABLE)
#define lua51_islightuserdata(L, n) (lua51_type(L, (n)) == LUA_TLIGHTUSERDATA)
#define lua51_isnil(L, n) (lua51_type(L, (n)) == LUA_TNIL)
#define lua51_isboolean(L, n) (lua51_type(L, (n)) == LUA_TBOOLEAN)
#define lua51_isthread(L, n) (lua51_type(L, (n)) == LUA_TTHREAD)
#define lua51_isnone(L, n) (lua51_type(L, (n)) == LUA_TNONE)
#define lua51_isnoneornil(L, n) (lua51_type(L, (n)) <= 0)
#define lua51_pushliteral(L, s) lua51_pushlstring(L, "" s, (sizeof(s) / sizeof(char)) - 1)
#define lua51_setglobal(L, s) lua51_setfield(L, LUA51_GLOBALSINDEX, (s))
#define lua51_getglobal(L, s) lua51_getfield(L, LUA51_GLOBALSINDEX, (s))
#define lua51_tostring(L, i) lua51_tolstring(L, (i), NULL)
#define lua51_open() lua51L_newstate()
#define lua51_getregistry(L) lua51_pushvalue(L, LUA51_REGISTRYINDEX)
#define lua51_getgccount(L) lua51_gc(L, LUA_GCCOUNT, 0)

/*
** =====================================================================
** Auxiliary library (lua51L_*)
** =====================================================================
*/

/* extra error code for luaL_load */
#define LUA51_ERRFILE 6

typedef struct lua51L_Reg
{
	const char *name;
	lua_CFunction func;
} lua51L_Reg;

#define lua51I_openlib lua51L_openlib

/*
** Buffer type — uses BUFSIZ from <stdio.h>
*/
#ifndef LUA51L_BUFFERSIZE
#define LUA51L_BUFFERSIZE BUFSIZ
#endif

typedef struct lua51L_Buffer
{
	char *p;
	int lvl;
	lua_State *L;
	char buffer[LUA51L_BUFFERSIZE];
} lua51L_Buffer;

#ifdef __cplusplus
extern "C"
{
#endif

	/*
	** Auxiliary library functions
	*/
	void lua51L_openlib(lua_State *L, const char *libname, const lua51L_Reg *l, int nup);
	void lua51L_register(lua_State *L, const char *libname, const lua51L_Reg *l);
	int lua51L_getmetafield(lua_State *L, int obj, const char *e);
	int lua51L_callmeta(lua_State *L, int obj, const char *e);
	int lua51L_typerror(lua_State *L, int narg, const char *tname);
	int lua51L_argerror(lua_State *L, int numarg, const char *extramsg);
	const char *lua51L_checklstring(lua_State *L, int numArg, size_t *l);
	const char *lua51L_optlstring(lua_State *L, int numArg, const char *def, size_t *l);
	lua_Number lua51L_checknumber(lua_State *L, int numArg);
	lua_Number lua51L_optnumber(lua_State *L, int nArg, lua_Number def);
	lua_Integer lua51L_checkinteger(lua_State *L, int numArg);
	lua_Integer lua51L_optinteger(lua_State *L, int nArg, lua_Integer def);
	void lua51L_checkstack(lua_State *L, int sz, const char *msg);
	void lua51L_checktype(lua_State *L, int narg, int t);
	void lua51L_checkany(lua_State *L, int narg);
	int lua51L_newmetatable(lua_State *L, const char *tname);
	void *lua51L_checkudata(lua_State *L, int ud, const char *tname);
	void lua51L_where(lua_State *L, int lvl);
	int lua51L_error(lua_State *L, const char *fmt, ...);
	int lua51L_checkoption(lua_State *L, int narg, const char *def, const char *const lst[]);
	int lua51L_ref(lua_State *L, int t);
	void lua51L_unref(lua_State *L, int t, int ref);
	int lua51L_loadfile(lua_State *L, const char *filename);
	int lua51L_loadbuffer(lua_State *L, const char *buff, size_t sz, const char *name);
	int lua51L_loadstring(lua_State *L, const char *s);
	lua_State *lua51L_newstate(void);
	const char *lua51L_gsub(lua_State *L, const char *s, const char *p, const char *r);
	const char *lua51L_findtable(lua_State *L, int idx, const char *fname, int szhint);

	/*
	** Buffer functions
	*/
	void lua51L_buffinit(lua_State *L, lua51L_Buffer *B);
	char *lua51L_prepbuffer(lua51L_Buffer *B);
	void lua51L_addlstring(lua51L_Buffer *B, const char *s, size_t l);
	void lua51L_addstring(lua51L_Buffer *B, const char *s);
	void lua51L_addvalue(lua51L_Buffer *B);
	void lua51L_pushresult(lua51L_Buffer *B);

	/*
	** Standard library open functions
	*/
	int lua51open_base(lua_State *L);
	int lua51open_table(lua_State *L);
	int lua51open_io(lua_State *L);
	int lua51open_os(lua_State *L);
	int lua51open_string(lua_State *L);
	int lua51open_math(lua_State *L);
	int lua51open_debug(lua_State *L);
	int lua51open_package(lua_State *L);
	void lua51L_openlibs(lua_State *L);

#ifdef __cplusplus
}
#endif

/*
** Convenience macros (auxiliary library)
*/
#define lua51L_getn(L, i) ((int)lua51_objlen(L, i))
#define lua51L_setn(L, i, j) ((void)0)

#define lua51L_argcheck(L, cond, numarg, extramsg) ((void)((cond) || lua51L_argerror(L, (numarg), (extramsg))))
#define lua51L_checkstring(L, n) (lua51L_checklstring(L, (n), NULL))
#define lua51L_optstring(L, n, d) (lua51L_optlstring(L, (n), (d), NULL))
#define lua51L_checkint(L, n) ((int)lua51L_checkinteger(L, (n)))
#define lua51L_optint(L, n, d) ((int)lua51L_optinteger(L, (n), (d)))
#define lua51L_checklong(L, n) ((long)lua51L_checkinteger(L, (n)))
#define lua51L_optlong(L, n, d) ((long)lua51L_optinteger(L, (n), (d)))
#define lua51L_typename(L, i) lua51_typename(L, lua51_type(L, (i)))
#define lua51L_dofile(L, fn) (lua51L_loadfile(L, fn) || lua51_pcall(L, 0, LUA_MULTRET, 0))
#define lua51L_dostring(L, s) (lua51L_loadstring(L, s) || lua51_pcall(L, 0, LUA_MULTRET, 0))
#define lua51L_getmetatable(L, n) (lua51_getfield(L, LUA51_REGISTRYINDEX, (n)))
#define lua51L_opt(L, f, n, d) (lua51_isnoneornil(L, (n)) ? (d) : f(L, (n)))

#define lua51L_addchar(B, c)                                                                                           \
	((void)((B)->p < ((B)->buffer + LUA51L_BUFFERSIZE) || lua51L_prepbuffer(B)), (*(B)->p++ = (char)(c)))
#define lua51L_putchar(B, c) lua51L_addchar(B, c)
#define lua51L_addsize(B, n) ((B)->p += (n))

#define lua51_ref(L, lock)                                                                                             \
	((lock) ? lua51L_ref(L, LUA51_REGISTRYINDEX)                                                                       \
	        : (lua51_pushstring(L, "unlocked references are obsolete"), lua51_error(L), 0))
#define lua51_unref(L, ref) lua51L_unref(L, LUA51_REGISTRYINDEX, (ref))
#define lua51_getref(L, ref) lua51_rawgeti(L, LUA51_REGISTRYINDEX, (ref))

#define lua51_assert(x) ((void)0)

/*
** Init function (no-op — Lua 5.1.2 is statically linked)
*/
bool Lua51_Load(const wchar_t *sDll = 0);

#endif /* LUA51_H_ */
