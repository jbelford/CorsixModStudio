cmake_minimum_required(VERSION 3.20)

# vcpkg toolchain integration (must be set before project())
if(DEFINED ENV{VCPKG_ROOT} AND EXISTS "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

project(CorsixModStudio
    VERSION 0.5.6
    DESCRIPTION "Dawn of War / Company of Heroes Modding IDE"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# Release build optimizations (MSVC)
#
# /GL + /LTCG  — Whole Program Optimization / Link-Time Code Generation
#                Enables cross-module inlining, const propagation, devirtualization.
# /Gw          — Package global data in individual COMDATs so the linker can
#                discard unreferenced globals.
# /Zi + /DEBUG — Generate PDB debug info even in Release (zero runtime cost,
#                enables post-mortem crash debugging).
# /OPT:REF    — Remove unreferenced functions and data.
# /OPT:ICF    — Fold identical COMDATs (reduces binary size).
# ---------------------------------------------------------------------------

# Link-Time Code Generation (maps to /GL compile + /LTCG link on MSVC)
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
if(ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif()

if(MSVC)
    # Build performance: multi-processor compilation
    add_compile_options(/MP)

    # Release-only compiler flags
    add_compile_options(
        $<$<CONFIG:Release>:/Gw>
        $<$<CONFIG:Release>:/Zi>
    )

    # Release-only linker flags
    add_link_options(
        $<$<CONFIG:Release>:/OPT:REF>
        $<$<CONFIG:Release>:/OPT:ICF>
        $<$<CONFIG:Release>:/DEBUG>
    )
endif()


# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Enable testing
include(CTest)
enable_testing()

# Parallel test execution: use all available cores
include(ProcessorCount)
ProcessorCount(NPROC)
if(NPROC EQUAL 0)
    set(NPROC 4)
endif()
set(CTEST_PARALLEL_LEVEL ${NPROC} CACHE STRING "Number of parallel tests")

# Subdirectories
add_subdirectory(src/rainman)
add_subdirectory(src/cdms)

if(BUILD_TESTING)
    add_subdirectory(tests/rainman)
    add_subdirectory(tests/cdms)
endif()
