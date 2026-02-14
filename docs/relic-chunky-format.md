# Relic Chunky Container Format

The Relic Chunky format is a generic hierarchical container used by most Relic file types
(RGD, RGT, RGM, etc.). It organises data into a tree of typed chunks. The Rainman library
implements it in `CChunkyFile`.

## File Header

| Offset | Size | Field | Description |
|--------|------|-------|-------------|
| 0 | 16 | Signature | `"Relic Chunky\x0D\x0A\x1A\x00"` |
| 16 | 4 | Version | `1` (DoW) or `3` (CoH) |
| 20 | 4 | Unknown1 | Always `1` |
| 24 | 4 | Unknown2 | v3 only |
| 28 | 4 | Unknown3 | v3 only |
| 32 | 4 | Unknown4 | v3 only |

## Chunk Structure

After the file header, chunks appear sequentially. Each chunk has:

| Offset | Size | Field | Description |
|--------|------|-------|-------------|
| 0 | 4 | Type | `"FOLD"` (folder) or `"DATA"` (data) |
| 4 | 4 | Name | 4-character chunk identifier (e.g. `"AEGD"`) |
| 8 | 4 | Version | Chunk version number |
| 12 | 4 | Data Length | Byte length of payload |
| 16 | 4 | Descriptor Length | Byte length of descriptor string |
| 20 | 4 | Unknown1 | v3 only |
| 24 | 4 | Unknown2 | v3 only |
| 20/28 | N | Descriptor | Descriptor string (N = descriptor length) |
| varies | M | Payload | M = data length (see below) |

The combined type + name gives the 8-character chunk tag. For example, `"DATAAEGD"` is a
data chunk named `AEGD` (used by RGD files).

## Recursive Parsing Algorithm

```
function LoadChunk(stream, chunkyVersion):
    Read 4 bytes → chunkType ("FOLD" or "DATA")
    Read 4 bytes → chunkName
    Read 4 bytes → chunkVersion
    Read 4 bytes → dataLength
    Read 4 bytes → descriptorLength
    if chunkyVersion >= 3:
        Read 4 bytes → unknown1
        Read 4 bytes → unknown2
    Read descriptorLength bytes → descriptor

    if chunkType == "FOLD":
        endPos = currentPosition + dataLength
        while currentPosition < endPos:
            child = LoadChunk(stream, chunkyVersion)
            add child to children list
    else:  # "DATA"
        Read dataLength bytes → raw data buffer

    return chunk
```

The top-level file may contain multiple root chunks. `CChunkyFile` stores them in a
`std::vector<CChunk*>`.

## Size Calculation

When saving, folder chunks must know their total payload size before writing the header.
`_FoldUpdateSize()` recursively computes this:

```
function FoldUpdateSize(chunk):
    if chunk is DATA:
        return headerSize + descriptorLength + dataLength
    total = 0
    for each child:
        total += FoldUpdateSize(child)
    chunk.dataLength = total
    return headerSize + descriptorLength + total
```

Header size is 20 bytes (v1) or 28 bytes (v3) plus the descriptor.

## Usage by Other Formats

| Chunk Tag | Used By | Purpose |
|-----------|---------|---------|
| `DATAAEGD` | RGD files | Game data tables |
| `FOLDTSET` | RGT files | Texture set container |
| `DATATMAN` | RGT files | Mip-level metadata |
| `DATATDAT` | RGT files | Compressed texture data |
| `DATATFMT` | RGT files | Texture format info |
| `FOLDMTRL` | RGM files | Material container |
| `DATAINFO` | RGM files | Shader name |
| `DATA0VAR` | RGM files | Material variable |

## Source Files

- `CChunkyFile.h` / `CChunkyFile.cpp`
