find_package(GTest CONFIG REQUIRED)

add_executable(rainman_tests
    test_main.cpp
    placeholder_test.cpp
    exception_test.cpp
    memorystore_test.cpp
    crc32_case_idt_test.cpp
    filesystemstore_test.cpp
    sgafile_test.cpp
    rgdfile_test.cpp
    ucsfile_test.cpp
    chunkyfile_test.cpp
    modulefile_test.cpp
    rgdhashtable_test.cpp
    inherittable_test.cpp
    internal_util_test.cpp
    md5_test.cpp
    callbacks_test.cpp
    writetime_test.cpp
)

target_link_libraries(rainman_tests
    PRIVATE
        rainman
        GTest::gtest
)

include(GoogleTest)
gtest_discover_tests(rainman_tests)
