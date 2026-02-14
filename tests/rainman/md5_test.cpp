#include <gtest/gtest.h>
extern "C" {
#include "md5.h"
}
#include <cstring>
#include <cstdio>

namespace {

// Helper to convert 16-byte digest to hex string
std::string DigestToHex(const unsigned char digest[16]) {
    char hex[33];
    for (int i = 0; i < 16; ++i) {
        sprintf(hex + i * 2, "%02x", digest[i]);
    }
    hex[32] = '\0';
    return std::string(hex);
}

std::string ComputeMD5(const char* input, size_t len) {
    MD5Context ctx;
    unsigned char digest[16];
    MD5Init(&ctx);
    MD5Update(&ctx, reinterpret_cast<const unsigned char*>(input), static_cast<unsigned>(len));
    MD5Final(digest, &ctx);
    return DigestToHex(digest);
}

} // namespace

// RFC 1321 test vectors
TEST(MD5, EmptyString) {
    EXPECT_EQ(ComputeMD5("", 0), "d41d8cd98f00b204e9800998ecf8427e");
}

TEST(MD5, SingleCharA) {
    EXPECT_EQ(ComputeMD5("a", 1), "0cc175b9c0f1b6a831c399e269772661");
}

TEST(MD5, ABC) {
    EXPECT_EQ(ComputeMD5("abc", 3), "900150983cd24fb0d6963f7d28e17f72");
}

TEST(MD5, MessageDigest) {
    const char* input = "message digest";
    EXPECT_EQ(ComputeMD5(input, strlen(input)), "f96b697d7cb7938d525a2f31aaf161d0");
}

TEST(MD5, AlphabetLowercase) {
    const char* input = "abcdefghijklmnopqrstuvwxyz";
    EXPECT_EQ(ComputeMD5(input, strlen(input)), "c3fcd3d76192e4007dfb496cca67e13b");
}

TEST(MD5, InitKeyEquivalence) {
    // MD5InitKey(ctx, s) should equal MD5Init + MD5Update(s)
    const char* key = "test_key";

    MD5Context ctx1;
    unsigned char digest1[16];
    MD5Init(&ctx1);
    MD5Update(&ctx1, reinterpret_cast<const unsigned char*>(key),
              static_cast<unsigned>(strlen(key)));
    MD5Final(digest1, &ctx1);

    MD5Context ctx2;
    unsigned char digest2[16];
    MD5InitKey(&ctx2, key);
    MD5Final(digest2, &ctx2);

    EXPECT_EQ(memcmp(digest1, digest2, 16), 0);
}

TEST(MD5, MultiPartUpdateConsistency) {
    // Hashing "abcdef" in one call vs two parts should produce same result
    const char* full = "abcdef";
    std::string fullHash = ComputeMD5(full, 6);

    MD5Context ctx;
    unsigned char digest[16];
    MD5Init(&ctx);
    MD5Update(&ctx, reinterpret_cast<const unsigned char*>("abc"), 3);
    MD5Update(&ctx, reinterpret_cast<const unsigned char*>("def"), 3);
    MD5Final(digest, &ctx);

    EXPECT_EQ(DigestToHex(digest), fullHash);
}

TEST(MD5, LargeInputCrossesBoundary) {
    // 128 bytes — crosses the 64-byte internal block boundary
    char input[128];
    memset(input, 'X', sizeof(input));

    // Just verify it doesn't crash and produces a consistent result
    std::string hash1 = ComputeMD5(input, sizeof(input));
    std::string hash2 = ComputeMD5(input, sizeof(input));
    EXPECT_EQ(hash1, hash2);
    EXPECT_EQ(hash1.length(), 32u);
}
