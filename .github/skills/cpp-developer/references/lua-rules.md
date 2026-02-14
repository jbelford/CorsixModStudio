# Vendored Lua Rules

## DO NOT MODIFY Lua 5.0.2

The directory `src/rainman/lua502/` contains a vendored copy of Lua 5.0.2 with Corsix's patches to `ltm.c`, `ltm.h`, `lvm.c`, and `lvm.h`. These patches are required for game data compatibility.

**Rules:**
- Do NOT upgrade Lua 5.0.2 to a newer version.
- Do NOT modify any files in `lua502/` unless specifically fixing a bug in the patched files.
- Do NOT add new files to `lua502/`.
- The Lua 5.0.2 library is built as a separate static library (`lua502` target) and linked into Rainman.

## Lua 5.1.2 Runtime Loading

Lua 5.1.2 is loaded at runtime via `LoadLibraryW()` / `GetProcAddress()` (see `Lua51.cpp` and `Lua51.h`). Only type headers exist in `src/rainman/lua512/` — there is no compiled Lua 5.1.2 in this project.

**Rules:**
- Do NOT add Lua 5.1.2 source files to compile.
- Function pointers for Lua 5.1.2 are resolved at runtime; see `Lua51.cpp` for the pattern.
- If adding new Lua 5.1.2 API usage, add the function pointer declaration in `Lua51.h` and the `GetProcAddress` call in `Lua51.cpp`.

## Lua Integration in Rainman

Rainman uses Lua for:
- `CInheritTable` — resolves data inheritance in Lua-based game files.
- `CLuaFile` / `CLuaFile2` — parses Lua data files.
- `CLuaScript` — executes Lua scripts for modding operations.

When writing code that interacts with Lua:
- Use the Lua 5.0.2 C API (`lua_State*`, `lua_pushstring`, etc.).
- Include headers from `lua502/` (e.g., `extern "C" { #include "lua.h" }`).
- Handle Lua errors via `lua_pcall` return codes, then translate to `CRainmanException*`.
