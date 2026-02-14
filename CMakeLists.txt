cmake_minimum_required(VERSION 3.20)

# vcpkg toolchain integration (must be set before project())
if(DEFINED ENV{VCPKG_ROOT} AND EXISTS "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

project(CorsixModStudio
    VERSION 0.55.0
    DESCRIPTION "Dawn of War / Company of Heroes Modding IDE"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
