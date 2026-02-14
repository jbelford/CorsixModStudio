#include <gtest/gtest.h>
#include <wx/init.h>
#include "strconv.h"
#include <cstdlib>

class StrconvTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() { wxInitialize(); }
    static void TearDownTestSuite() { wxUninitialize(); }
};

TEST_F(StrconvTest, AsciiToUnicodeNormalString) {
    wchar_t* result = AsciiToUnicode("hello");
    ASSERT_NE(result, nullptr);
    EXPECT_STREQ(result, L"hello");
    delete[] result;
}

TEST_F(StrconvTest, AsciiToUnicodeEmptyString) {
    wchar_t* result = AsciiToUnicode("");
    ASSERT_NE(result, nullptr);
    EXPECT_STREQ(result, L"");
    delete[] result;
}

TEST_F(StrconvTest, UnicodeToAsciiNormalString) {
    char* result = UnicodeToAscii(L"hello");
    ASSERT_NE(result, nullptr);
    EXPECT_STREQ(result, "hello");
    delete[] result;
}

TEST_F(StrconvTest, UnicodeToAsciiOutOfRangeBecomesQuestionMark) {
    // Characters > 0xFF should become '?'
    wchar_t input[] = { L'A', 0x0100, L'B', 0x00 };
    char* result = UnicodeToAscii(input);
    ASSERT_NE(result, nullptr);
    EXPECT_STREQ(result, "A?B");
    delete[] result;
}

TEST_F(StrconvTest, UnicodeToAsciiEmptyString) {
    char* result = UnicodeToAscii(L"");
    ASSERT_NE(result, nullptr);
    EXPECT_STREQ(result, "");
    delete[] result;
}

TEST_F(StrconvTest, AsciiToUnicodeDelConvertsAndDeletesSource) {
    char* source = new char[6];
    std::memcpy(source, "world", 6);
    wchar_t* result = AsciiToUnicodeDel(source);
    // source is now deleted by the function
    ASSERT_NE(result, nullptr);
    EXPECT_STREQ(result, L"world");
    delete[] result;
}

TEST_F(StrconvTest, AsciiToUnicodeFreeConvertsAndFreesSource) {
    char* source = static_cast<char*>(std::malloc(4));
    std::memcpy(source, "abc", 4);
    wchar_t* result = AsciiToUnicodeFree(source);
    // source is now freed by the function
    ASSERT_NE(result, nullptr);
    EXPECT_STREQ(result, L"abc");
    delete[] result;
}

TEST_F(StrconvTest, AsciiTowxStringNormalConversion) {
    wxString result = AsciiTowxString("test");
    EXPECT_EQ(result, wxT("test"));
}

TEST_F(StrconvTest, AsciiTowxStringNullReturnsEmpty) {
    wxString result = AsciiTowxString(nullptr);
    EXPECT_TRUE(result.IsEmpty());
}

TEST_F(StrconvTest, wxStringToAsciiRoundTrip) {
    wxString input(wxT("round trip"));
    char* result = wxStringToAscii(input);
    ASSERT_NE(result, nullptr);
    EXPECT_STREQ(result, "round trip");
    delete[] result;
}
