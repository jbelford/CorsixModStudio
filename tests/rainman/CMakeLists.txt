find_package(GTest CONFIG REQUIRED)

add_executable(rainman_tests
    placeholder_test.cpp
    exception_test.cpp
    memorystore_test.cpp
    crc32_case_idt_test.cpp
)

target_link_libraries(rainman_tests
    PRIVATE
        rainman
        GTest::gtest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(rainman_tests)
