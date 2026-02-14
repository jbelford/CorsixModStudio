# Relic Format Algorithms

Documentation for the file format handling algorithms in the Rainman library.

## Format Documentation

| Document | Formats | Key Algorithms |
|----------|---------|----------------|
| [SGA Archive Format](sga-archive-format.md) | `.sga` | Zlib compression, keyed MD5 validation, CRC32 directory hash map with binary search |
| [Relic Chunky Container](relic-chunky-format.md) | Chunky wrapper | Recursive chunk parsing (FOLD/DATA tree) |
| [RGD Game Data Format](rgd-game-data-format.md) | `.rgd` | Binary table serialisation, Jenkins hash property names, Lua state conversion |
| [Data Inheritance System](data-inheritance-system.md) | Inheritance tree | CRC32 path hashing, three-way differential merge, `Inherit()`/`Reference()` resolution |
| [UCS Localisation Format](ucs-localisation-format.md) | `.ucs` | UTF-16LE parsing, dollar-string resolution |
| [Module File Format](module-file-format.md) | `.module` | INI-style directive parsing, dynamic placeholder expansion, recursive mod loading |
| [Texture, Material & Script Formats](texture-material-script-formats.md) | `.rgt`, `.rgm`, BFX | DXT compression, mip-map management, material variable encoding, Lua serialisation |
| [Hash Algorithms](hash-algorithms.md) | — | Case-insensitive CRC32, Bob Jenkins lookup hash, keyed MD5 |
