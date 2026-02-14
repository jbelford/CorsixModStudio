#include <gtest/gtest.h>
#include "CMemoryStore.h"
#include "Exception.h"
#include <cstring>

class MemoryStoreTest : public ::testing::Test {
protected:
    CMemoryStore store;

    void SetUp() override {
        store.VInit();
    }
};

TEST_F(MemoryStoreTest, ReadFromMemoryRange) {
    const char data[] = "Hello, Rainman!";
    char* range = store.MemoryRange((void*)data, sizeof(data) - 1);

    auto* stream = store.VOpenStream(range);
    ASSERT_NE(stream, nullptr);

    char buf[32] = {};
    stream->VRead(sizeof(data) - 1, 1, buf);
    EXPECT_STREQ(buf, "Hello, Rainman!");

    delete stream;
}

TEST_F(MemoryStoreTest, StreamTellAndSeek) {
    const char data[] = "ABCDEFGHIJ";
    char* range = store.MemoryRange((void*)data, 10);

    auto* stream = store.VOpenStream(range);
    ASSERT_NE(stream, nullptr);

    EXPECT_EQ(stream->VTell(), 0);

    char buf[4] = {};
    stream->VRead(3, 1, buf);
    EXPECT_EQ(stream->VTell(), 3);
    EXPECT_EQ(buf[0], 'A');
    EXPECT_EQ(buf[1], 'B');
    EXPECT_EQ(buf[2], 'C');

    // Seek from current
    stream->VSeek(2, IFileStore::IStream::SL_Current);
    EXPECT_EQ(stream->VTell(), 5);

    // Read after seek
    stream->VRead(1, 1, buf);
    EXPECT_EQ(buf[0], 'F');

    // Seek from beginning
    stream->VSeek(0, IFileStore::IStream::SL_Root);
    EXPECT_EQ(stream->VTell(), 0);

    delete stream;
}

TEST_F(MemoryStoreTest, OutputStreamWriteAndRead) {
    auto* ostream = store.VOpenOutputStream("test", false);
    ASSERT_NE(ostream, nullptr);

    const char data[] = "Test output data";
    ostream->VWrite(sizeof(data) - 1, 1, data);

    auto* outStream = dynamic_cast<CMemoryStore::COutStream*>(ostream);
    ASSERT_NE(outStream, nullptr);
    EXPECT_EQ(outStream->GetDataLength(), sizeof(data) - 1);

    const char* result = outStream->GetData();
    EXPECT_EQ(memcmp(result, data, sizeof(data) - 1), 0);

    delete ostream;
}

TEST_F(MemoryStoreTest, OutputStreamMultipleWrites) {
    auto* outStream = CMemoryStore::OpenOutputStreamExt();
    ASSERT_NE(outStream, nullptr);

    outStream->VWrite(5, 1, "Hello");
    outStream->VWrite(1, 1, " ");
    outStream->VWrite(5, 1, "World");

    EXPECT_EQ(outStream->GetDataLength(), 11u);
    EXPECT_EQ(memcmp(outStream->GetData(), "Hello World", 11), 0);

    delete outStream;
}

TEST_F(MemoryStoreTest, OpenStreamExtStatic) {
    char* data = new char[5];
    memcpy(data, "ABCDE", 5);

    auto* stream = CMemoryStore::OpenStreamExt(data, 5, true);
    ASSERT_NE(stream, nullptr);

    char buf[5] = {};
    stream->VRead(5, 1, buf);
    EXPECT_EQ(memcmp(buf, "ABCDE", 5), 0);

    delete stream; // should also delete data since bDeleteWhenDone=true
}
