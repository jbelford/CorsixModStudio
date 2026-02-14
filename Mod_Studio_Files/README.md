# Mod_Studio_Files — Runtime Data Files

CorsixModStudio looks for optional data files in this directory at startup.
These come from the original Corsix's Mod Studio 0.5.5 distribution.

Note: `squish.dll` and `lua512.dll` are no longer needed — libsquish and
Lua 5.1.2 are now statically linked into the rainman library.

## Optional data files

| File / Directory | Purpose |
|------------------|---------|
| `lua.dat` | Lua API reference data |
| `bfx.dat` | BFX map data |
| `scardoc.dat` | SCAR documentation (Dawn of War) |
| `scardoc_coh.dat` | SCAR documentation (Company of Heroes) |
| `dictionaries\` | Spell-check dictionaries |
| `rgd_macros\` | RGD macro definitions |
| `docs\` | Built-in help files |

## Where to get these files

Extract them from the original Corsix's Mod Studio 0.5.5 installer or
distribution archive.
