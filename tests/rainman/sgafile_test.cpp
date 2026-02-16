#include <gtest/gtest.h>
#include "archive/CSgaFile.h"
#include "io/CMemoryStore.h"
#include "core/Exception.h"
#include <cstring>
#include <filesystem>
#include <fstream>

class SgaFileTest : public ::testing::Test {
protected:
    std::filesystem::path tempDir;

    void SetUp() override {
        tempDir = std::filesystem::temp_directory_path() /
            ("sga_test_" + std::to_string(reinterpret_cast<uintptr_t>(this)));
        std::filesystem::create_directories(tempDir);
    }

    void TearDown() override {
        std::filesystem::remove_all(tempDir);
    }
};

TEST_F(SgaFileTest, LoadFromNullStreamThrows) {
    CSgaFile sga;
    CRainmanException* caught = nullptr;
    try {
        sga.Load(nullptr);
    } catch (CRainmanException* ex) {
        caught = ex;
    }
    ASSERT_NE(caught, nullptr);
    caught->destroy();
}

TEST_F(SgaFileTest, LoadInvalidHeaderThrows) {
    // Create a stream with garbage data
    CMemoryStore store;
    store.VInit();

    const char garbage[] = "THIS_IS_NOT_AN_SGA_FILE_HEADER__";
    char* range = store.MemoryRange((void*)garbage, sizeof(garbage) - 1);
    auto* stream = store.VOpenStream(range);
    ASSERT_NE(stream, nullptr);

    CSgaFile sga;
    CRainmanException* caught = nullptr;
    try {
        sga.Load(stream);
    } catch (CRainmanException* ex) {
        caught = ex;
    }
    ASSERT_NE(caught, nullptr);
    caught->destroy();
    delete stream;
}

TEST_F(SgaFileTest, LoadTruncatedStreamThrows) {
    // Only 4 bytes - too short even for identifier
    CMemoryStore store;
    store.VInit();

    const char data[] = "_ARC";
    char* range = store.MemoryRange((void*)data, 4);
    auto* stream = store.VOpenStream(range);
    ASSERT_NE(stream, nullptr);

    CSgaFile sga;
    CRainmanException* caught = nullptr;
    try {
        sga.Load(stream);
    } catch (CRainmanException* ex) {
        caught = ex;
    }
    ASSERT_NE(caught, nullptr);
    caught->destroy();
    delete stream;
}

TEST_F(SgaFileTest, LoadValidIdentifierBadVersionThrows) {
    // Write "_ARCHIVE" + unsupported version (99)
    CMemoryStore store;
    store.VInit();
    auto* out = CMemoryStore::OpenOutputStreamExt();

    const char ident[] = "_ARCHIVE";
    out->VWrite(8, 1, ident);
    unsigned long version = 99;
    out->VWrite(1, sizeof(unsigned long), &version);

    auto* stream = CMemoryStore::OpenStreamExt(
        const_cast<char*>(out->GetData()), out->GetDataLength(), false);

    CSgaFile sga;
    CRainmanException* caught = nullptr;
    try {
        sga.Load(stream);
    } catch (CRainmanException* ex) {
        caught = ex;
    }
    ASSERT_NE(caught, nullptr);
    EXPECT_NE(std::strstr(caught->getMessage(), "not supported"), nullptr);
    caught->destroy();
    delete stream;
    delete out;
}

TEST_F(SgaFileTest, VCreateFolderInThrows) {
    CSgaFile sga;
    CRainmanException* caught = nullptr;
    try {
        sga.VCreateFolderIn("some/path", "folder");
    } catch (CRainmanException* ex) {
        caught = ex;
    }
    ASSERT_NE(caught, nullptr);
    caught->destroy();
}

TEST_F(SgaFileTest, GetInputStreamWithoutInitThrows) {
    CSgaFile sga;
    CRainmanException* caught = nullptr;
    try {
        sga.GetInputStream();
    } catch (CRainmanException* ex) {
        caught = ex;
    }
    ASSERT_NE(caught, nullptr);
    caught->destroy();
}
