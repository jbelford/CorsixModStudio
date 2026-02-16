#include <gtest/gtest.h>
#include "rainman/util/crc32_case_idt.h"

TEST(Crc32CaseIdt, NullBufferReturnsZero) {
    EXPECT_EQ(crc32_case_idt(0, nullptr, 0), 0UL);
    EXPECT_EQ(crc32_case_idt(12345, nullptr, 10), 0UL);
}

TEST(Crc32CaseIdt, CaseInsensitive) {
    const unsigned char upper[] = "HELLO";
    const unsigned char lower[] = "hello";

    unsigned long crc_upper = crc32_case_idt(0, upper, 5);
    unsigned long crc_lower = crc32_case_idt(0, lower, 5);

    EXPECT_EQ(crc_upper, crc_lower);
}

TEST(Crc32CaseIdt, MixedCaseMatches) {
    const unsigned char mixed1[] = "GameData\\Attrib";
    const unsigned char mixed2[] = "gamedata\\attrib";
    const unsigned char mixed3[] = "GAMEDATA\\ATTRIB";

    unsigned long crc1 = crc32_case_idt(0, mixed1, 15);
    unsigned long crc2 = crc32_case_idt(0, mixed2, 15);
    unsigned long crc3 = crc32_case_idt(0, mixed3, 15);

    EXPECT_EQ(crc1, crc2);
    EXPECT_EQ(crc2, crc3);
}

TEST(Crc32CaseIdt, DifferentStringsProduceDifferentHashes) {
    const unsigned char str1[] = "hello";
    const unsigned char str2[] = "world";

    unsigned long crc1 = crc32_case_idt(0, str1, 5);
    unsigned long crc2 = crc32_case_idt(0, str2, 5);

    EXPECT_NE(crc1, crc2);
}

TEST(Crc32CaseIdt, EmptyStringProducesConsistentHash) {
    const unsigned char empty[] = "";
    unsigned long crc1 = crc32_case_idt(0, empty, 0);
    unsigned long crc2 = crc32_case_idt(0, empty, 0);
    EXPECT_EQ(crc1, crc2);
}

TEST(Crc32CaseIdt, LongStringHandledCorrectly) {
    // Test with >8 chars to exercise the unrolled loop
    const unsigned char longstr[] = "this_is_a_long_test_string.lua";
    const unsigned char longstr_upper[] = "THIS_IS_A_LONG_TEST_STRING.LUA";

    unsigned long crc1 = crc32_case_idt(0, longstr, (unsigned int)strlen((const char*)longstr));
    unsigned long crc2 = crc32_case_idt(0, longstr_upper, (unsigned int)strlen((const char*)longstr_upper));

    EXPECT_EQ(crc1, crc2);
}
