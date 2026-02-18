---
name: cpp-developer
description: Write new C++ code that follows CorsixModStudio codebase conventions. Use when the user asks to implement a feature, add a class, create a new file, write C++ code, or develop functionality in the Rainman library, CDMS GUI, or vendored Lua layer. Triggers on requests mentioning "implement", "add class", "new file", "write code", "develop", "create", "C++", "cpp", or "feature".
---

# C++ Developer

Write new C++ code that follows CorsixModStudio codebase conventions across all layers,
**using smart pointers and modern C++20 ownership practices by default**. Raw `new`/`delete`
is only acceptable at legacy API boundaries — all other heap ownership must use
`std::unique_ptr`, `std::shared_ptr`, or RAII containers.

**Boy-scout rule**: When touching existing code, improve it toward modern C++ if the change
is safe and localized — especially migrating raw `new`/`delete` to smart pointers. Keep
modernization changes within the scope of the files you're already modifying.

## Workflow

1. Determine which layer the new code belongs to:

   **Rainman** (`src/rainman/`) → Consult [references/rainman-patterns.md](references/rainman-patterns.md)
   **CDMS** (`src/cdms/`) → Consult [references/cdms-patterns.md](references/cdms-patterns.md)
   **Vendored Lua** (`src/rainman/lua502/`) → Consult [references/lua-rules.md](references/lua-rules.md)

2. Apply the common conventions below to all new code.
3. **Default to modern C++20** — see the Modern C++ Practices section. Fall back to legacy
   style only at existing API boundaries. When modifying existing code, apply boy-scout
   improvements (add `override`, `nullptr`, `const`, range-for, etc.) within the touched scope.
4. After writing code, add new files to the appropriate `CMakeLists.txt`.
5. Build and verify: `cmake --build --preset debug`

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

## Memory Management — Smart Pointers by Default

**All new code MUST use smart pointers for heap ownership.** Raw `new`/`delete` is only
acceptable at legacy API boundaries that require it (e.g., returning from `VOpenStream()`).
Even there, use smart pointers internally and `.release()` at the return statement.

### Ownership hierarchy (choose in order)

1. **`std::unique_ptr<T>`** — default for sole ownership. Use `std::make_unique<T>(...)`.
2. **`std::shared_ptr<T>`** — when ownership is genuinely shared (e.g., `CUcsFile` in
   `CUcsHandler` and `CDoWModule`). Use `std::make_shared<T>(...)`.
3. **`std::unique_ptr<T, Deleter>`** — for objects with custom cleanup (streams, exceptions).
4. **Raw pointer** — only for non-owning observation, or at existing virtual API boundaries.

### Wrapping legacy API returns

Always wrap raw-pointer returns from legacy APIs in a smart pointer **immediately** at the
call site — never hold a raw owning pointer across more than one statement:

```cpp
// Streams from VOpenStream() — caller owns, so wrap immediately
auto pStream = std::unique_ptr<IFileStore::IStream>(store.VOpenStream("path"));
pStream->VRead(1, sizeof(unsigned long), &iValue);
// stream is automatically deleted on scope exit or exception

// CRainmanException* uses destroy(), not delete — use custom deleter
auto pEx = std::unique_ptr<CRainmanException, decltype(&CRainmanException::destroy)>(
    pCaught, &CRainmanException::destroy);

// Heap buffers — use make_unique instead of new[]
auto pBuffer = std::make_unique<char[]>(1024);
```

### Implementing virtual interfaces that return raw pointers

When overriding `VOpenStream()`, `VOpenOutputStream()`, or `VIterate()`, the API contract
requires returning a raw pointer. Build the object with a smart pointer internally, then
release at the boundary:

```cpp
IFileStore::IStream* CMyStore::VOpenStream(const char* sIdentifier)
{
    auto pStream = std::make_unique<CMyStream>(/* args */);
    pStream->Init(sIdentifier);       // safe: if Init throws, unique_ptr cleans up
    return pStream.release();          // transfer ownership to caller
}
```

### Boy-scout rule for ownership

When touching existing code that uses raw `new`/`delete`, migrate to smart pointers if the
change is safe and localized:
- `new T` → `std::make_unique<T>(...)` (adjust callers in the same file)
- `new T[]` / `delete[]` → `std::make_unique<T[]>(n)` or `std::vector<T>`
- `delete pX` at function exit → wrap in `std::unique_ptr` at creation
- Multiple ownership sites → `std::shared_ptr<T>`

### CHECK_MEM at API boundaries

Use `CHECK_MEM(new ...)` only when the allocation is immediately returned through a raw-pointer
API boundary. For internal code, `std::make_unique` already throws `std::bad_alloc` on failure.

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

## Modern C++ Practices

**Default to modern C++20** in all new code. When editing existing code, apply boy-scout
improvements within the scope of your change. Only preserve legacy style at existing API
boundaries where changing would cascade.

### Smart pointers & RAII (mandatory)
- **Always** use `std::unique_ptr` / `std::make_unique` for sole-ownership heap objects.
- Use `std::shared_ptr` / `std::make_shared` when ownership is genuinely shared.
- Wrap raw-pointer returns from legacy APIs (e.g., `VOpenStream()`) in `std::unique_ptr`
  **immediately** at the call site — never hold a raw owning pointer.
- For `CRainmanException*`, use a `unique_ptr` with a custom deleter that calls `destroy()`.
- When overriding virtual methods that return raw pointers, build internally with
  `std::unique_ptr` and call `.release()` at the return statement.
- **Boy-scout**: When touching code with raw `new`/`delete`, migrate to smart pointers
  if the change is safe and localized within the files you're modifying.

### Modern types (internal / new code)
| Use | Instead of | When |
|-----|-----------|------|
| `std::string` / `std::string_view` | `char*` / `const char*` | Internal storage & parameters |
| `std::size_t` | `unsigned long` | Internal sizes & loop counters |
| `std::uint32_t`, `std::int64_t` | `unsigned long`, `long` | Internal fixed-width data |
| `std::vector<T>` | `T*` + manual `new[]`/`delete[]` | Owning dynamic arrays |
| `std::array<T,N>` | `T[N]` raw array | Fixed-size arrays |
| `std::optional<T>` | sentinel value / out-parameter | Nullable return values |
| `std::span<T>` (C++20) or pointer+size | `T*` + separate length | Non-owning views (when C++20 is available) |

> **API boundaries**: Public virtual interfaces and methods that override existing
> signatures must continue using `unsigned long`, `char*`, etc. to match.

### Keywords & syntax
- **`auto`** — use for iterators, factory returns, lambdas, and any type that is obvious
  from the right-hand side. Avoid `auto` when the type is not immediately clear.
- **`nullptr`** — never use `NULL` or `0` for null pointers.
- **`override`** — always mark virtual overrides. Add `override` to every reimplemented
  virtual method.
- **`const` / `constexpr`** — mark variables, parameters, and methods `const` wherever
  possible. Use `constexpr` for compile-time constants.
- **`enum class`** — prefer over plain `enum` for new enumerations.
- **`[[nodiscard]]`** — add to functions whose return value must not be ignored
  (e.g., factory functions, error codes).
- **`noexcept`** — mark functions that are guaranteed not to throw (note: the codebase
  throws `CRainmanException*`, so many functions cannot be noexcept).

### Loops & algorithms
- Prefer **range-based for** over index-based iteration:
  ```cpp
  for (const auto& item : m_tItems) { ... }
  ```
- Prefer **`<algorithm>`** functions (`std::find_if`, `std::transform`, `std::any_of`, etc.)
  over hand-written loops when they improve clarity.
- Use **structured bindings** for pairs, tuples, and map entries:
  ```cpp
  for (const auto& [key, value] : myMap) { ... }
  ```

### Strings
- Prefer `std::string` for new string storage and `std::string_view` for non-owning
  read-only access.
- Use `std::format` (C++20, or `fmt::format` if available) over `sprintf` / `snprintf`
  for new formatting code if the build supports it; otherwise `std::ostringstream` or
  `snprintf` is acceptable.

### Initialization
- Use **brace initialization** for aggregates and containers:
  ```cpp
  std::vector<int> ids = {1, 2, 3};
  ```
- Prefer **in-class member initializers** over constructor initializer lists for default
  values:
  ```cpp
  class CMyClass {
      bool m_bInited = false;
      unsigned long m_iCount = 0;
  };
  ```

### What NOT to modernize
These legacy patterns must be preserved for compatibility — do not change them:
- **Heap-allocated exceptions** (`throw new CRainmanException`) — this is a project-wide
  convention, not a mistake.
- **`RAINMAN_API` macro** on public classes.
- **Hungarian-prefixed member names** (`m_sName`, `m_pStream`).
- **`C`/`I` class prefixes** and **`V` virtual-method prefix**.
- **Include guards** (`_C_CLASS_NAME_H_` style) — do not replace with `#pragma once`.
- **`unsigned long` in public/virtual API signatures** — changing cascades.
- **Vendored Lua code** — never touch `lua502/` or `lua512/`.

## Legacy Type Rules (API boundaries only)

These rules apply when implementing or overriding existing public/virtual interfaces.
For purely internal new code, prefer modern types (see above).

- Use `unsigned long` for sizes and counts in **existing public APIs** (changing would cascade through the codebase).
- Use `long` for signed offsets in existing seek interfaces.
- Use Windows-only APIs where the codebase already does (`_wfopen`, `LoadLibraryW`, `GetProcAddress`).
- Match the existing cast style (C-style or `static_cast`) of the file you're editing.
  In new files, prefer `static_cast` / `reinterpret_cast` / `const_cast`.

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
cmake --build --preset debug
ctest --preset debug
```
