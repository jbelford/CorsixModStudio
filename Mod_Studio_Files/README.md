# Mod_Studio_Files — Runtime Dependencies

CorsixModStudio looks for optional DLLs and data files in this directory
at startup. These are **not** built from source — they come from the
original Corsix's Mod Studio 0.5.5 distribution.

## Required files

| File | Purpose | Impact if missing |
|------|---------|-------------------|
| `lua512.dll` | Lua 5.1.2 runtime (release build) | SCAR script editor features reduced |
| `debug\lua512d.dll` | Lua 5.1.2 runtime (debug build) | SCAR script editor features reduced |

Note: `squish.dll` is no longer needed — libsquish is now statically linked into the rainman library.

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
distribution archive. The `squish.dll` can also be rebuilt from the
[squish source](https://sourceforge.net/projects/libsquish/) (must match
the `CompressImage`/`DecompressImage` function signatures expected by
`CRgtFile`).
