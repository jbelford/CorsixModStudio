#include <gtest/gtest.h>
#include "Internal_Util.h"
#include <cstring>

TEST(InternalUtil, MystrdupCreatesExactCopy) {
    char* copy = Util_mystrdup("hello world");
    ASSERT_NE(copy, nullptr);
    EXPECT_STREQ(copy, "hello world");
    delete[] copy;
}

TEST(InternalUtil, MystrdupEmptyString) {
    char* copy = Util_mystrdup("");
    ASSERT_NE(copy, nullptr);
    EXPECT_STREQ(copy, "");
    delete[] copy;
}

TEST(InternalUtil, StrtolowerConvertsToLowercase) {
    char buf[] = "Hello WORLD 123";
    Util_strtolower(buf);
    EXPECT_STREQ(buf, "hello world 123");
}

TEST(InternalUtil, StrtolowerAlreadyLowercase) {
    char buf[] = "already lower";
    Util_strtolower(buf);
    EXPECT_STREQ(buf, "already lower");
}

TEST(InternalUtil, StrtolowerEmptyString) {
    char buf[] = "";
    Util_strtolower(buf);
    EXPECT_STREQ(buf, "");
}

TEST(InternalUtil, TrimWhitespaceLeadingSpaces) {
    char buf[] = "   trimmed";
    char* p = buf;
    Util_TrimWhitespace(&p);
    EXPECT_STREQ(p, "trimmed");
}

TEST(InternalUtil, TrimWhitespaceTrailingSpaces) {
    char buf[] = "trimmed   ";
    char* p = buf;
    Util_TrimWhitespace(&p);
    EXPECT_STREQ(p, "trimmed");
}

TEST(InternalUtil, TrimWhitespaceBothEnds) {
    char buf[] = "  \t hello \r\n ";
    char* p = buf;
    Util_TrimWhitespace(&p);
    EXPECT_STREQ(p, "hello");
}

TEST(InternalUtil, TrimWhitespaceNoWhitespace) {
    char buf[] = "clean";
    char* p = buf;
    Util_TrimWhitespace(&p);
    EXPECT_STREQ(p, "clean");
}

TEST(InternalUtil, TrimWhitespaceTabsAndCRLF) {
    char buf[] = "\t\r\nvalue\t\r\n";
    char* p = buf;
    Util_TrimWhitespace(&p);
    EXPECT_STREQ(p, "value");
}

TEST(InternalUtil, AutoDeleteSingle) {
    int* p = new int(42);
    {
        AutoDelete<int> ad(p, false);
        EXPECT_EQ(*ad.p, 42);
    }
    // p is deleted by AutoDelete destructor — no leak
}

TEST(InternalUtil, AutoDeleteArray) {
    int* p = new int[5]{1, 2, 3, 4, 5};
    {
        AutoDelete<int> ad(p, true);
        EXPECT_EQ(ad.p[0], 1);
        EXPECT_EQ(ad.p[4], 5);
    }
}
