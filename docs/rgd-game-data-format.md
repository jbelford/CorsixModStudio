# RGD (Relic Game Data) Format

RGD files store game configuration data as binary-serialised key/value trees. They are
wrapped in the [Relic Chunky](relic-chunky-format.md) container format and use the
`DATAAEGD` chunk tag. The Rainman library implements reading, writing, and Lua conversion
in `CRgdFile`.

## Chunky Wrapper

An RGD file is a Chunky file containing one or more `DATAAEGD` chunks. Each chunk has:

| Field | Size | Description |
|-------|------|-------------|
| Descriptor | variable | Human-readable description string |
| Unknown1, Unknown2 | 4 each | v3 chunky only |
| CRC32 | 4 | CRC32 checksum of the data payload |
| Data Length | 4 | Byte length of the binary RGD data |
| Data | variable | Binary RGD table (see below) |

## Binary Table Format

The binary data inside a `DATAAEGD` chunk encodes a recursive key/value tree.

### Table Header

```
[0..3]  uint32  keyCount    — number of entries in this table
```

Immediately after the key count come `keyCount` key descriptors (12 bytes each), followed
by the data section containing the actual values.

### Key Descriptor (12 bytes each)

| Offset | Size | Field | Description |
|--------|------|-------|-------------|
| 0 | 4 | Hash | Jenkins hash of the property name |
| 4 | 4 | Type | Data type code (see below) |
| 8 | 4 | Offset | Byte offset into the data section |

### Data Types

| Code | Name | Binary Size | Alignment | Description |
|------|------|-------------|-----------|-------------|
| `0x00` | DT_NoData | 0 | — | Empty / nil marker |
| `0x01` | DT_Bool | 1 | 1 | `0x00` = false, `0x01` = true |
| `0x02` | DT_Float | 4 | 4 | IEEE 754 32-bit float |
| `0x03` | DT_Integer | 4 | 4 | Unsigned 32-bit integer |
| `0x04` | DT_String | variable | 1 | Null-terminated ASCII string |
| `0x05` | DT_WString | variable | 2 | Null-terminated UTF-16LE string |
| `0x06` | DT_Table | variable | 4 | Nested table (recursive) |
| `0x65` (101) | TableInt | variable | 4 | Numerically-keyed table variant |

### Alignment

Values in the data section are padded to their natural alignment boundary:

```
paddingNeeded = (alignment - (currentOffset % alignment)) % alignment
```

### Recursive Deserialization

```
function ProcessRawRgdData(stream, entry):
    keyCount = read uint32
    dataOffset = currentPosition + (keyCount × 12)

    for i = 0 to keyCount-1:
        hash   = read uint32
        type   = read uint32
        offset = read uint32

        seek to (dataOffset + offset)

        switch type:
            case DT_Bool:    read 1 byte
            case DT_Float:   read 4 bytes as float
            case DT_Integer: read 4 bytes as uint32
            case DT_String:  read bytes until '\0'
            case DT_WString: read wchar_t until L'\0'
            case DT_Table:   recursively call ProcessRawRgdData()

        store (hash, type, value) in entry
```

## Property Name Hashing

Property names are not stored in the binary format — only their hashes appear. The hash
function is Bob Jenkins' lookup hash (see [Hash Algorithms](hash-algorithms.md)). A
separate dictionary file (`#RGD_DIC` format) maps hashes back to human-readable names
via `CRgdHashTable`.

## RGD ↔ Lua Conversion

### RGD → Lua State (`CreateLuaState`)

1. Create a new Lua 5.0.2 state.
2. Load safe standard libraries (base, string, math, table).
3. **Remove unsafe globals:** `coroutine`, `dofile`, `getfenv`, `setfenv`,
   `getmetatable`, `setmetatable`, `loadlib`, `loadfile`, `print`, `rawget`, `rawset`,
   `require`.
4. Create a global table `GameData`.
5. Recursively populate it from the RGD entry tree:
   - Float → `lua_pushnumber`
   - Bool → `lua_pushboolean`
   - String → `lua_pushstring`
   - Table → `lua_newtable` + recurse
   - Unresolved hashes → key formatted as `"0xHHHHHHHH"`

### Lua → RGD (`Load(CLuaFile*)`)

Traverses a Lua state's `GameData` table and builds the equivalent `_RgdEntry` tree.

## RGD Hash Dictionary (`CRgdHashTable`)

The dictionary file format (`#RGD_DIC`) maps Jenkins hashes to property names:

```
#RGD_DIC
0x12345678=property_name
0xABCDEF01=another_property
# This is a comment
```

**Rules:**
- First line must be `#RGD_DIC`.
- `#` starts a comment (rest of line ignored).
- Format: `0xHEX=token` (hex is case-insensitive).
- Whitespace in codes and tokens is ignored.
- Duplicate entries should be avoided.

When `HashToValue()` encounters an unknown hash, it records it. The application can later
call `FillUnknownList()` to retrieve all unresolved hashes.

## Source Files

- `CRgdFile.h` / `CRgdFile.cpp` — RGD parser and serializer
- `CRgdHashTable.h` / `CRgdHashTable.cpp` — Hash ↔ name dictionary
- `CLuaFromRgd.h` / `CLuaFromRgd.cpp` — RGD → Lua file conversion
