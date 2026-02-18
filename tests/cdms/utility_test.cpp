#include <gtest/gtest.h>
#include <wx/init.h>
#include "common/Utility.h"
#include <cstring>
#include <string>

class UtilityTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() { wxInitialize(); }
    static void TearDownTestSuite() { wxUninitialize(); }
};

TEST_F(UtilityTest, SimpleConstructor) {
    CModStudioException ex("test.cpp", 42, "simple error");
    EXPECT_STREQ(ex.getFile(), "test.cpp");
    EXPECT_EQ(ex.getLine(), 42u);
    EXPECT_STREQ(ex.getMessage(), "simple error");
    EXPECT_EQ(ex.getPrecursor(), nullptr);
}

TEST_F(UtilityTest, ConstructorWithPrecursor) {
    CRainmanException precursor("inner.cpp", 10, "inner error");
    CModStudioException ex(precursor, "outer.cpp", 20, "outer error");
    EXPECT_STREQ(ex.getMessage(), "outer error");
    EXPECT_NE(ex.getPrecursor(), nullptr);
    EXPECT_STREQ(ex.getPrecursor()->getMessage(), "inner error");
}

TEST_F(UtilityTest, VariadicFormatConstructor) {
    CModStudioException ex(nullptr, "fmt.cpp", 99, "error %d: %s", 404, "not found");
    EXPECT_STREQ(ex.getFile(), "fmt.cpp");
    EXPECT_EQ(ex.getLine(), 99u);
    EXPECT_STREQ(ex.getMessage(), "error 404: not found");
    EXPECT_EQ(ex.getPrecursor(), nullptr);
}

TEST_F(UtilityTest, VariadicFormatWithPrecursor) {
    CRainmanException precursor("base.cpp", 1, "base");
    CModStudioException ex(precursor, "wrap.cpp", 2, "wrapped: %s", "detail");
    EXPECT_STREQ(ex.getMessage(), "wrapped: detail");
    EXPECT_NE(ex.getPrecursor(), nullptr);
    EXPECT_STREQ(ex.getPrecursor()->getMessage(), "base");
}

TEST_F(UtilityTest, VariadicFormatLongMessage) {
    // Build a format string that exceeds the initial 128-byte buffer
    std::string longPart(200, 'X');
    CModStudioException ex(nullptr, "long.cpp", 5, "msg: %s", longPart.c_str());
    std::string expected = "msg: " + longPart;
    EXPECT_STREQ(ex.getMessage(), expected.c_str());
}

TEST_F(UtilityTest, DestroyWithNullPrecursor) {
    CModStudioException ex("file.cpp", 1, "no precursor");
    // Should not crash
}

TEST_F(UtilityTest, DestroyCleansPrecursorChain) {
    CRainmanException inner("a.cpp", 1, "level 1");
    CModStudioException middle(inner, "b.cpp", 2, "level 2");
    CModStudioException outer(middle, "c.cpp", 3, "level 3");
    // Verify three-level chain
    EXPECT_NE(outer.getPrecursor(), nullptr);
    EXPECT_NE(outer.getPrecursor()->getPrecursor(), nullptr);
    // Should not crash or leak
}
