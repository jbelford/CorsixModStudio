#include <gtest/gtest.h>
#include "CFileSystemStore.h"
#include "Exception.h"
#include <cstring>
#include <filesystem>
#include <process.h>
#include <string>

class FileSystemStoreTest : public ::testing::Test {
protected:
    CFileSystemStore store;
    std::filesystem::path tempDir;

    void SetUp() override {
        tempDir = std::filesystem::temp_directory_path() / "cfs_test_XXXXXX";
        // Create a unique directory name using the test address as seed
        tempDir = std::filesystem::temp_directory_path() /
            ("cfs_test_" + std::to_string(_getpid()) + "_" + std::to_string(reinterpret_cast<uintptr_t>(this)));
        std::filesystem::create_directories(tempDir);
        store.VInit();
    }

    void TearDown() override {
        std::filesystem::remove_all(tempDir);
    }

    std::string filePath(const std::string& name) {
        return (tempDir / name).string();
    }

    // Helper: write data to a file via CFileSystemStore
    void writeFile(const std::string& name, const char* data, size_t len) {
        auto* os = store.VOpenOutputStream(filePath(name).c_str(), true);
        os->VWrite(static_cast<unsigned long>(len), 1, data);
        delete os;
    }
};

TEST_F(FileSystemStoreTest, InitSetsReady) {
    // VInit completes without throwing — store is usable
    CFileSystemStore s;
    EXPECT_NO_THROW(s.VInit());
}

TEST_F(FileSystemStoreTest, WriteAndReadBack) {
    const char data[] = "Hello, FileSystemStore!";
    const size_t len = sizeof(data) - 1;

    writeFile("roundtrip.bin", data, len);

    auto* stream = store.VOpenStream(filePath("roundtrip.bin").c_str());
    ASSERT_NE(stream, nullptr);

    char buf[64] = {};
    stream->VRead(static_cast<unsigned long>(len), 1, buf);
    EXPECT_EQ(std::memcmp(buf, data, len), 0);

    delete stream;
}

TEST_F(FileSystemStoreTest, ReadCorrectData) {
    const unsigned char bytes[] = {0x00, 0x01, 0x7F, 0x80, 0xFE, 0xFF};
    const size_t len = sizeof(bytes);

    writeFile("binary.bin", reinterpret_cast<const char*>(bytes), len);

    auto* stream = store.VOpenStream(filePath("binary.bin").c_str());
    ASSERT_NE(stream, nullptr);

    unsigned char buf[6] = {};
    stream->VRead(static_cast<unsigned long>(len), 1, buf);
    EXPECT_EQ(std::memcmp(buf, bytes, len), 0);

    delete stream;
}

TEST_F(FileSystemStoreTest, SeekFromRoot) {
    const char data[] = "ABCDEFGHIJ";
    writeFile("seek.bin", data, 10);

    auto* stream = store.VOpenStream(filePath("seek.bin").c_str());
    ASSERT_NE(stream, nullptr);

    stream->VSeek(5, IFileStore::IStream::SL_Root);
    EXPECT_EQ(stream->VTell(), 5);

    char ch = 0;
    stream->VRead(1, 1, &ch);
    EXPECT_EQ(ch, 'F');

    delete stream;
}

TEST_F(FileSystemStoreTest, SeekFromCurrent) {
    const char data[] = "ABCDEFGHIJ";
    writeFile("seekcur.bin", data, 10);

    auto* stream = store.VOpenStream(filePath("seekcur.bin").c_str());
    ASSERT_NE(stream, nullptr);

    // Read 3 bytes to advance position to 3
    char buf[3] = {};
    stream->VRead(3, 1, buf);
    EXPECT_EQ(stream->VTell(), 3);

    // Seek forward 4 from current → position 7
    stream->VSeek(4, IFileStore::IStream::SL_Current);
    EXPECT_EQ(stream->VTell(), 7);

    char ch = 0;
    stream->VRead(1, 1, &ch);
    EXPECT_EQ(ch, 'H');

    delete stream;
}

TEST_F(FileSystemStoreTest, SeekFromEnd) {
    const char data[] = "ABCDEFGHIJ";
    writeFile("seekend.bin", data, 10);

    auto* stream = store.VOpenStream(filePath("seekend.bin").c_str());
    ASSERT_NE(stream, nullptr);

    // Seek to end
    stream->VSeek(0, IFileStore::IStream::SL_End);
    EXPECT_EQ(stream->VTell(), 10);

    // Seek 3 bytes before end
    stream->VSeek(-3, IFileStore::IStream::SL_End);
    EXPECT_EQ(stream->VTell(), 7);

    char ch = 0;
    stream->VRead(1, 1, &ch);
    EXPECT_EQ(ch, 'H');

    delete stream;
}

TEST_F(FileSystemStoreTest, OpenNonexistentFileThrows) {
    std::string bad = filePath("does_not_exist.bin");
    CRainmanException* caught = nullptr;
    try {
        store.VOpenStream(bad.c_str());
    } catch (CRainmanException* ex) {
        caught = ex;
    }
    ASSERT_NE(caught, nullptr);
    EXPECT_NE(std::strstr(caught->getMessage(), "Could not open"), nullptr);
    caught->destroy();
}
