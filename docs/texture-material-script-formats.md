# Texture, Material, and Script Formats

The Rainman library handles several secondary Relic file formats for textures, materials,
and Lua script serialisation. All are wrapped in the
[Relic Chunky](relic-chunky-format.md) container.

## RGT — Relic Texture (`CRgtFile`)

### Purpose

RGT files store textures with optional DXT compression and mip-map levels.

### Chunky Structure

```
FOLDFBIF     — File burn info
FOLDTSET     — Texture set
  └─ FOLDTXTR  — Texture
       ├─ DATATMAN  — Mip-level metadata
       ├─ DATATDAT  — Compressed texture data
       └─ DATATFMT  — Texture format descriptor
```

### Image Formats

| Format | Description |
|--------|-------------|
| `IF_Tga` | Uncompressed TGA (RGBA) |
| `IF_Dxtc` | DXT-compressed (DXT1, DXT3, or DXT5) |

### Algorithms

**Mip-map storage:** Mip levels are stored in a `std::map<long, _MipLevel*>`. RGT files
store levels smallest-to-largest (reversed from DDS ordering).

**DXT codec:** Pluggable via function pointer (`tfnDXTCodec`). Defaults to the Squish
library for DXT1/3/5 compression and decompression.

**Data compression:** Individual mip-level data blocks are zlib-compressed using
`compress2(Z_BEST_COMPRESSION)`.

**TMAN chunk layout:**

| Field | Size | Description |
|-------|------|-------------|
| Mip count | 4 | Number of mip levels |
| Per level: uncompressed size | 4 | Raw texture data size |
| Per level: compressed size | 4 | Zlib-compressed size |

### Source Files

`CRgtFile.h` / `CRgtFile.cpp`

---

## RGM — Relic Material (`CRgmFile`)

### Purpose

RGM files define material properties: shader assignments, numeric parameters, and texture
references.

### Chunky Structure

```
FOLDMTRL     — Material (descriptor = material name)
  ├─ DATAINFO  — DirectX shader name
  ├─ DATA0VAR  — Variable definitions (repeated)
  └─ FOLDTSET  — Texture references
```

### Variable Types

| Code | Name | Description |
|------|------|-------------|
| 1 | `VT_Number` | Single float |
| 5 | `VT_NumberArray` | Array of floats |
| 9 | `VT_Text` | Text string |

### Texture Path Validation (on save)

Material texture paths are validated with these rules:
- Allowed characters: `a-z`, `A-Z`, `0-9`, `_`, space, `/`
- Cannot start or end with `\`
- No double backslashes

Unused texture references (TSET chunks) are pruned during save.

### Source Files

`CRgmFile.h` / `CRgmFile.cpp`

---

## BFX — Lua Script Serialisation (`CBfxFile`)

### Purpose

BFX is a Lua text output format for RGD data. It extends `CRgdFile` and recursively
serialises the RGD entry tree as valid Lua source.

### Output Algorithm (`_SaveRaw`)

```
function SaveRaw(entry, output, indent):
    if entry is table:
        if all keys are numeric digits:
            sort entries numerically
        output "{"
        for each child:
            SaveRaw(child, output, indent + 1)
        output "}"
    else:
        output key = value
```

### Type Formatting

| Type | Lua Output |
|------|------------|
| Float | `%.5g` format with manual decimal placement via `_fcvt()` |
| Integer | Decimal number |
| Bool | `true` / `false` |
| String | Double-quoted with escaped `\`, `'`, `"` |
| WString | Double-quoted wide string |
| Table | `{ ... }` with nested indentation |

### Source Files

`CBfxFile.h` / `CBfxFile.cpp`

---

## File Store Abstraction (`IFileStore`)

### Interface

All file access in Rainman goes through the `IFileStore` interface:

| Method | Description |
|--------|-------------|
| `VInit(param)` | Initialise the store (param meaning varies by implementation) |
| `VOpenStream(path)` | Open a read-only stream (caller owns the returned pointer) |
| `VOpenOutputStream(path)` | Open a writable stream |

### Stream Interfaces

**`IStream` (read-only):**
- `VRead(count, size, dest)` — read items
- `VSeek(offset, origin)` — seek (`SL_Current`, `SL_Root`, `SL_End`)
- `VTell()` — current position

**`IOutputStream` (extends IStream):**
- `VWrite(count, size, src)` — write items

### Implementations

| Class | Backend |
|-------|---------|
| `CFileSystemStore` | Host filesystem (`fopen`/`_wfopen`) |
| `CMemoryStore` | In-memory buffers |
| `CSgaFile` | SGA archive (read-only) |
| `CModuleFile` | Composite virtual filesystem |

---

## Memory Store (`CMemoryStore`)

### MemoryRange Pattern

`MemoryRange(pointer, size)` wraps a raw buffer as a pseudo file path that `VOpenStream()`
understands:

```
function MemoryRange(data, length):
    Allocate _MemRange descriptor
    descriptor.pointer = data
    descriptor.length = length
    return (char*)descriptor    // cast for VOpenStream compatibility
```

### Dynamic Output Buffer (`COutStream`)

- **Initial size:** 128 KB (131,072 bytes)
- **Growth:** Doubles the buffer when full (`capacity <<= 1`)
- **Pointer relocation:** After realloc, the write cursor is recalculated:
  `newCursor = newBuffer + (oldCursor - oldBuffer)`

### Source Files

- `IFileStore.h` / `IFileStore.cpp`
- `CMemoryStore.h` / `CMemoryStore.cpp`
- `CFileSystemStore.h` / `CFileSystemStore.cpp`
