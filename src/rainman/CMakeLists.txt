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

# Rainman library
find_package(ZLIB REQUIRED)
find_package(unofficial-libsquish CONFIG REQUIRED)

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
        ${CMAKE_CURRENT_SOURCE_DIR}/lua512
)

target_link_libraries(rainman
    PUBLIC
        lua502
        ZLIB::ZLIB
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
