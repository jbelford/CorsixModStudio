# Test Writing Patterns

## Google Test Conventions

### Test Naming

Use `TEST(ClassName, MethodOrBehavior)` for standalone tests or `TEST_F(ClassNameTest, MethodOrBehavior)` for fixture-based tests.

```cpp
// Standalone — use when no shared setup is needed
TEST(CRainmanException, BasicCreation) { ... }

// Fixture — use when tests share common setup/teardown
class SgaFileTest : public ::testing::Test {
protected:
    void SetUp() override { ... }
    void TearDown() override { ... }
};
TEST_F(SgaFileTest, LoadFromNullStreamThrows) { ... }
```

### Assertion Selection

- `EXPECT_*` — Non-fatal; test continues after failure (prefer this)
- `ASSERT_*` — Fatal; test stops on failure (use when subsequent code depends on the check)
- `EXPECT_EQ/NE/LT/GT/LE/GE` — Value comparisons
- `EXPECT_STREQ/STRNE` — C-string comparisons
- `ASSERT_NE(ptr, nullptr)` — Always assert-not-null before dereferencing
- `EXPECT_EQ(memcmp(a, b, n), 0)` — Binary data comparison
- `EXPECT_THROW(stmt, ExceptionType)` — Exception testing

## Exception Testing Patterns

This codebase uses heap-allocated exceptions (`throw new CRainmanException*`). Two patterns:

### Pattern 1: EXPECT_THROW (simple, preferred for macro tests)
```cpp
TEST(ClassName, OperationThrows) {
    EXPECT_THROW({
        QUICK_THROW("message");
    }, CRainmanException*);
}
```

### Pattern 2: Manual catch (when you need to inspect the exception)
```cpp
TEST_F(Fixture, OperationThrowsWithDetails) {
    CRainmanException* caught = nullptr;
    try {
        object.SomeOperation(bad_input);
    } catch (CRainmanException* ex) {
        caught = ex;
    }
    ASSERT_NE(caught, nullptr);
    EXPECT_NE(std::strstr(caught->getMessage(), "expected text"), nullptr);
    caught->destroy();  // CRITICAL: always destroy, never delete
}
```

## File I/O Test Patterns

### Using CMemoryStore for in-memory testing
```cpp
CMemoryStore store;
store.VInit();

// Create a readable stream from data
const char data[] = "test data";
char* range = store.MemoryRange((void*)data, sizeof(data) - 1);
auto* stream = store.VOpenStream(range);
// ... use stream ...
delete stream;

// Create a writable output stream
auto* ostream = store.VOpenOutputStream("name", false);
ostream->VWrite(len, 1, data);
delete ostream;
```

### Using temp directories for filesystem tests
```cpp
class MyTest : public ::testing::Test {
protected:
    std::filesystem::path tempDir;

    void SetUp() override {
        tempDir = std::filesystem::temp_directory_path() /
            ("test_prefix_" + std::to_string(reinterpret_cast<uintptr_t>(this)));
        std::filesystem::create_directories(tempDir);
    }

    void TearDown() override {
        std::filesystem::remove_all(tempDir);
    }
};
```

## Common Test Categories

For each class, aim to cover:

1. **Construction/initialization** — Default state, VInit() success
2. **Happy path** — Primary operation works with valid input
3. **Error paths** — Null inputs, invalid data, uninitialized state
4. **Edge cases** — Empty data, boundary values, large inputs
5. **Round-trip** — Write then read back, verify data integrity
6. **Resource cleanup** — Streams/objects properly deleted, no leaks

## Registration

After creating a test file, add it to `tests/rainman/CMakeLists.txt`:
```cmake
add_executable(rainman_tests
    # ... existing files ...
    newfile_test.cpp    # Add new test file here
)
```
