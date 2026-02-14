# Hash Algorithms

The Rainman library uses three distinct hash algorithms for different purposes in Relic
game format handling.

## Case-Insensitive CRC32 (`crc32_case_idt`)

**Used for:** SGA archive directory path lookups.

A modified CRC32 that forces case-insensitive matching by OR-ing each input byte with
`0x20` before hashing. This maps ASCII uppercase letters to their lowercase equivalents
(and is harmless for digits and most punctuation).

### Algorithm

```
Polynomial: 0xEDB88320 (standard CRC32, reflected)

function crc32_case_idt(crc, buffer, length):
    if buffer is null: return 0

    build lookup table (256 entries, standard CRC32 polynomial)

    crc = crc XOR 0xFFFFFFFF

    // Unrolled loop — 8 bytes per iteration for performance
    while length >= 8:
        for i = 0 to 7:
            byte = buffer[i] OR 0x20          ← case-insensitive step
            crc = table[(crc XOR byte) AND 0xFF] XOR (crc >> 8)
        buffer += 8
        length -= 8

    // Handle remaining bytes
    while length > 0:
        byte = *buffer OR 0x20
        crc = table[(crc XOR byte) AND 0xFF] XOR (crc >> 8)
        buffer += 1
        length -= 1

    return crc XOR 0xFFFFFFFF
```

The lookup table is lazily initialized on first use. The unrolled loop processes 8 bytes
per iteration to reduce branch overhead.

### Source

`crc32_case_idt.c` / `crc32_case_idt.h`

---

## Bob Jenkins Hash (`hash`)

**Used for:** RGD property name hashing. This is the hash that appears in binary RGD
files as the 32-bit identifier for each key.

Bob Jenkins' 1996 lookup hash. Every bit of the key affects every bit of the output.
Achieves avalanche for all 1-bit and 2-bit deltas.

### Mix Function

The core mixing step combines three 32-bit values reversibly using shifts and XORs:

```c
#define mix(a,b,c)
{
  a -= b; a -= c; a ^= (c >> 13);
  b -= c; b -= a; b ^= (a <<  8);
  c -= a; c -= b; c ^= (b >> 13);
  a -= b; a -= c; a ^= (c >> 12);
  b -= c; b -= a; b ^= (a << 16);
  c -= a; c -= b; c ^= (b >>  5);
  a -= b; a -= c; a ^= (c >>  3);
  b -= c; b -= a; b ^= (a << 10);
  c -= a; c -= b; c ^= (b >> 15);
}
```

### Algorithm

```
function hash(key, length, initval):
    a = b = 0x9E3779B9      // golden ratio
    c = initval              // previous hash or arbitrary seed

    // Process 12 bytes at a time
    while length >= 12:
        a += key[0..3]  as little-endian uint32
        b += key[4..7]  as little-endian uint32
        c += key[8..11] as little-endian uint32
        mix(a, b, c)
        key += 12
        length -= 12

    // Handle last 0–11 bytes (fall-through switch)
    c += original_length
    switch(length):
        case 11: c += key[10] << 24
        case 10: c += key[9]  << 16
        case 9:  c += key[8]  <<  8
        case 8:  b += key[7]  << 24
        case 7:  b += key[6]  << 16
        case 6:  b += key[5]  <<  8
        case 5:  b += key[4]
        case 4:  a += key[3]  << 24
        case 3:  a += key[2]  << 16
        case 2:  a += key[1]  <<  8
        case 1:  a += key[0]

    mix(a, b, c)
    return c
```

### Usage in Rainman

`CRgdHashTable::ValueToHashStatic()` calls `hash()` with `initval = 0` to compute the
hash of an RGD property name string.

### Performance

About `36 + 6×len` single-cycle instructions. Designed for hash table lookups where one
collision in 2³² is acceptable. **Not suitable for cryptographic use.**

### Source

`hash.c` (public domain, by Bob Jenkins, December 1996)

---

## MD5 (Message Digest 5)

**Used for:** SGA archive integrity verification.

Colin Plumb's 1993 MD5 implementation, extended with `MD5InitKey()` for keyed hashing.

### API

| Function | Description |
|----------|-------------|
| `MD5Init(ctx)` | Initialize a new MD5 context |
| `MD5InitKey(ctx, key)` | Initialize and immediately feed the key string |
| `MD5Update(ctx, data, len)` | Append data to the running hash |
| `MD5Final(digest, ctx)` | Finalize and output 16-byte digest |
| `MD5Transform(buf, in)` | Core block transform (16 × 32-bit words → 4 × 32-bit state) |

### Keyed Hashing in SGA

The SGA format uses `MD5InitKey()` to create keyed digests. This is equivalent to
prepending the key to the data:

```c
MD5InitKey(&ctx, "DFC9AF62-FC1B-4180-BC27-11CCE87D3EFF");
// is the same as:
MD5Init(&ctx);
MD5Update(&ctx, key, strlen(key));
```

Two fixed key strings are used:

| Key | Purpose |
|-----|---------|
| `"DFC9AF62-FC1B-4180-BC27-11CCE87D3EFF"` | Header MD5 — validates data header integrity |
| `"E01519D6-2DB7-4640-AF54-0A23319C56C3"` | Tool MD5 — validates file data integrity |

### Source

`md5.c` / `md5.h` (public domain, by Colin Plumb, 1993)
