# Coverage Analysis Workflow

## Step 1: Inventory Source Files

Scan the source directory for all `.cpp`/`.h` pairs that represent testable units. Exclude:
- Pure header-only utility files (macros, typedefs)
- Vendored/third-party code (e.g. `lua502/`, `lua512/`, `zLib/`)
- Internal utility files that are implementation details of other classes

Focus on files that define a class with `C` or `I` prefix — these are the primary testable units.

## Step 2: Map Tests to Source

For each source file, check if a corresponding `*_test.cpp` exists in the test directory. Build a coverage matrix:

| Source File | Test File | Status |
|---|---|---|
| CSgaFile.cpp | sgafile_test.cpp | ✅ Covered |
| CSgaCreator.cpp | — | ❌ Missing |

## Step 3: Assess Test Depth

For files WITH tests, evaluate depth:
- **Shallow**: Only tests construction/destruction or trivial getters
- **Moderate**: Tests primary workflows and error paths
- **Deep**: Tests edge cases, integration with other components, round-trip scenarios

Rate each covered file and identify gaps even in "covered" code.

## Step 4: Prioritize

Rank untested files by:
1. **Complexity** — More complex classes benefit more from tests
2. **Risk** — Files that handle I/O, parsing, or memory management are higher risk
3. **Testability** — Some files may require extensive mocking or test infrastructure
4. **Dependencies** — Prefer testing leaf classes before classes with many dependencies

## Step 5: Generate Report

Output a structured coverage report with:
- Total source files, tested files, untested files
- Coverage percentage
- Prioritized list of files needing tests
- Specific test cases recommended for each untested file
- Depth assessment for existing tests
