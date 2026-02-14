---
name: cpp-developer
description: Write new C++ code that follows CorsixModStudio codebase conventions. Use when the user asks to implement a feature, add a class, create a new file, write C++ code, or develop functionality in the Rainman library, CDMS GUI, or vendored Lua layer. Triggers on requests mentioning "implement", "add class", "new file", "write code", "develop", "create", "C++", "cpp", or "feature".
---

# C++ Developer

Write new C++ code that follows the CorsixModStudio codebase conventions across all layers.

## Workflow

1. Determine which layer the new code belongs to:

   **Rainman** (`src/rainman/`) → Consult [references/rainman-patterns.md](references/rainman-patterns.md)
   **CDMS** (`src/cdms/`) → Consult [references/cdms-patterns.md](references/cdms-patterns.md)
   **Vendored Lua** (`src/rainman/lua502/`) → Consult [references/lua-rules.md](references/lua-rules.md)

2. Apply the common conventions below to all new code.
3. After writing code, add new files to the appropriate `CMakeLists.txt`.
4. Build and verify: `cmake --build build --config Debug`

## File Header

Every new `.h` and `.cpp` file MUST begin with the LGPL v2.1 copyright block:

```cpp
/*
Rainman Library
Copyright (C) 2006 Corsix <corsix@gmail.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
```

## Naming Conventions

| Element | Convention | Example |
|---------|-----------|---------|
| Concrete class | `C` prefix | `CSgaFile`, `CMemoryStore` |
| Interface/abstract class | `I` prefix | `IFileStore`, `IMetaTable` |
| Member variable | `m_` + Hungarian prefix | `m_sMessage` (string), `m_iLine` (integer), `m_pPrecursor` (pointer), `m_bInited` (bool) |
| Virtual method | `V` prefix | `VInit`, `VRead`, `VSeek`, `VOpenStream` |
| Include guard | `_UPPER_SNAKE_H_` | `_C_SGA_FILE_H_` |

Hungarian prefixes: `s` = string/char*, `i` = integer/unsigned long, `p` = pointer, `b` = bool, `f` = float, `ws` = wchar_t*, `t` = table/vector, `o` = object.

## Error Handling

This codebase uses **heap-allocated exception objects**, not stack-based C++ exceptions. Exceptions are created with `new` and cleaned up with `destroy()`, **never** `delete`.

### Throwing
```cpp
// Simple throw
throw new CRainmanException(__FILE__, __LINE__, "message");

// Or use macros:
QUICK_THROW("message");                    // throw new CRainmanException(...)
CATCH_THROW("context message");            // catch + re-throw with precursor
```

### Catching and Cleanup
```cpp
try {
    SomeOperation();
} catch (CRainmanException* pE) {
    pE->destroy();  // CRITICAL: always destroy(), never delete
}

// Or use macros:
try { SomeOperation(); } IGNORE_EXCEPTIONS  // catch + destroy
try { SomeOperation(); } CATCH_THROW("Failed during X")  // catch + re-throw
```

### Memory Allocation Checks
```cpp
char* pBuffer = CHECK_MEM(new char[1024]);  // throws if allocation fails
```

## Memory Management

- Use raw `new`/`delete` — no smart pointers in existing APIs.
- Callers own stream objects returned by `VOpenStream()`/`VOpenOutputStream()` — caller must `delete` them.
- Use `AutoDelete<T>` (from `Internal_Util.h`) for RAII cleanup of raw pointers when convenient.
- Use `CHECK_MEM(new ...)` for all allocations that could fail.

## RAINMAN_API Macro

All public Rainman classes MUST use the `RAINMAN_API` macro on their class declaration. It currently expands to nothing (static lib build) but must remain for future DLL compatibility:

```cpp
#include "Api.h"

class RAINMAN_API CMyNewClass
{
    // ...
};
```

Nested public classes also need `RAINMAN_API`:
```cpp
class RAINMAN_API CMyClass : public IFileStore
{
public:
    class RAINMAN_API CStream : public IFileStore::IStream { ... };
};
```

## Legacy Type Rules

- Use `unsigned long` for sizes and counts in public APIs (not `size_t` or `uint32_t` — changing would cascade through the codebase).
- Use `long` for signed offsets (e.g., seek positions).
- Use Windows-only APIs where the codebase already does (`_wfopen`, `LoadLibraryW`, `GetProcAddress`).
- C-style casts exist throughout; match existing style in the file you're editing.

## Build Integration

### Rainman (`src/rainman/CMakeLists.txt`)
Sources are collected via `file(GLOB)`, so new `.cpp`/`.c` files are auto-discovered. Just add the file and rebuild.

### CDMS (`src/cdms/CMakeLists.txt`)
Same `file(GLOB)` pattern — new source files are auto-discovered.

### Tests (`tests/rainman/CMakeLists.txt`)
Test files must be **explicitly listed** in the `add_executable(rainman_tests ...)` call:
```cmake
add_executable(rainman_tests
    # ... existing files ...
    newfile_test.cpp    # Add new test file here
)
```

### Build & Verify
```powershell
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```
