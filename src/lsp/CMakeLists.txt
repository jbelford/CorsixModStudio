# LSP Client Library — static library, no wxWidgets dependency.
# Pure C++ + Win32 + nlohmann/json.

find_package(nlohmann_json CONFIG REQUIRED)

# Source files are auto-discovered via GLOB (same convention as Rainman/CDMS).
file(GLOB LSP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(lsp STATIC ${LSP_SOURCES})

target_include_directories(lsp
    PUBLIC ${CMAKE_SOURCE_DIR}/src
)

target_compile_definitions(lsp PUBLIC LSP_NO_EXPORTS)

target_link_libraries(lsp
    PUBLIC nlohmann_json::nlohmann_json
)

# Precompiled header (optional, can add later if build times warrant it)
