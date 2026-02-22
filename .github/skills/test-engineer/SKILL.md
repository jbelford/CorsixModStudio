---
name: test-engineer
description: Investigate test coverage gaps, write new Google Test (gtest) tests, and improve existing test quality for C++ codebases. Use when the user asks to analyze test coverage, find untested code, write unit tests, improve test depth, create test fixtures, add edge case tests, or do anything related to C++ testing with Google Test. Triggers on requests mentioning "test", "coverage", "gtest", "unit test", "test gaps", "untested", or "test quality".
---

# Test Engineer

Analyze test coverage and write high-quality Google Test tests for C++ projects.

## Workflow

Determine the task type:

**"Analyze coverage" or "find gaps"** → Follow Coverage Analysis workflow
**"Write tests for X"** → Follow Test Writing workflow
**"Improve tests for X"** → Follow Test Improvement workflow

## Coverage Analysis

Follow the structured process in [references/coverage-analysis.md](references/coverage-analysis.md).

Steps:
1. Scan source directory for testable classes (files with `C` or `I` prefix)
2. Scan test directory for `*_test.cpp` files
3. Map each source file to its test file (or mark as missing)
4. For covered files, assess test depth (shallow/moderate/deep)
5. Prioritize untested files by complexity, risk, and testability
6. Output a coverage report with recommendations

Exclude from analysis: vendored code (`lua502/`, `lua512/`, `zLib/`), pure macro/typedef headers, and build system files.

## Writing New Tests

Consult [references/test-patterns.md](references/test-patterns.md) for conventions and examples.

### Process

1. Read the source file's header (`.h`) to understand the public API
2. Read the implementation (`.cpp`) to understand error paths and edge cases
3. Check if related classes already have test patterns to follow
4. Write tests covering: construction, happy path, error paths, edge cases, round-trip, cleanup
5. Add the new `*_test.cpp` file to the test CMakeLists.txt
6. Build and run the new tests to verify they pass

### Critical rules

- Exceptions are stack-based: `throw CRainmanException(...)`, catch by `const CRainmanException &`. No heap allocation or manual cleanup.
- Use `CMemoryStore` for in-memory I/O testing — avoid writing to the real filesystem when possible.
- When filesystem access is needed, use `std::filesystem::temp_directory_path()` with cleanup in `TearDown()`.
- Use `ASSERT_NE(ptr, nullptr)` before dereferencing any pointer.
- Follow existing naming: `TEST(ClassName, BehaviorDescription)` or `TEST_F(ClassNameTest, BehaviorDescription)`.
- Preserve existing file headers with LGPL v2.1 copyright notices.
- **Boy-scout rule**: When writing tests that exercise existing code, apply localized modern C++ improvements to the code under test if safe (e.g., add `override`, replace `NULL` with `nullptr`, add `const`). Keep such improvements within the scope of touched files.

## Improving Existing Tests

1. Read the existing test file to understand current coverage
2. Read the source to identify untested methods, branches, and error conditions
3. Add new test cases for:
   - Untested public methods
   - Missing error/edge case paths
   - Missing round-trip (write→read) validation
   - Boundary conditions (empty, max, off-by-one)
4. Avoid modifying passing tests unless they have bugs

## Build & Verify

After writing or modifying tests:

```powershell
cmake --build build --config Debug
ctest --test-dir build -C Debug -R "TestSuiteName" --output-on-failure
```

Always run the specific new/modified tests. Fix any compilation or assertion failures before considering the task complete.
