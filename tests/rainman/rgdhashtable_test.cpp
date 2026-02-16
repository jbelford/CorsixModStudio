#include <gtest/gtest.h>
#include "rainman/formats/CRgdHashTable.h"
#include <vector>
#include <cstring>

class RgdHashTableTest : public ::testing::Test {
protected:
    CRgdHashTable table;
};

TEST_F(RgdHashTableTest, ConstructorPrePopulatesNumericKeys) {
    // Constructor pre-populates keys "1" through "9999"
    const char* val = table.HashToValue(table.ValueToHash("42"));
    ASSERT_NE(val, nullptr);
    EXPECT_STREQ(val, "42");
}

TEST_F(RgdHashTableTest, ValueToHashAndHashToValueRoundTrip) {
    unsigned long h = table.ValueToHash("weapon_damage");
    const char* resolved = table.HashToValue(h);
    ASSERT_NE(resolved, nullptr);
    EXPECT_STREQ(resolved, "weapon_damage");
}

TEST_F(RgdHashTableTest, ValueToHashRegistersUnknownString) {
    // A non-numeric string not already in the table gets auto-registered
    unsigned long h = table.ValueToHash("my_custom_key");
    const char* resolved = table.HashToValue(h);
    ASSERT_NE(resolved, nullptr);
    EXPECT_STREQ(resolved, "my_custom_key");
}

TEST_F(RgdHashTableTest, ValueToHashStaticConsistentWithValueToHash) {
    unsigned long h1 = table.ValueToHash("test_value");
    unsigned long h2 = CRgdHashTable::ValueToHashStatic("test_value");
    EXPECT_EQ(h1, h2);
}

TEST_F(RgdHashTableTest, ValueToHashStaticWithLengthConsistent) {
    const char* val = "test_value";
    unsigned long h1 = CRgdHashTable::ValueToHashStatic(val);
    unsigned long h2 = CRgdHashTable::ValueToHashStatic(val, strlen(val));
    EXPECT_EQ(h1, h2);
}

TEST_F(RgdHashTableTest, HexStringPassthrough) {
    // A 10-char hex string "0xDEADBEEF" should be parsed as a literal hash
    unsigned long h = table.ValueToHash("0xDEADBEEF");
    EXPECT_EQ(h, 0xDEADBEEFUL);
}

TEST_F(RgdHashTableTest, HexStringPassthroughLowercase) {
    unsigned long h = table.ValueToHash("0xdeadbeef");
    EXPECT_EQ(h, 0xDEADBEEFUL);
}

TEST_F(RgdHashTableTest, HexStringStaticPassthrough) {
    unsigned long h = CRgdHashTable::ValueToHashStatic("0xABCD1234");
    EXPECT_EQ(h, 0xABCD1234UL);
}

TEST_F(RgdHashTableTest, FillUnknownListTracksUnresolvedHashes) {
    // Look up a hash that doesn't exist — it should be tracked as unknown
    unsigned long fakeHash = 0x12345678UL;
    const char* result = table.HashToValue(fakeHash);
    EXPECT_EQ(result, nullptr);

    std::vector<unsigned long> unknowns;
    table.FillUnknownList(unknowns);
    bool found = false;
    for (unsigned long h : unknowns) {
        if (h == fakeHash) { found = true; break; }
    }
    EXPECT_TRUE(found);
}

TEST_F(RgdHashTableTest, ValueToHashNumericNotCustom) {
    // Numeric-only strings (like "42") are pre-populated and not flagged custom
    unsigned long h = table.ValueToHash("42");
    const char* val = table.HashToValue(h);
    ASSERT_NE(val, nullptr);
    EXPECT_STREQ(val, "42");
}

TEST_F(RgdHashTableTest, DifferentStringsDifferentHashes) {
    unsigned long h1 = table.ValueToHash("alpha");
    unsigned long h2 = table.ValueToHash("beta");
    EXPECT_NE(h1, h2);
}

TEST_F(RgdHashTableTest, SameStringProducesSameHash) {
    unsigned long h1 = table.ValueToHash("consistent_key");
    unsigned long h2 = table.ValueToHash("consistent_key");
    EXPECT_EQ(h1, h2);
}
