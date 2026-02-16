#include <gtest/gtest.h>
#include "rainman/core/Exception.h"

TEST(CRainmanException, BasicCreation) {
    auto* ex = new CRainmanException("test.cpp", 42, "test error");
    EXPECT_STREQ(ex->getFile(), "test.cpp");
    EXPECT_EQ(ex->getLine(), 42u);
    EXPECT_STREQ(ex->getMessage(), "test error");
    EXPECT_EQ(ex->getPrecursor(), nullptr);
    ex->destroy();
}

TEST(CRainmanException, ChainedExceptions) {
    auto* inner = new CRainmanException("inner.cpp", 10, "inner error");
    auto* outer = new CRainmanException("outer.cpp", 20, "outer error", inner);

    EXPECT_STREQ(outer->getFile(), "outer.cpp");
    EXPECT_EQ(outer->getLine(), 20u);
    EXPECT_STREQ(outer->getMessage(), "outer error");

    const auto* precursor = outer->getPrecursor();
    ASSERT_NE(precursor, nullptr);
    EXPECT_STREQ(precursor->getFile(), "inner.cpp");
    EXPECT_STREQ(precursor->getMessage(), "inner error");

    outer->destroy();
}

TEST(CRainmanException, FormattedMessage) {
    auto* ex = new CRainmanException(nullptr, "test.cpp", 99, "error %d: %s", 404, "not found");
    EXPECT_STREQ(ex->getFile(), "test.cpp");
    EXPECT_EQ(ex->getLine(), 99u);
    EXPECT_NE(strstr(ex->getMessage(), "404"), nullptr);
    EXPECT_NE(strstr(ex->getMessage(), "not found"), nullptr);
    ex->destroy();
}

TEST(CRainmanException, QuickThrowMacro) {
    EXPECT_THROW({
        QUICK_THROW("macro test");
    }, CRainmanException*);
}

TEST(CRainmanException, CatchThrowMacro) {
    EXPECT_THROW({
        try {
            QUICK_THROW("original");
        }
        CATCH_THROW("rethrown");
    }, CRainmanException*);
}

TEST(CRainmanException, CheckMemValid) {
    int* p = new int(42);
    EXPECT_EQ(CHECK_MEM(p), p);
    delete p;
}

TEST(CRainmanException, CheckMemNull) {
    EXPECT_THROW({
        CHECK_MEM((int*)nullptr);
    }, CRainmanException*);
}
