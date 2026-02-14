# UCS Localisation Format

UCS (Unicode Character String) files provide localised text for Dawn of War and Company of
Heroes. Each file maps numeric IDs to Unicode strings. The Rainman library supports the
DoW format (`CUcsFile`) and has a stub for the CoH variant (`CCohUcsFile`).

## DoW UCS File Format

### Binary Layout

```
Offset  Size   Field
0       2      BOM: 0xFEFF (UTF-16 LE byte order mark)
2       ...    Entries (UTF-16 LE text lines)
```

Each entry is a UTF-16LE text line:

```
[StringID]\t[Value]\r\n
```

- **StringID** — decimal digits forming the numeric identifier.
- **Separator** — tab character (`0x0009`) or space (`0x0020`).
- **Value** — the localised Unicode string.
- **Line ending** — `\r\n` (CR LF).

### Load Algorithm

```
function Load(stream):
    Read 2 bytes → validate 0xFEFF BOM
    for each wide-character line until EOF:
        Parse leading decimal digits → iID
        Skip separator character
        Copy remaining characters → sValue
        Strip trailing \r and \n
        Store m_mapValues[iID] = sValue
        (silently ignore duplicate IDs)
```

### Save Algorithm

```
function Save(filename):
    Write 0xFEFF BOM
    for each (iID, sValue) in m_mapValues:
        Write iID as wide decimal string
        Write 0x0009 (tab)
        Write sValue as wide string
        Write 0x000D 0x000A (CR LF)
```

### DAT Variant (`LoadDat`)

An ASCII variant of the same format. Parses single-byte lines and converts strings to
wide characters.

## Dollar-String Resolution

Game data files reference localised strings using **dollar-strings** of the form `$12345`.

### Detection (`IsDollarString`)

```
function IsDollarString(s):
    if s[0] != '$': return false
    for each character after '$':
        if not digit: return false
    return true
```

Overloaded for both `char*` and `wchar_t*`.

### Resolution

`CModuleFile::ResolveUCS()` extracts the numeric ID from the dollar-string and looks it
up across all loaded UCS files until a match is found.

## CoH UCS Variant (`CCohUcsFile`)

The Company of Heroes UCS format extends the DoW format with:

- Comment support (`//` prefix)
- `filerange begin end` directives to define ID ranges
- Per-range metadata and descriptions

> **Note:** The CoH implementation is currently commented out in the codebase and not
> functional.

## Data Structure

Both variants store strings in a `std::map<unsigned long, wchar_t*>`, providing O(log n)
lookup by string ID.

## Source Files

- `CUcsFile.h` / `CUcsFile.cpp` — DoW UCS reader/writer
- `CCohUcsFile.h` / `CCohUcsFile.cpp` — CoH UCS (disabled)
