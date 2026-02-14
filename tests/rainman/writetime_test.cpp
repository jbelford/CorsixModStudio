#include <gtest/gtest.h>
#include "WriteTime.h"

TEST(WriteTimeTest, GetInvalidWriteTimeReturnsZero) {
    EXPECT_EQ(GetInvalidWriteTime(), 0ULL);
}

TEST(WriteTimeTest, InvalidWriteTimeIsNotValid) {
    EXPECT_FALSE(IsValidWriteTime(GetInvalidWriteTime()));
}

TEST(WriteTimeTest, NonZeroTimeIsValid) {
    EXPECT_TRUE(IsValidWriteTime(1ULL));
    EXPECT_TRUE(IsValidWriteTime(1000000ULL));
}

TEST(WriteTimeTest, NewerTimeIsNewer) {
    tLastWriteTime older = 100;
    tLastWriteTime newer = 200;
    EXPECT_TRUE(IsWriteTimeNewer(newer, older));
}

TEST(WriteTimeTest, OlderTimeIsNotNewer) {
    tLastWriteTime older = 100;
    tLastWriteTime newer = 200;
    EXPECT_FALSE(IsWriteTimeNewer(older, newer));
}

TEST(WriteTimeTest, EqualTimesAreNotNewer) {
    tLastWriteTime t = 500;
    EXPECT_FALSE(IsWriteTimeNewer(t, t));
}

TEST(WriteTimeTest, ZeroIsNotNewerThanAnything) {
    EXPECT_FALSE(IsWriteTimeNewer(0ULL, 1ULL));
}

TEST(WriteTimeTest, AnythingIsNewerThanZero) {
    EXPECT_TRUE(IsWriteTimeNewer(1ULL, 0ULL));
}
