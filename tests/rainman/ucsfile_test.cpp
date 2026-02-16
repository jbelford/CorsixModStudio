#include <gtest/gtest.h>
#include "rainman/localization/CUcsFile.h"
#include "rainman/io/CMemoryStore.h"
#include "rainman/core/Exception.h"
#include <cstring>
#include <string>

#define EXPECT_WSTREQ(expected, actual) \
    EXPECT_EQ(std::wstring(expected), std::wstring(actual))

class UcsFileTest : public ::testing::Test {
protected:
    CUcsFile ucs;
};

TEST_F(UcsFileTest, NewCreatesEmptyFile) {
    ucs.New();
    auto* map = ucs.GetRawMap();
    ASSERT_NE(map, nullptr);
    EXPECT_TRUE(map->empty());
}

TEST_F(UcsFileTest, SetStringAndResolve) {
    ucs.New();
    ucs.SetString(100, L"Hello World");

    const wchar_t* result = ucs.ResolveStringID(100);
    ASSERT_NE(result, nullptr);
    EXPECT_WSTREQ(L"Hello World", result);
}

TEST_F(UcsFileTest, ResolveNonexistentReturnsNull) {
    ucs.New();
    // Note: ResolveStringID inserts a null entry for missing keys
    const wchar_t* result = ucs.ResolveStringID(999);
    EXPECT_EQ(result, nullptr);
}

TEST_F(UcsFileTest, SetMultipleStrings) {
    ucs.New();
    ucs.SetString(1, L"First");
    ucs.SetString(2, L"Second");
    ucs.SetString(3, L"Third");

    auto* map = ucs.GetRawMap();
    EXPECT_GE(map->size(), 3u);

    EXPECT_WSTREQ(L"First", ucs.ResolveStringID(1));
    EXPECT_WSTREQ(L"Second", ucs.ResolveStringID(2));
    EXPECT_WSTREQ(L"Third", ucs.ResolveStringID(3));
}

TEST_F(UcsFileTest, OverwriteString) {
    ucs.New();
    ucs.SetString(1, L"Original");
    ucs.SetString(1, L"Updated");

    EXPECT_WSTREQ(L"Updated", ucs.ResolveStringID(1));
}

TEST_F(UcsFileTest, SetStringNullRemoves) {
    ucs.New();
    ucs.SetString(1, L"Hello");
    ucs.SetString(1, nullptr);

    // After setting null, entry should be erased
    auto* map = ucs.GetRawMap();
    EXPECT_EQ(map->find(1), map->end());
}

TEST_F(UcsFileTest, ReplaceStringTransfersOwnership) {
    ucs.New();

    wchar_t* owned = new wchar_t[6];
    wcscpy(owned, L"Hello");
    ucs.ReplaceString(1, owned);

    EXPECT_WSTREQ(L"Hello", ucs.ResolveStringID(1));
    // `owned` is now managed by CUcsFile — no manual delete needed
}

TEST_F(UcsFileTest, LoadUcsFromMemory) {
    ucs.New();

    // Build a minimal UCS file in memory:
    // BOM (0xFEFF) + "1\tHello\r\n"
    CMemoryStore store;
    store.VInit();
    auto* out = CMemoryStore::OpenOutputStreamExt();

    unsigned short bom = 0xFEFF;
    out->VWrite(1, sizeof(unsigned short), &bom);

    // Write line: L"1\tHello\r\n"
    const wchar_t line[] = L"1\tHello\r\n";
    out->VWrite(static_cast<unsigned long>(wcslen(line)), sizeof(wchar_t), line);

    auto* inStream = CMemoryStore::OpenStreamExt(
        const_cast<char*>(out->GetData()), out->GetDataLength(), false);

    CUcsFile ucs2;
    ucs2.Load(inStream);

    const wchar_t* result = ucs2.ResolveStringID(1);
    ASSERT_NE(result, nullptr);
    EXPECT_WSTREQ(L"Hello", result);

    delete inStream;
    delete out;
}

TEST_F(UcsFileTest, LoadInvalidHeaderThrows) {
    CMemoryStore store;
    store.VInit();
    auto* out = CMemoryStore::OpenOutputStreamExt();

    // Write an invalid BOM
    unsigned short badBom = 0x1234;
    out->VWrite(1, sizeof(unsigned short), &badBom);

    auto* inStream = CMemoryStore::OpenStreamExt(
        const_cast<char*>(out->GetData()), out->GetDataLength(), false);

    CUcsFile ucs2;
    CRainmanException* caught = nullptr;
    try {
        ucs2.Load(inStream);
    } catch (CRainmanException* ex) {
        caught = ex;
    }
    ASSERT_NE(caught, nullptr);
    caught->destroy();
    delete inStream;
    delete out;
}

TEST_F(UcsFileTest, NewClearsExistingData) {
    ucs.New();
    ucs.SetString(1, L"First");
    ucs.SetString(2, L"Second");

    ucs.New(); // Should clear
    auto* map = ucs.GetRawMap();
    EXPECT_TRUE(map->empty());
}

TEST_F(UcsFileTest, IsDollarStringValid) {
    EXPECT_TRUE(CUcsFile::IsDollarString("$12345"));
    EXPECT_TRUE(CUcsFile::IsDollarString("$0"));
    EXPECT_TRUE(CUcsFile::IsDollarString("$999999"));
}

TEST_F(UcsFileTest, IsDollarStringInvalid) {
    EXPECT_FALSE(CUcsFile::IsDollarString("12345"));
    EXPECT_FALSE(CUcsFile::IsDollarString("$"));
    EXPECT_FALSE(CUcsFile::IsDollarString("$abc"));
    EXPECT_FALSE(CUcsFile::IsDollarString("$12a"));
    EXPECT_FALSE(CUcsFile::IsDollarString((const char*)nullptr));
    EXPECT_FALSE(CUcsFile::IsDollarString(""));
}

TEST_F(UcsFileTest, IsDollarStringWideValid) {
    EXPECT_TRUE(CUcsFile::IsDollarString(L"$12345"));
    EXPECT_TRUE(CUcsFile::IsDollarString(L"$0"));
}

TEST_F(UcsFileTest, IsDollarStringWideInvalid) {
    EXPECT_FALSE(CUcsFile::IsDollarString(L"12345"));
    EXPECT_FALSE(CUcsFile::IsDollarString(L"$"));
    EXPECT_FALSE(CUcsFile::IsDollarString(L"$abc"));
    EXPECT_FALSE(CUcsFile::IsDollarString((const wchar_t*)nullptr));
}

TEST_F(UcsFileTest, GetRawMapIsConsistent) {
    ucs.New();
    ucs.SetString(10, L"Ten");
    ucs.SetString(20, L"Twenty");

    auto* map = ucs.GetRawMap();
    ASSERT_NE(map, nullptr);

    auto it10 = map->find(10);
    ASSERT_NE(it10, map->end());
    EXPECT_WSTREQ(L"Ten", it10->second);

    auto it20 = map->find(20);
    ASSERT_NE(it20, map->end());
    EXPECT_WSTREQ(L"Twenty", it20->second);
}
