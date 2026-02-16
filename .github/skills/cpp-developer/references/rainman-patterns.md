# Rainman Library Patterns

## Architecture

Rainman (`src/rainman/`) is a ~25k-line static library for Relic game file format I/O. No UI dependencies. Key design:

- **Polymorphic I/O**: `IFileStore` is the central abstraction. Implementations (`CMemoryStore`, `CFileSystemStore`, `CSgaFile`) provide `VOpenStream()` / `VOpenOutputStream()` returning `IStream` / `IOutputStream` objects.
- **Parser classes**: `CRgdFile`, `CUcsFile`, `CChunkyFile`, `CModuleFile` parse Relic's proprietary binary and text formats.
- **Archive writer**: `CSgaCreator` writes SGA archives.
- **Lua integration**: `CInheritTable` resolves Lua-based data inheritance.

## IFileStore / IStream Pattern

This is the most important pattern. Every file store follows this lifecycle:

```cpp
class RAINMAN_API CMyStore : public IFileStore
{
public:
    CMyStore();
    ~CMyStore();

    // Step 1: Initialize the store
    virtual void VInit(void* pInitData = 0);

    // Step 2: Open streams for reading
    virtual IStream* VOpenStream(const char* sIdentifier);

    // Step 3 (optional): Open streams for writing
    virtual IOutputStream* VOpenOutputStream(const char* sIdentifier, bool bEraseIfPresent);

protected:
    bool m_bInited;
};
```

### Stream Lifecycle Rules

1. `VOpenStream()` / `VOpenOutputStream()` return a heap-allocated stream.
2. The **caller** owns the returned stream and must `delete` it when done.
3. Streams throw `CRainmanException*` on errors (read past end, seek out of range).
4. Always check `m_bInited` before operations.

**Modern (preferred) usage — wrap in `std::unique_ptr` at the call site:**
```cpp
CMyStore store;
store.VInit();

auto pStream = std::unique_ptr<IFileStore::IStream>(store.VOpenStream("path/to/file"));
unsigned long iValue;
pStream->VRead(1, sizeof(unsigned long), &iValue);
// pStream is auto-deleted on scope exit or exception
```

**Legacy usage (when editing existing code):**
```cpp
CMyStore store;
store.VInit();

IFileStore::IStream* pStream = store.VOpenStream("path/to/file");
try {
    unsigned long iValue;
    pStream->VRead(1, sizeof(unsigned long), &iValue);
    // Or use the template helper:
    pStream->Read(iValue);
} catch (CRainmanException* pE) {
    delete pStream;
    throw; // re-throw after cleanup
}
delete pStream;
```

## CMemoryStore Pattern

Used for in-memory I/O, especially in tests:

```cpp
CMemoryStore store;
store.VInit();

// Reading: wrap existing data in a memory range
const char data[] = "file content here";
char* pRange = store.MemoryRange((void*)data, sizeof(data) - 1);
IFileStore::IStream* pStream = store.VOpenStream(pRange);
// ... read from stream ...
delete pStream;

// Writing: create an auto-resizing output buffer
IFileStore::IOutputStream* pOut = store.VOpenOutputStream("ignored", false);
pOut->VWrite(4, 1, "test");
// Retrieve written data via cast:
CMemoryStore::COutStream* pMemOut = static_cast<CMemoryStore::COutStream*>(pOut);
const char* pWritten = pMemOut->GetData();
unsigned long iLen = pMemOut->GetDataLength();
delete pOut;
```

## Parser Class Pattern

Parser classes follow a consistent structure:

```cpp
class RAINMAN_API CMyFormatFile
{
public:
    CMyFormatFile();
    ~CMyFormatFile();

    // Load from a stream (does not take ownership of the stream)
    void Load(IFileStore::IStream* pStream);

    // Save to an output stream
    void Save(IFileStore::IOutputStream* pStream);

    // Accessors for parsed data
    unsigned long GetCount() const;
    const char* GetName(unsigned long iIndex) const;

protected:
    // Parsed data stored as raw arrays/vectors
    unsigned long m_iCount;
    char** m_sNames;
    // ...
};
```

Key rules:
- `Load()` reads from a stream but does NOT take ownership (caller still deletes the stream).
- For **new** parser classes, prefer `std::vector<T>` and `std::string` for internal storage
  instead of raw `char**` / `T*` arrays.
- Existing parser classes use raw C arrays — match their style when modifying them.
- Destructor must clean up all owned memory (smart pointers handle this automatically).

## Directory Traversal

Classes that represent hierarchical data implement `IDirectoryTraverser`:

```cpp
class RAINMAN_API CMyArchive : public IFileStore, public IDirectoryTraverser
{
public:
    // IDirectoryTraverser interface
    virtual IDirectoryTraverser::IIterator* VIterate(const char* sPath);
};
```

## Adding a New Rainman Class

1. Create `CNewClass.h` and `CNewClass.cpp` in the appropriate subdirectory under `src/rainman/`.
2. Add the LGPL header to both files.
3. Use `#include "rainman/core/Api.h"` and any needed headers with fully-qualified `rainman/` prefix.
4. Apply `RAINMAN_API` to the class declaration.
5. Use `_C_NEW_CLASS_H_` style include guard.
6. **Default to modern C++20**: use `std::unique_ptr`, `std::string`, `std::vector`, `auto`,
   `override`, `nullptr`, range-for, etc. Only use legacy patterns at API boundaries
   that must match existing virtual signatures.
7. Files are auto-discovered by `file(GLOB)` — just rebuild.
