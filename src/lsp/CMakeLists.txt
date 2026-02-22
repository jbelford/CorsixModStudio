# LSP Client Library — static library, no wxWidgets dependency.
# Pure C++ + Win32 + nlohmann/json + spdlog.

find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# ---------------------------------------------------------------------------
# Auto-download LuaLS binary (Lua Language Server)
# Downloaded once at configure time; cached in build/luals/.
# ---------------------------------------------------------------------------
set(LUALS_VERSION "3.17.1")
set(LUALS_DIR "${CMAKE_BINARY_DIR}/luals")
set(LUALS_EXE "${LUALS_DIR}/bin/lua-language-server.exe")
set(LUALS_ZIP "${LUALS_DIR}/lua-language-server-${LUALS_VERSION}-win32-x64.zip")
set(LUALS_URL "https://github.com/LuaLS/lua-language-server/releases/download/${LUALS_VERSION}/lua-language-server-${LUALS_VERSION}-win32-x64.zip")

if(NOT EXISTS "${LUALS_EXE}")
    message(STATUS "Downloading LuaLS ${LUALS_VERSION}...")
    file(MAKE_DIRECTORY "${LUALS_DIR}")
    file(DOWNLOAD "${LUALS_URL}" "${LUALS_ZIP}"
        STATUS LUALS_DL_STATUS
        SHOW_PROGRESS
    )
    list(GET LUALS_DL_STATUS 0 LUALS_DL_CODE)
    if(NOT LUALS_DL_CODE EQUAL 0)
        list(GET LUALS_DL_STATUS 1 LUALS_DL_MSG)
        message(WARNING "Failed to download LuaLS: ${LUALS_DL_MSG}")
    else()
        message(STATUS "Extracting LuaLS to ${LUALS_DIR}...")
        file(ARCHIVE_EXTRACT INPUT "${LUALS_ZIP}" DESTINATION "${LUALS_DIR}")
        file(REMOVE "${LUALS_ZIP}")
        if(EXISTS "${LUALS_EXE}")
            message(STATUS "LuaLS ${LUALS_VERSION} ready at ${LUALS_EXE}")
        else()
            message(WARNING "LuaLS extraction succeeded but exe not found at expected path")
        endif()
    endif()
else()
    message(STATUS "LuaLS ${LUALS_VERSION} already present at ${LUALS_EXE}")
endif()

# Source files are auto-discovered via GLOB (same convention as Rainman/CDMS).
file(GLOB LSP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(lsp STATIC ${LSP_SOURCES})

target_include_directories(lsp
    PUBLIC ${CMAKE_SOURCE_DIR}/src
)

target_compile_definitions(lsp PUBLIC LSP_NO_EXPORTS RAINMAN_NO_EXPORTS)

target_link_libraries(lsp
    PUBLIC nlohmann_json::nlohmann_json
    PRIVATE spdlog::spdlog
)

# Export the LuaLS directory path so the CDMS target can copy it
set(LUALS_DIR "${LUALS_DIR}" PARENT_SCOPE)
