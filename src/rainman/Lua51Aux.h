/*
** Lua 5.1.2 auxiliary library + standard libraries header.
** Declares lua51L_* and lua51open_* functions that link to renamed
** symbols in lua512.lib.  No rename macros — safe to include alongside
** Lua 5.0.2 headers.
**
** Requires Lua51.h to be included first (for lua_State, lua_CFunction,
** lua_Integer, lua_Number, etc.).
*/

#ifndef LUA51AUX_H_
#define LUA51AUX_H_

#include <stddef.h>
#include <stdio.h>

/* extra error code for luaL_load */
#define LUA51_ERRFILE 6

typedef struct lua51L_Reg
{
	const char *name;
	lua_CFunction func;
} lua51L_Reg;

#define lua51I_openlib lua51L_openlib

/*
** Buffer functions — uses BUFSIZ from <stdio.h>
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
** Convenience macros
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

#endif /* LUA51AUX_H_ */
