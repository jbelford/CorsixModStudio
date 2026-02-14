#include <gtest/gtest.h>
#include <wx/init.h>
#include "Utility.h"
#include <cstring>
#include <string>

class UtilityTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() { wxInitialize(); }
    static void TearDownTestSuite() { wxUninitialize(); }
};

TEST_F(UtilityTest, SimpleConstructor) {
    auto* pE = new CModStudioException("test.cpp", 42, "simple error");
    EXPECT_STREQ(pE->getFile(), "test.cpp");
    EXPECT_EQ(pE->getLine(), 42u);
    EXPECT_STREQ(pE->getMessage(), "simple error");
    EXPECT_EQ(pE->getPrecursor(), nullptr);
    pE->destroy();
}

TEST_F(UtilityTest, ConstructorWithPrecursor) {
    auto* pPrecursor = new CRainmanException("inner.cpp", 10, "inner error");
    auto* pE = new CModStudioException("outer.cpp", 20, "outer error", pPrecursor);
    EXPECT_STREQ(pE->getMessage(), "outer error");
    EXPECT_NE(pE->getPrecursor(), nullptr);
    EXPECT_STREQ(pE->getPrecursor()->getMessage(), "inner error");
    pE->destroy(); // also destroys precursor
}

TEST_F(UtilityTest, VariadicFormatConstructor) {
    auto* pE = new CModStudioException(nullptr, "fmt.cpp", 99, "error %d: %s", 404, "not found");
    EXPECT_STREQ(pE->getFile(), "fmt.cpp");
    EXPECT_EQ(pE->getLine(), 99u);
    EXPECT_STREQ(pE->getMessage(), "error 404: not found");
    EXPECT_EQ(pE->getPrecursor(), nullptr);
    pE->destroy();
}

TEST_F(UtilityTest, VariadicFormatWithPrecursor) {
    auto* pPrecursor = new CRainmanException("base.cpp", 1, "base");
    auto* pE = new CModStudioException(pPrecursor, "wrap.cpp", 2, "wrapped: %s", "detail");
    EXPECT_STREQ(pE->getMessage(), "wrapped: detail");
    EXPECT_NE(pE->getPrecursor(), nullptr);
    EXPECT_STREQ(pE->getPrecursor()->getMessage(), "base");
    pE->destroy();
}

TEST_F(UtilityTest, VariadicFormatLongMessage) {
    // Build a format string that exceeds the initial 128-byte buffer
    std::string longPart(200, 'X');
    auto* pE = new CModStudioException(nullptr, "long.cpp", 5, "msg: %s", longPart.c_str());
    std::string expected = "msg: " + longPart;
    EXPECT_STREQ(pE->getMessage(), expected.c_str());
    pE->destroy();
}

TEST_F(UtilityTest, DestroyWithNullPrecursor) {
    auto* pE = new CModStudioException("file.cpp", 1, "no precursor", nullptr);
    // Should not crash
    pE->destroy();
}

TEST_F(UtilityTest, DestroyCleansPrecursorChain) {
    auto* pInner = new CRainmanException("a.cpp", 1, "level 1");
    auto* pMiddle = new CModStudioException("b.cpp", 2, "level 2", pInner);
    auto* pOuter = new CModStudioException("c.cpp", 3, "level 3", pMiddle);
    // Verify three-level chain
    EXPECT_NE(pOuter->getPrecursor(), nullptr);
    EXPECT_NE(pOuter->getPrecursor()->getPrecursor(), nullptr);
    // Should not crash or leak
    pOuter->destroy();
}
