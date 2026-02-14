# Data Inheritance System

The Rainman library implements a Lua-based data inheritance system that allows game data
files to derive from parent files, overriding only the properties that differ. This is
central to how Dawn of War and Company of Heroes organise unit/ability definitions.

## Overview

Game data files (RGD or Lua) can declare a parent via `Inherit([[path]])`. The
inheritance system resolves the full property set by walking the inheritance chain and
merging tables. The `CInheritTable` class manages the tree, while `CLuaFromRgd` handles
the RGD-to-Lua conversion with differential output.

## Inheritance Tree (`CInheritTable`)

### Data Structure

```
CInheritTable
  └─ m_mapNodes: std::map<uint32, CNode*>   (CRC → collision chain)
  └─ m_pRootNode: CNode*                     (empty-path root)

CNode
  ├─ m_sFullName      — full path (e.g. "attrib\\weapon\\bolter")
  ├─ m_sMiniName      — leaf name (e.g. "bolter")
  ├─ m_bIsNil         — true if path ends with ".nil"
  ├─ m_pParent        — parent node pointer
  ├─ m_vChildren      — vector of child nodes
  └─ m_pHashMapNext   — collision chain pointer
```

### Path → Node Resolution (`findOrMake`)

1. Compute case-insensitive CRC32 of the full path string.
2. Look up the CRC in `m_mapNodes` (`std::map`).
3. If found, walk the collision chain (`m_pHashMapNext`), comparing paths with
   `stricmp()`.
4. If an exact match is found, return it.
5. Otherwise, create a new `CNode`, set its path via `setName()`, prepend it to the
   collision chain, and return it.

Collision chaining handles the (rare) case where different paths produce the same CRC32.

### `setName` — Nil Detection

When a node's path is set:
- The **mini name** is extracted as the portion after the last `\\`.
- If the path ends with `".nil"`, `m_bIsNil` is set to `true`. Nil files mark the
  "default" state — properties that match the nil file's values are omitted in output.

### `setParent` — Tree Assembly

```
function setParent(newParent):
    if currentParent exists:
        remove self from currentParent.children
    m_pParent = newParent
    if newParent exists:
        newParent.children.push_back(self)
```

Parent assignment is explicit — it is not inferred from the path hierarchy.

### `assignOrphansTo` — Orphan Adoption

Iterates every node in the hash map. Any node with no parent (except the root itself) is
adopted by the specified node.

## RGD → Lua Conversion (`CLuaFromRgd`)

### Quick Dump (`MakeLuaFromRgdQuickly`)

Recursively dumps an RGD file to Lua syntax without any differential comparison. Every
property is written, regardless of whether it matches a parent/nil file.

### Smart Merge (`MakeLuaFromRgdAndNil`)

Produces minimal Lua output by comparing the RGD data against a nil (default) file. Only
properties that differ from the defaults are emitted.

#### Algorithm

```
function MakeLuaFromRgdAndNil(rgd, nilFile, output):
    Load RGD file
    If nilFile provided:
        Find matching "GameData" child in nil
    MakeLuaFromRgdAndNil_Node(rgdRoot, nilRoot, output)
```

#### Three-Way Node Comparison

For each property in the RGD and nil trees:

| RGD Value | Nil Value | Action |
|-----------|-----------|--------|
| absent | present | Output `= nil` (explicit deletion) |
| present | absent | Output the value |
| same type, same value | — | **Skip** (inherited from default) |
| different type or value | — | Output the value |

**Float comparison** uses a tolerance of ±0.00001 for equality.

#### Reference Resolution

When a table's `Reference()` path differs from the nil file:

1. Search for the referenced file in 4 locations (in order):
   - `attrib\attrib\*.lua`
   - `attrib\attrib\*.rgd`
   - `data\attrib\*.lua`
   - `data\attrib\*.rgd`
2. Load the referenced file (Lua or RGD).
3. Extract the `GameData` table.
4. Use the loaded table as the new baseline for child comparison.

#### Child Merging

1. Collect all children from both the RGD and nil tables.
2. Match children by Jenkins hash of their property name.
3. Recursively compare each matched pair.
4. Output only children whose values differ from the nil baseline.

### Output Format

```lua
-- Global declaration with inheritance
GameData = Inherit([[attrib\weapon\bolter]])

-- Only overridden properties
GameData["damage"] = 25.00000
GameData["range"]  = 35.00000
-- GameData["speed"] is omitted (matches parent)
```

UCS string references are annotated with human-readable comments when a UCS resolver is
available.

## Source Files

- `CInheritTable.h` / `CInheritTable.cpp` — Inheritance tree structure
- `CLuaFromRgd.h` / `CLuaFromRgd.cpp` — RGD → Lua differential conversion
