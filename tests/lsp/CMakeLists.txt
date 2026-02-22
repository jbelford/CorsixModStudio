find_package(GTest CONFIG REQUIRED)

add_executable(lsp_tests
    jsonrpc_test.cpp
    protocol_test.cpp
)

target_link_libraries(lsp_tests
    PRIVATE
        lsp
        GTest::gtest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(lsp_tests)
