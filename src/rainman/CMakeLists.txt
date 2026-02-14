# Lua 5.0.2 - vendored static library
# Only the core library + standard libs (no lua/luac executables)
add_library(lua502 STATIC
    lua502/lapi.c
    lua502/lcode.c
    lua502/ldebug.c
    lua502/ldo.c
    lua502/ldump.c
    lua502/lfunc.c
    lua502/lgc.c
    lua502/llex.c
    lua502/lmem.c
    lua502/lobject.c
    lua502/lopcodes.c
    lua502/lparser.c
    lua502/lstate.c
    lua502/lstring.c
    lua502/ltable.c
    lua502/ltm.c
    lua502/lundump.c
    lua502/lvm.c
    lua502/lzio.c
    # Standard libraries
    lua502/lbaselib.c
    lua502/ldblib.c
    lua502/liolib.c
    lua502/lmathlib.c
    lua502/loadlib.c
    lua502/lstrlib.c
    lua502/ltablib.c
    lua502/lauxlib.c
)

target_include_directories(lua502 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lua502)
target_compile_definitions(lua502 PRIVATE _CRT_SECURE_NO_WARNINGS)
set_target_properties(lua502 PROPERTIES C_STANDARD 99)

# Lua 5.1.2 - vendored static library with renamed symbols
# All public symbols are prefixed lua51_ to avoid conflicts with Lua 5.0.2
add_library(lua512 STATIC
    lua512src/lapi.c
    lua512src/lauxlib.c
    lua512src/lbaselib.c
    lua512src/lcode.c
    lua512src/ldblib.c
    lua512src/ldebug.c
    lua512src/ldo.c
    lua512src/ldump.c
    lua512src/lfunc.c
    lua512src/lgc.c
    lua512src/linit.c
    lua512src/liolib.c
    lua512src/llex.c
    lua512src/lmathlib.c
    lua512src/lmem.c
    lua512src/loadlib.c
    lua512src/lobject.c
    lua512src/lopcodes.c
    lua512src/loslib.c
    lua512src/lparser.c
    lua512src/lstate.c
    lua512src/lstring.c
    lua512src/lstrlib.c
    lua512src/ltable.c
    lua512src/ltablib.c
    lua512src/ltm.c
    lua512src/lundump.c
    lua512src/lvm.c
    lua512src/lzio.c
)

target_include_directories(lua512 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lua512src)
target_compile_definitions(lua512 PRIVATE _CRT_SECURE_NO_WARNINGS)
set_target_properties(lua512 PROPERTIES C_STANDARD 99)
# Force-include the rename header so all symbols get lua51_ prefix
if(MSVC)
    target_compile_options(lua512 PRIVATE /FI"lua512_rename.h")
else()
    target_compile_options(lua512 PRIVATE -include lua512_rename.h)
endif()

# Rainman library
find_package(ZLIB REQUIRED)
find_package(unofficial-libsquish CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

file(GLOB RAINMAN_SOURCES "*.cpp" "*.c")
# Exclude gnuc_defines.cpp on MSVC (it provides GNUC compatibility shims)
if(MSVC)
    list(FILTER RAINMAN_SOURCES EXCLUDE REGEX "gnuc_defines\\.cpp$")
endif()

add_library(rainman STATIC ${RAINMAN_SOURCES})

target_include_directories(rainman
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include/rainman
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/lua502
)

target_link_libraries(rainman
    PUBLIC
        lua502
        lua512
        ZLIB::ZLIB
        spdlog::spdlog
    PRIVATE
        unofficial::libsquish::squish
)

if(WIN32)
    target_link_libraries(rainman PRIVATE shlwapi)
endif()

target_compile_definitions(rainman
    PUBLIC
        RAINMAN_NO_EXPORTS
    PRIVATE
        _CRT_SECURE_NO_WARNINGS
        _CRT_NONSTDC_NO_DEPRECATE
)

if(MSVC)
    target_compile_options(rainman PRIVATE
        /W3
        /wd4251  # DLL interface warning (keeping for compatibility)
        /wd4996  # Deprecated CRT functions
    )
endif()

# Precompiled header for C++ sources (skip C files — PCH has C++ content)
set_source_files_properties(crc32_case_idt.c hash.c md5.c
    PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
target_precompile_headers(rainman PRIVATE pch_rainman.h)
