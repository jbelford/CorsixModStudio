# Lua Language Server Integration — Design Document

## 1. Overview

CorsixModStudio has a built-in SCAR/Lua editor (`frmScarEditor`) using wxStyledTextCtrl
(Scintilla) with a manual "Check Lua" button that does one-shot syntax validation via
`luaL_loadbuffer`, and a custom autocomplete/calltip system driven by binary reference
files. We want to add **real-time diagnostics and context-aware autocompletion** by
integrating [LuaLS](https://github.com/LuaLS/lua-language-server) (the Lua Language
Server) via the Language Server Protocol.

This document focuses on three areas: **making LuaLS work with Lua 5.0.2** (Dawn of
War), **making it understand the SCAR game scripting API** (both DoW and CoH), and
**replacing the current autocomplete/calltip system** with LSP-driven completions.

---

## 2. Current State

### What exists today

| Component | Details |
|-----------|---------|
| **Editor widget** | `wxStyledTextCtrl` with `wxSTC_LEX_LUA` lexer, syntax highlighting, calltips, autocomplete |
| **Syntax check** | One-shot `luaL_loadbuffer()` via "Check Lua" button — reports first error only |
| **SCAR API docs** | `scardoc.dat` (DoW, ~500 functions) and `scardoc_coh.dat` (CoH, ~1000 functions) in a custom binary format |
| **Autocomplete** | WORD3 = SCAR function names, WORD4 = SCAR constants — populated from scardoc at editor open |
| **Calltips** | Parameter hints built from `_ScarFunction` structs parsed from scardoc |
| **Lua versions** | Vendored Lua 5.0.2 (DoW) and Lua 5.1.2 (CoH) with symbol renaming |
| **File extensions** | `.scar` for SCAR scripts, `.lua` for standard Lua (RGD macros) |

### Current Autocomplete & Calltip System (Deep Dive)

The existing editor intelligence is entirely custom-built in `frmScarEditor.cpp`, driven
by the binary `scardoc.dat`/`scardoc_coh.dat` reference files. Here's how it works:

#### Data Source

At editor construction, the binary reference file is parsed into
`std::list<_ScarFunction> m_lstScarFunctions`:

```cpp
struct _ScarFunction {
    char *sReturn;              // Return type (e.g. "Void", "SGroupID")
    char *sName;                // Function name (e.g. "SGroup_Create")
    std::list<char *> sArguments; // Parameters (e.g. "PlayerID player")
    char *sDesc;                // Documentation text
    int iType;                  // 0 = function, 1 = constant
};
```

Functions (iType=0) and constants (iType=1) are also fed into Scintilla's keyword
sets (WORD3 and WORD4) for **syntax highlighting** — SCAR API calls appear in a
distinct colour.

#### Autocomplete

**Trigger:** The `_` (underscore) character — because SCAR functions use `Namespace_`
naming (e.g., typing `SGroup_` triggers the popup).

**Behaviour:**
1. On `_`, extract the word-so-far from cursor position using `WordStartPosition()`
2. Linear scan through `m_lstScarFunctions`, prefix-matching with `strncmp()`
3. Build a space-separated string of matching function names
4. Call `m_pSTC->AutoCompShow(charCount, matchList)`
5. Scintilla handles the popup UI, arrow-key selection, and insertion

**Limitations:**
- Only triggers on `_` — typing `SG` without an underscore won't show completions
- Only matches SCAR API functions — no completions for user-defined functions,
  local variables, Lua standard library, or table fields
- No fuzzy matching — strictly prefix-based
- No sorting — results appear in scardoc file order
- Linear scan every keystroke — O(n) over the full function list
- Only works for functions (iType=0), never offers constant completions

#### Calltips (Parameter Hints)

**Trigger:** The `(` character — shows a popup with the function signature.

**Behaviour:**
1. On `(`, extract the word before the parenthesis
2. Linear search through `m_lstScarFunctions` for exact name match
3. Format calltip string: `ReturnType FuncName(arg1,\n    arg2)\nDescription`
4. Display via `m_pSTC->CallTipShow()` with light-blue background
5. If no match found, shows "No help available"

**Nested call handling:** A stack (`m_stkCalltips`) saves the current calltip when a
new `(` is typed, and restores it when `)` is typed:
```cpp
void _PushThisCalltip();          // push current, show new
void _RestorePreviousCalltip();   // pop and restore previous
```

**Limitations:**
- Only knows about SCAR API functions — no calltips for user-defined functions
- No parameter highlighting (doesn't track which argument the cursor is on)
- Calltip content is plain text — no rich formatting or type colouring
- Must match function name exactly — no fuzzy or partial matching

#### Other Editor Intelligence

| Feature | Status |
|---------|--------|
| **Auto-indent** | ✓ Copies previous line's indentation on Enter |
| **Syntax highlighting** | ✓ Scintilla's built-in Lua lexer with SCAR keyword colouring |
| **Function dropdown** | ✓ Scans document for `function` keyword, populates a sorted wxChoice — click to jump to definition |
| **Auto-close brackets** | ✗ Not implemented |
| **Go-to-definition** | ✗ Not implemented |
| **Find references** | ✗ Not implemented |
| **Rename symbol** | ✗ Not implemented |
| **Variable completions** | ✗ Not implemented |
| **Table field completions** | ✗ Not implemented |

### What we want to add

Real-time diagnostics as you type — syntax errors, undefined globals, type mismatches —
displayed as squiggly underlines in the editor. Also, **LSP-driven autocompletion** that
replaces the current underscore-triggered system with context-aware completions for SCAR
API functions, user-defined functions, local variables, table fields, and Lua standard
library. The language server runs as a bundled subprocess communicating over stdin/stdout
JSON-RPC.

---

## 3. The Three Hard Problems

### 3a. Lua 5.0.2 Support

**LuaLS only supports Lua 5.1+.** Dawn of War uses Lua 5.0.2, which has meaningful
differences. Our strategy: **configure LuaLS for Lua 5.1 and provide definition stubs
that bridge the gap.**

#### Lua 5.0.2 → 5.1 API Differences

| Category | Lua 5.0.2 | Lua 5.1+ | Impact on LuaLS |
|----------|-----------|----------|-----------------|
| **String iteration** | `string.gfind(s, pat)` | `string.gmatch(s, pat)` | False "undefined" error on `gfind` |
| **String matching** | _(doesn't exist)_ | `string.match(s, pat)` | Scripts using `match` would be caught — correct |
| **Table length** | `table.getn(t)` / `table.setn(t, n)` | `#t` operator | False "deprecated" warnings on `getn`/`setn` |
| **Table iteration** | `table.foreach(t, f)` / `table.foreachi(t, f)` | _(removed)_ | False "undefined" errors |
| **Environments** | `getfenv()` / `setfenv()` | _(removed in 5.2)_ | Available in 5.1 mode ✓ |
| **Math** | `math.mod(x, y)` | `math.fmod(x, y)` | False error on `math.mod` |
| **Varargs** | `arg` table in functions | `...` expression | Both work in 5.1 mode ✓ |
| **Module system** | `require` + `_LOADED` table | `require` + `package` system | Different semantics, but syntax OK |
| **Length operator** | Not available (`table.getn` only) | `#` operator | If scripts use `#`, LuaLS won't catch the error |

#### The Definition Stub Strategy

We create a **Lua 5.0.2 compatibility definition file** (`lua502-compat.lua`) using
LuaLS `@meta` annotations. This file declares the 5.0-specific functions so LuaLS
doesn't flag them as undefined:

```lua
---@meta lua502-compat

-- Lua 5.0.2 standard library functions not present in 5.1

---Find all matches of pattern in string (Lua 5.0 name for string.gmatch)
---@param s string
---@param pattern string
---@return fun(): string ...
function string.gfind(s, pattern) end

---Return the size of a table (Lua 5.0; use # operator in 5.1+)
---@param t table
---@return integer
function table.getn(t) end

---Set the size of a table (Lua 5.0 only)
---@param t table
---@param n integer
function table.setn(t, n) end

---Apply function to each key-value pair (Lua 5.0; removed in 5.1)
---@param t table
---@param f fun(key: any, value: any): any?
function table.foreach(t, f) end

---Apply function to each integer-key element (Lua 5.0; removed in 5.1)
---@param t table
---@param f fun(index: integer, value: any): any?
function table.foreachi(t, f) end

---Modulo operation (Lua 5.0 name; renamed to math.fmod in 5.1)
---@param x number
---@param y number
---@return number
function math.mod(x, y) end
```

#### LuaLS Configuration for Lua 5.0.2

```json
{
  "runtime.version": "Lua 5.1",
  "runtime.builtin": {
    "io": "disable",
    "debug": "disable",
    "os": "disable",
    "package": "disable"
  },
  "workspace.library": [
    "path/to/definitions/lua502-compat.lua",
    "path/to/definitions/scar-dow.lua"
  ],
  "diagnostics.globals": [
    "GameData", "MetaData", "Reference", "InheritMeta"
  ],
  "diagnostics.disable": [
    "lowercase-global"
  ]
}
```

**Why disable `io`/`debug`/`os`/`package`?** The game's Lua sandbox strips these
(CLuaScript explicitly deletes `dofile`, `loadfile`, `require`, `getfenv`, `setfenv`).
Scripts using them would be bugs — and disabling them in LuaLS means those get correctly
flagged as undefined.

#### Known Limitations (Lua 5.0.2 mode)

1. **`#` operator false acceptance** — LuaLS in 5.1 mode won't flag `#t` as invalid,
   but it doesn't exist in 5.0.2. Modders using `#` will get a runtime error.
   _Mitigation:_ Could add a custom diagnostic plugin, but this is low priority.

2. **Pattern syntax differences** — Lua 5.0.2 patterns are slightly more restrictive
   than 5.1. LuaLS can't catch this.

3. **`arg` table vs `...`** — Both work in 5.1 mode, so no false positives, but also
   no guidance about which to use.

These are acceptable trade-offs. The vast majority of SCAR script errors (typos,
undefined globals, wrong argument counts) will still be caught correctly.

---

### 3b. SCAR API Support

SCAR (Scripting At Relic) is the game's scripting layer built on top of Lua. It adds
**~500 functions (DoW) / ~1000 functions (CoH)** and **custom data types** that don't
exist in standard Lua. Without definition stubs, LuaLS would flag every SCAR API call
as "undefined global".

#### What SCAR adds to Lua

**Custom types** (passed as opaque handles from the C++ engine):
```
SGroupID     — Squad group (collection of military squads)
EGroupID     — Entity group (collection of buildings/objects)
SquadID      — Individual squad handle
EntityID     — Individual entity handle
PlayerID     — Player handle
MarkerID     — World-editor marker
Position     — 3D world coordinate { x, y, z }
ActorTable   — NIS actor reference
AbilityID    — Ability handle
UpgradeID    — Upgrade handle
CriticalID   — Critical hit type
```

**Function namespaces** (examples from scardoc.dat):
```
Actor_*       — NIS actor speech and animation
AI_*          — AI player control
Anim_*        — Animation playback
Balance_*     — Balance tuning
Camera_*      — Camera control
CPath_*       — Camera paths
EGroup_*      — Entity group operations
Entity_*      — Individual entity operations
Event_*       — Event system (rules, conditions)
FOW_*         — Fog of war
Misc_*        — Miscellaneous utilities
Modifier_*    — Gameplay modifiers
Objective_*   — Mission objectives (CoH)
Player_*      — Player operations
SGroup_*      — Squad group operations
Squad_*       — Individual squad operations
Sound_*       — Audio playback
UI_*          — HUD / UI control
Util_*        — Utility functions
World_*       — World queries and state
```

**Game-specific globals:**
```
import(fileName)        — Load and execute another SCAR file (NOT Lua require)
print(s1, s2, ...)      — Overridden to go to game console
blueprint_dump()        — Dump animator stats to file
```

#### Converting scardoc.dat → LuaLS Definition Files

The existing `scardoc.dat` and `scardoc_coh.dat` files contain all the API information
we need. They use a custom binary format:

```
[uint32] functionCount

For each function:
  [uint32 len][chars] returnType     — e.g. "Void", "Boolean", "SGroupID"
  [uint32 len][chars] functionName   — e.g. "SGroup_Create"
  if functionName is not empty:      — (empty name = constant, not function)
    [uint32] argCount
    for each arg:
      [uint32 len][chars] argument   — e.g. "PlayerID player"
    [uint32 len][chars] description  — documentation text
  else:
    — This is a constant; returnType actually contains the constant name
```

We build a **converter tool** (`tools/scardoc-to-luadefs.py` or C++) that reads
scardoc.dat and generates LuaLS-compatible definition files:

**Input** (binary scardoc.dat entry):
```
Return: "SGroupID"
Name:   "SGroup_CreateIfNotFound"
Args:   ["String name"]
Desc:   "Returns the SGroup with the given name..."
```

**Output** (generated `scar-dow.lua`):
```lua
---@meta scar-dow

-- Auto-generated from scardoc.dat — do not edit manually

---@class SGroupID
---@class EGroupID
---@class SquadID
---@class EntityID
---@class PlayerID
---@class MarkerID
---@class Position
---@field x number
---@field y number
---@field z number
---@class ActorTable

---Returns the SGroup with the given name...
---@param name string
---@return SGroupID
function SGroup_CreateIfNotFound(name) end

---Create a new Position object.
---@param x number
---@param y number
---@param z number
---@return Position
function World_Pos(x, y, z) end

---Loads data:scar/fileName and executes it
---@param fileName string
function import(fileName) end
```

#### Type Mapping: scardoc types → LuaLS annotations

The scardoc files use Relic's type names. We map them to LuaLS annotations:

| scardoc Type | LuaLS Annotation | Notes |
|-------------|-----------------|-------|
| `Void` | _(no @return)_ | Function returns nothing |
| `Boolean` | `boolean` | |
| `Integer` | `integer` | |
| `Real` / `Float` | `number` | |
| `String` | `string` | |
| `Table` | `table` | Generic table |
| `Function` / `LuaFunction` | `function` | Callback parameter |
| `SGroupID` | `SGroupID` | Custom `@class` |
| `EGroupID` | `EGroupID` | Custom `@class` |
| `PlayerID` | `PlayerID` | Custom `@class` |
| `Position` / `ScarPosition` | `Position` | Custom `@class` with x,y,z fields |
| `SquadID` | `SquadID` | Custom `@class` |
| `EntityID` | `EntityID` | Custom `@class` |
| `MarkerID` | `MarkerID` | Custom `@class` |
| `Any` | `any` | |
| `CMD_*` | _(global constant)_ | Command enum values |
| `PropertyBagGroup` | `PropertyBagGroup` | Custom `@class` |

#### Argument Parsing

scardoc argument strings look like `"PlayerID playerId"` or `"String squadgroupName"`.
The converter splits on the first space: type = first word, name = rest.

Some arguments have optional markers like `[Boolean continueButton]` — these map to
`---@param continueButton? boolean` (optional parameter).

#### DoW vs CoH: Two Separate Definition Files

Since the SCAR APIs differ between games, we generate **two independent definition files**:
- `scar-dow.lua` — from `scardoc.dat` (~500 functions)
- `scar-coh.lua` — from `scardoc_coh.dat` (~1000 functions)

The LSP client selects the correct one based on the loaded module type
(`CModuleFile::MT_DawnOfWar` vs others).

#### SCAR-Specific Globals and Quirks

Beyond the scardoc functions, SCAR scripts use patterns that need special handling:

1. **`import()` instead of `require()`** — SCAR's module system. We define it as a
   global function in the stubs. LuaLS won't resolve the imported file (it doesn't know
   the game's file system), but at least `import` won't be flagged as undefined.

2. **Rule system** — SCAR uses a callback registration pattern:
   ```lua
   Rule_AddOneShot(MyFunction, 5)  -- call MyFunction after 5 seconds
   Rule_AddInterval(OnTick, 1)     -- call OnTick every 1 second
   ```
   Functions passed to `Rule_Add*` are called by the engine — LuaLS might warn they're
   "unused". We can suppress this with `diagnostics.disable: ["unused-function"]` or
   by annotating them in the definition stubs.

3. **Global-heavy style** — SCAR scripts define most functions as globals (not `local`).
   This is idiomatic for mission scripts. We disable the `lowercase-global` diagnostic
   and potentially `global-element` to avoid noise.

4. **No `local` discipline** — Classic DoW/CoH scripts rarely use `local`. LuaLS will
   flag these as globals, which is technically correct but noisy. Configurable per user
   preference.

5. **String concatenation with `..`** — Standard Lua, works fine with LuaLS.

6. **`World_Pos()` returns a table** — Position is `{ x, y, z }` accessed as fields.
   Our `@class Position` with `@field` annotations will give proper field completion
   and type checking.

---

### 3c. Replacing the Autocomplete System

The existing autocomplete is the weakest part of the editor — it only triggers on `_`,
only knows about SCAR API functions, and has no awareness of the code being edited. An
LSP-based approach is dramatically better.

#### What LSP Completion Gives Us (for Free)

With LuaLS and our `@meta` definition files, autocompletion will automatically support:

| Completion Type | Current System | With LuaLS |
|----------------|----------------|------------|
| SCAR API functions | ✓ On `_` only | ✓ On any character, with fuzzy matching |
| SCAR API constants | ✗ Never offered | ✓ Included in completions |
| SCAR parameter types | ✗ | ✓ Via calltip from `@param` annotations |
| User-defined functions | ✗ | ✓ Scanned from current document |
| Local variables | ✗ | ✓ Scope-aware |
| Table field access | ✗ | ✓ e.g., `pos.` → `x`, `y`, `z` for Position |
| Lua standard library | ✗ | ✓ `string.`, `table.`, `math.` |
| Keyword completions | ✗ | ✓ `function`, `local`, `if`, etc. |
| Snippet expansions | ✗ | ✓ `for`, `function`, `if-then-end` blocks |

#### LSP Completion Protocol

We need to add three more LSP messages beyond the diagnostics-only set:

| Direction | Method | Purpose |
|-----------|--------|---------|
| Client→Server | `textDocument/completion` | Request completions at cursor position |
| Server→Client | _(response)_ | List of `CompletionItem` objects |
| Client→Server | `completionItem/resolve` | Get full detail for a selected item (optional, lazy-loaded) |

**Completion request** is triggered by the GUI when the user types a character. The LSP
client sends the cursor position and receives a list of completion items, each with:
- `label` — display text (e.g., `SGroup_CreateIfNotFound`)
- `kind` — icon type (Function, Variable, Keyword, Field, etc.)
- `detail` — type signature (e.g., `fun(name: string): SGroupID`)
- `documentation` — description from scardoc
- `insertText` — text to insert (may differ from label for snippets)

#### How to Wire It: Scintilla Autocomp ↔ LSP

The wxStyledTextCtrl autocomplete API is simple — it takes a space-separated word list.
To bridge LSP completions into Scintilla:

```
User types character
    ↓
GUI calls LspClient::RequestCompletion(uri, line, column)
    ↓
LspClient sends textDocument/completion to LuaLS
    ↓
LuaLS responds with CompletionItem[]
    ↓
LspClient invokes completion callback with items
    ↓
GUI formats items into space-separated string
GUI calls m_pSTC->AutoCompShow(prefixLen, itemList)
    ↓
User selects item → Scintilla inserts text
```

**Key consideration: Scintilla's autocomplete is synchronous** but LSP completions
arrive asynchronously. The GUI must either:
- **Option A (Recommended):** Request completion on typing, then show the popup when
  the response arrives (brief delay, usually <100ms from LuaLS). If the user has moved
  on, discard stale results.
- **Option B:** Pre-fetch completions and cache them. More complex and less accurate.

#### Calltips → LSP Signature Help

The current calltip system (triggered on `(`) maps directly to LSP's
`textDocument/signatureHelp`:

| Direction | Method | Purpose |
|-----------|--------|---------|
| Client→Server | `textDocument/signatureHelp` | Request signature at cursor position |
| Server→Client | _(response)_ | `SignatureHelp` with signatures + active parameter |

**What LuaLS signature help adds over the current system:**
- **Active parameter highlighting** — LuaLS tracks which parameter the cursor is on
  and highlights it in the signature. The current system shows all parameters with no
  indication of which one you're filling in.
- **User-defined function signatures** — Not just SCAR API functions. If you define
  `function MyHelper(a, b, c)` elsewhere in the file, typing `MyHelper(` will show
  the signature.
- **Overload support** — If a function has multiple signatures (via `@overload`
  annotations), LuaLS shows all of them.

**Scintilla calltip integration:** Scintilla's `CallTipShow()` takes a plain string,
so the GUI formats the LSP `SignatureHelp` response into a display string, similar to
today but with the active parameter emphasised (Scintilla supports calltip highlighting
via `CallTipSetHighlight(start, end)`).

#### Migration Strategy: Current System → LSP

The transition should be **incremental**, not a hard switch:

1. **Phase 1: LSP diagnostics only** — Add the LSP client, wire up diagnostics.
   Autocomplete continues using the current `_`-triggered system.

2. **Phase 2: LSP completions alongside existing** — Add completion request/response.
   Wire into a new trigger (e.g., `Ctrl+Space` or after any identifier character).
   Keep the existing `_` trigger as fallback.

3. **Phase 3: Replace existing autocomplete** — Once LSP completions are proven
   reliable, remove the `_`-triggered system and the `m_lstScarFunctions` linear scan.
   Remove the `scardoc.dat` parsing from the editor constructor (the data now lives
   in LuaLS definition files).

4. **Phase 4: Replace calltips** — Swap `CallTipShow()` from the current
   `_ScarFunction`-based system to LSP `signatureHelp` responses.

**What we keep regardless:** The Scintilla keyword lists (WORD3/WORD4) for syntax
highlighting. These are separate from autocomplete — they just colour SCAR function
names in the editor. We could either keep populating them from scardoc.dat, or generate
a keyword list from the LuaLS definition files at build time.

---

## 4. Architecture

### LSP Client Library (`src/lsp/`)

```
┌─────────────────────────────────────────────────────────┐
│                    CDMS GUI (future)                     │
│                                                         │
│  frmScarEditor ──► LspClient.OpenDocument("file.scar")  │
│  wxTimer       ──► LspClient.Poll() → diagnostics CB    │
│  OnTextChanged ──► LspClient.ChangeDocument(...)        │
└────────────────────────┬────────────────────────────────┘
                         │ C++ API
┌────────────────────────▼────────────────────────────────┐
│                   LspClient (src/lsp/)                   │
│                                                         │
│  ┌─────────────┐  ┌──────────────┐  ┌───────────────┐  │
│  │ Protocol.h  │  │ JsonRpc.h/cpp│  │ Transport.h/  │  │
│  │ LSP types + │  │ Content-Len  │  │ cpp           │  │
│  │ JSON serde  │  │ framing      │  │ Win32 process │  │
│  └─────────────┘  └──────────────┘  │ + pipe I/O    │  │
│                                     └───────┬───────┘  │
└─────────────────────────────────────────────┼──────────┘
                                              │ stdin/stdout
                                   ┌──────────▼──────────┐
                                   │   LuaLS process     │
                                   │   (bundled .exe)     │
                                   │                     │
                                   │  Configured with:   │
                                   │  • runtime = 5.1    │
                                   │  • library = [      │
                                   │      scar-dow.lua   │
                                   │      lua502-compat  │
                                   │    ]                │
                                   └─────────────────────┘
```

### File Layout

```
src/lsp/
├── CMakeLists.txt          # Static library target
├── Api.h                   # LSP_API export macro
├── Protocol.h              # LSP message types (structs + JSON)
├── JsonRpc.h / JsonRpc.cpp # Content-Length framing
├── Transport.h / .cpp      # Win32 process + pipe management
├── LspClient.h / .cpp      # High-level client API
└── README.md               # Usage guide for GUI integration

Mod_Studio_Files/lsp/
├── lua502-compat.lua       # Lua 5.0.2 API stubs for LuaLS
├── scar-dow.lua            # Generated SCAR API stubs (Dawn of War)
├── scar-coh.lua            # Generated SCAR API stubs (Company of Heroes)
├── scar-types.lua          # Shared SCAR type definitions (SGroupID, etc.)
├── dow-config.json         # LuaLS config for DoW (5.0.2 + SCAR)
└── coh-config.json         # LuaLS config for CoH (5.1 + SCAR)

tools/
├── fetch-luals.ps1         # Download LuaLS binary from GitHub releases
└── scardoc-to-luadefs.py   # Convert scardoc.dat → LuaLS definition files

tests/lsp/
├── CMakeLists.txt
├── jsonrpc_test.cpp
├── protocol_test.cpp
└── lspclient_test.cpp
```

### LSP Messages (Diagnostics + Completion Subset)

| Direction | Method | Purpose |
|-----------|--------|---------|
| Client→Server | `initialize` | Handshake, send workspace config |
| Client→Server | `initialized` | Ready notification |
| Client→Server | `textDocument/didOpen` | Open doc with full content |
| Client→Server | `textDocument/didChange` | Full doc sync on edit |
| Client→Server | `textDocument/didClose` | Release document |
| Client→Server | `textDocument/completion` | Request completions at cursor |
| Client→Server | `completionItem/resolve` | Get full detail for selected item |
| Client→Server | `textDocument/signatureHelp` | Request parameter hints on `(` |
| Client→Server | `shutdown` + `exit` | Clean teardown |
| Server→Client | `textDocument/publishDiagnostics` | Error/warning list |

### How Initialization Works

When the editor opens a `.scar` file:

1. **LspClient::Start(serverPath, workspaceRoot, moduleType)**
   - Spawns LuaLS process
   - Sends `initialize` with settings based on `moduleType`:
     - `MT_DawnOfWar` → Lua 5.1 + `lua502-compat.lua` + `scar-dow.lua`
     - Other → Lua 5.1 + `scar-coh.lua`
   - Declares client capabilities including `textDocument.completion` and
     `textDocument.signatureHelp`
   - Sends `initialized`

2. **LspClient::OpenDocument(uri, languageId, text)**
   - Sends `textDocument/didOpen`
   - `languageId` is always `"lua"` (LuaLS doesn't know `.scar` natively)
   - The URI uses `file:///` scheme pointing to a temp or virtual path

3. **LspClient::Poll()** (called on wxTimer, ~100ms interval)
   - Reads any pending messages from LuaLS stdout
   - Dispatches `publishDiagnostics` to registered callback
   - Callback receives: document URI, list of `{range, severity, message}`

4. **GUI maps diagnostics to Scintilla indicators** (future, out of scope)

### `.scar` File Extension Handling

LuaLS needs to know `.scar` files are Lua. Two approaches:

**Option A: Tell LuaLS via initialization settings**
```json
{
  "files.associations": {
    "*.scar": "lua"
  }
}
```

**Option B: Use `file:///` URIs with `.lua` extension internally**
When opening a `.scar` file, the LSP client maps it to a virtual `.lua` URI so LuaLS
treats it as Lua without extra configuration.

Option A is cleaner. Option B is the fallback.

---

## 5. Implementation Todos

### Phase 1: Infrastructure
1. **Add nlohmann/json** to vcpkg.json
2. **Create `src/lsp/` skeleton** — CMakeLists.txt, Api.h
3. **JSON-RPC framing** — Content-Length encode/decode
4. **LSP protocol types** — Struct definitions + JSON serde for all messages (initialize,
   didOpen/Change/Close, publishDiagnostics, completion, signatureHelp)
5. **Transport** — Win32 process spawn with pipe I/O

### Phase 2: Client
6. **LspClient** — High-level API: Start/Stop, Open/Change/CloseDocument, Poll,
   RequestCompletion, RequestSignatureHelp (with async callbacks)
7. **Bundle LuaLS** — Download script + CMake integration
8. **Tests** — Unit tests for all components + integration test

### Phase 3: SCAR + Lua 5.0.2 Compatibility
9. **scardoc converter** — `tools/scardoc-to-luadefs.py`
10. **Lua 5.0.2 compat stubs** — `lua502-compat.lua`
11. **SCAR type definitions** — `scar-types.lua`
12. **LuaLS configs** — `dow-config.json` + `coh-config.json`

### Phase 4: GUI Integration (future)
13. **Wire diagnostics** into Scintilla indicators
14. **Wire completions** into `AutoCompShow()` (replace `_`-triggered system)
15. **Wire signature help** into `CallTipShow()` (replace `_ScarFunction` calltips)
16. **Documentation**

---

## 6. Key Design Decisions

| Decision | Rationale |
|----------|-----------|
| **Separate `lsp` library** | Testable independently, no wxWidgets dependency |
| **Single-threaded Poll()** | Avoids thread-safety complexity in GUI; wxTimer drives it |
| **Full document sync** | Simple to implement; LuaLS handles incremental analysis internally |
| **nlohmann/json** | Industry standard C++ JSON, available in vcpkg |
| **LuaLS as server** | Most mature Lua LS, MIT licensed, actively maintained |
| **Lua 5.1 mode + stubs for 5.0** | Best available approximation; syntax is 99% compatible |
| **Generate stubs from scardoc.dat** | Reuses existing data; stays in sync with game API docs |
| **Two config profiles** | DoW and CoH have different APIs; clean separation |
| **Async completion with callback** | Scintilla autocomplete is synchronous but LSP is async; callback bridges the gap |
| **Incremental migration** | Keep existing autocomplete working while LSP is brought up; no big-bang switch |

---

## 7. Risks & Mitigations

| Risk | Severity | Mitigation |
|------|----------|------------|
| LuaLS doesn't support Lua 5.0 natively | Medium | 5.1 mode + compat stubs cover ~95% of cases |
| `#` operator false acceptance in 5.0 mode | Low | Document as known limitation; rarely causes real issues |
| SCAR type checking is shallow (opaque handles) | Low | `@class` annotations prevent type confusion; can't validate handle validity |
| LuaLS startup latency (~1-2s) | Low | Start server eagerly when editor opens |
| scardoc.dat format may differ between game versions | Medium | Converter handles gracefully; test with available .dat files |
| LuaLS binary size (~30MB) | Low | Download on first use; document in README |
| SCAR `import()` can't resolve cross-file references | Medium | Define as stub; cross-file analysis is a future extension |
| Global-heavy SCAR style generates diagnostic noise | Medium | Disable `lowercase-global` and `global-element` diagnostics |

---

## 8. What Won't Work (Honest Limitations)

1. **Cross-file analysis** — When a SCAR script calls `import("other_script.scar")`,
   LuaLS won't automatically analyze the imported file. It doesn't know the game's
   virtual file system. Functions defined in imported files will appear as undefined
   unless all related files are opened.

2. **Runtime type validation** — SCAR types like `SGroupID` are opaque C++ handles.
   LuaLS can check that you pass an `SGroupID` where one is expected, but it can't
   validate that the handle refers to a valid squad group.

3. **Game-state-dependent APIs** — Some SCAR functions only work in certain contexts
   (e.g., during NIS playback, during specific game phases). LuaLS can't know this.

4. **Lua 5.0.2 edge cases** — The `#` length operator, string pattern differences,
   and some metatable behaviors differ between 5.0 and 5.1. LuaLS may not catch these.

5. **Custom Corsix Lua extensions** — The `luax` extensions (`string_split`,
   `string_before`, `math_clamp`, etc.) are only available in CorsixModStudio's
   embedded Lua, not in the game. We'd need separate stubs for "editor Lua" vs
   "game Lua" if we ever want to lint editor-side scripts too.

---

## 9. Future Extensions (Out of Scope for Now)

- Cross-file analysis via workspace scanning of mod folders
- Blueprint/game-data-aware completions (e.g., squad blueprint names as string literals)
- Go-to-definition within mod files
- Find references / rename symbol
- Custom diagnostic plugin for Lua 5.0.2 strict mode (flag `#` operator, etc.)
- Hover info (textDocument/hover) — show documentation on mouse-over
