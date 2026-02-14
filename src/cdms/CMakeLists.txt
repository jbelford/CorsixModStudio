# CDMS GUI application
find_package(wxWidgets REQUIRED COMPONENTS core base aui html xml xrc propgrid stc adv)
include(${wxWidgets_USE_FILE})

file(GLOB CDMS_SOURCES "*.cpp" "*.c")

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
endif()
