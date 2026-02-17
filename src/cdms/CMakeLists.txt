# CDMS GUI application
find_package(wxWidgets REQUIRED COMPONENTS core base aui html xml xrc propgrid stc adv)
include(${wxWidgets_USE_FILE})

file(GLOB CDMS_SOURCES "*.cpp" "*.c" "common/*.cpp" "frame/*.cpp" "services/*.cpp" "actions/*.cpp" "async/*.cpp" "presenters/*.cpp" "views/*.cpp")

add_executable(CorsixModStudio WIN32 ${CDMS_SOURCES} resource.rc)

target_include_directories(CorsixModStudio PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${wxWidgets_INCLUDE_DIRS}
)

target_link_libraries(CorsixModStudio PRIVATE
    rainman
    ${wxWidgets_LIBRARIES}
)

if(WIN32)
    target_link_libraries(CorsixModStudio PRIVATE
        winmm comctl32 rpcrt4 wsock32 oleacc odbc32
    )
endif()

target_compile_definitions(CorsixModStudio PRIVATE
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
    UNICODE
    _UNICODE
    WIN32_LEAN_AND_MEAN
)

if(MSVC)
    target_compile_options(CorsixModStudio PRIVATE
        /W3
        /wd4996
        /wd4267
        /wd4244
    )
    # Disable MSVC auto-generated manifest — wx.rc embeds its own with
    # Per-Monitor V2 DPI awareness (wxUSE_DPI_AWARE_MANIFEST=2).
    target_link_options(CorsixModStudio PRIVATE /MANIFEST:NO)
endif()

# Copy Mod_Studio_Files (config, dictionaries, docs, etc.) next to the exe
add_custom_command(TARGET CorsixModStudio POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Mod_Studio_Files"
        "$<TARGET_FILE_DIR:CorsixModStudio>/Mod_Studio_Files"
    COMMENT "Copying Mod_Studio_Files to output directory"
)
