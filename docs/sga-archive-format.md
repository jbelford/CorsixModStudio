# SGA Archive Format

The SGA (Stored Game Archive) is the primary archive format used by Relic Entertainment in
Dawn of War (v2) and Company of Heroes (v4). The Rainman library provides both reading
(`CSgaFile`) and writing (`CSgaCreator`).

## File Layout

```
┌──────────────────────────────────────┐
│  File Header (180 bytes v2 / 184 v4) │
├──────────────────────────────────────┤
│  Data Header (variable)              │
│    ├─ Header Info (32 bytes)         │
│    ├─ Table of Contents entries      │
│    ├─ Directory entries              │
│    ├─ File entries                   │
│    └─ String pool                    │
├──────────────────────────────────────┤
│  File Data (variable)                │
│    ├─ Per-file metadata              │
│    └─ Raw / zlib-compressed data     │
└──────────────────────────────────────┘
```

## File Header

| Offset | Size | Field | Description |
|--------|------|-------|-------------|
| 0 | 8 | Signature | `"_ARCHIVE"` (ASCII) |
| 8 | 4 | Version | `2` = DoW, `4` = CoH |
| 12 | 16 | Tool MD5 | MD5 hash (keyed with `"E01519D6-2DB7-4640-AF54-0A23319C56C3"`) |
| 28 | 128 | Archive Name | Unicode string (64 × `wchar_t`) |
| 156 | 16 | Header MD5 | MD5 hash of the data header (keyed with `"DFC9AF62-FC1B-4180-BC27-11CCE87D3EFF"`) |
| 172 | 4 | Data Header Size | Size of the data header in bytes |
| 176 | 4 | Data Offset | Byte offset to the start of file data |
| 180 | 4 | Platform | **v4 only.** `1` = win32/x86 little-endian |

## MD5 Validation

Two keyed-MD5 hashes protect the archive integrity:

1. **Header MD5** — computed over the entire data header buffer using a fixed key string.
   Always validated on load.

   ```
   MD5InitKey(&ctx, "DFC9AF62-FC1B-4180-BC27-11CCE87D3EFF");
   MD5Update(&ctx, pDataHeader, iDataHeaderSize);
   MD5Final(digest, &ctx);
   ```

2. **Tool MD5** — computed from the data offset to end-of-file in 1 MB chunks using a
   different key. Not validated on load because it requires reading the entire archive
   (potentially gigabytes).

   ```
   Key: "E01519D6-2DB7-4640-AF54-0A23319C56C3"
   ```

The `MD5InitKey()` function is equivalent to `MD5Init()` followed by
`MD5Update(ctx, key, strlen(key))`.

## Data Header

The data header is loaded as a single contiguous block of `iDataHeaderSize` bytes. All
internal offsets (for ToC entries, directories, files, and strings) are relative to the
start of this block.

### Header Info (32 bytes)

| Offset | Size | Field | Description |
|--------|------|-------|-------------|
| 0 | 4 | iToCOffset | Offset to Table of Contents entries |
| 4 | 2 | iToCCount | Number of ToC entries |
| 6 | 4 | iDirOffset | Offset to directory entries |
| 10 | 2 | iDirCount | Number of directories |
| 12 | 4 | iFileOffset | Offset to file entries |
| 16 | 2 | iFileCount | Number of files |
| 18 | 4 | iItemOffset | Offset to string pool |
| 22 | 2 | iItemCount | Number of strings |

### Table of Contents Entry (138 bytes each)

| Offset | Size | Field |
|--------|------|-------|
| 0 | 64 | Alias (null-padded ASCII) |
| 64 | 64 | Base directory name |
| 128 | 2 | Start directory index |
| 130 | 2 | End directory index |
| 132 | 2 | Start file index |
| 134 | 2 | End file index |
| 136 | 4 | Folder offset (unknown purpose) |

Most archives have a single ToC entry with alias `"Data"`.

### Directory Entry (12 bytes each)

| Offset | Size | Field |
|--------|------|-------|
| 0 | 4 | Name offset (into string pool) |
| 4 | 2 | First subdirectory index |
| 6 | 2 | Last subdirectory index (exclusive) |
| 8 | 2 | First file index |
| 10 | 2 | Last file index (exclusive) |

### File Entry — v2 (20 bytes each)

| Offset | Size | Field |
|--------|------|-------|
| 0 | 4 | Name offset (into string pool) |
| 4 | 4 | Flags (`0x00` = uncompressed, `0x10` = zlib large, `0x20` = zlib small < 4 KB) |
| 8 | 4 | Data offset (relative to file data section) |
| 12 | 4 | Compressed length |
| 16 | 4 | Uncompressed length |

### File Entry — v4 (22 bytes each)

| Offset | Size | Field |
|--------|------|-------|
| 0 | 4 | Name offset |
| 4 | 4 | Data offset |
| 8 | 4 | Compressed length |
| 12 | 4 | Uncompressed length |
| 16 | 4 | Modification time |
| 20 | 2 | Flags (`0x100` = compressed) |

## Directory Hash Map

For fast file path → directory ID lookups, the loader builds a sorted hash map during
`Load()`.

### Construction

1. For each directory, compute a **case-insensitive CRC32** of the full path using
   `crc32_case_idt()` (see [Hash Algorithms](hash-algorithms.md)).
2. Store `(CRC, directoryID)` pairs.
3. Sort the array by CRC value using `qsort()`.

### Lookup (in VOpenStream)

1. Split the requested path into directory path + filename.
2. Hash the directory path with `crc32_case_idt()`.
3. **Binary search** the sorted hash map for the CRC.
4. Linear scan the directory's file range to find the filename.

This gives O(log n) directory resolution instead of O(n) string comparisons.

## File Decompression (VOpenStream)

When a file inside the archive is opened:

1. **Seek** to the file's data offset (adjusted by pre-data metadata size).
2. **Read pre-data fields:**
   - 256 bytes: filename (v4)
   - 4 bytes: modification date (v2 only)
   - 4 bytes: CRC32 of compressed data
3. **Read** compressed data into a buffer.
4. **Decompress** using zlib `uncompress()` if compressed length ≠ uncompressed length.
5. **Wrap** the decompressed buffer in a `CMemoryStore` stream and return it.

## Archive Creation (CSgaCreator)

### Algorithm

1. **Scan** the input directory tree recursively via `_ScanDirectory()`.
2. **Sort** subdirectories and files alphabetically at each level.
3. **Count** total directories and files for header allocation.
4. **Write file data** sequentially:
   - Compress each file with `compress2(Z_BEST_COMPRESSION)`.
   - If compressed ≥ uncompressed, store uncompressed (flags = `0x00`).
   - v2: files < 4096 bytes get flag `0x20`; larger files get `0x10`.
   - v4: compressed files get flag `0x100`.
5. **Build data header** with ToC, directory entries, file entries, and string pool.
6. **Compute MD5 hashes** and write the file header.

### Source Files

- `CSgaFile.h` / `CSgaFile.cpp` — Archive reader
- `CSgaCreator.h` / `CSgaCreator.cpp` — Archive writer
