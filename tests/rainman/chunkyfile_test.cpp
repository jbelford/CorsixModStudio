#include <gtest/gtest.h>
#include "rainman/formats/CChunkyFile.h"
#include "rainman/io/CMemoryStore.h"
#include "rainman/core/Exception.h"
#include <cstring>
#include <optional>

class ChunkyFileTest : public ::testing::Test {
protected:
    CChunkyFile chunky;
};

TEST(ChunkyFileEmpty, DefaultConstructor) {
    CChunkyFile cf;
    EXPECT_EQ(cf.GetChildCount(), 0u);
}

TEST_F(ChunkyFileTest, NewCreatesValidChunky) {
    chunky.New(3);
    EXPECT_EQ(chunky.GetChildCount(), 0u);
}

TEST_F(ChunkyFileTest, AppendDataChunk) {
    chunky.New(3);
    auto* chunk = chunky.AppendNew("TEST", CChunkyFile::CChunk::T_Data);
    ASSERT_NE(chunk, nullptr);
    EXPECT_STREQ(chunk->GetName(), "TEST");
    EXPECT_EQ(chunk->GetType(), CChunkyFile::CChunk::T_Data);
    EXPECT_EQ(chunky.GetChildCount(), 1u);
}

TEST_F(ChunkyFileTest, AppendFolderChunk) {
    chunky.New(3);
    auto* folder = chunky.AppendNew("FOLD", CChunkyFile::CChunk::T_Folder);
    ASSERT_NE(folder, nullptr);
    EXPECT_EQ(folder->GetType(), CChunkyFile::CChunk::T_Folder);
    EXPECT_EQ(folder->GetChildCount(), 0u);
}

TEST_F(ChunkyFileTest, NestedChunks) {
    chunky.New(3);
    auto* folder = chunky.AppendNew("PRNT", CChunkyFile::CChunk::T_Folder);
    auto* child = folder->AppendNew("CHLD", CChunkyFile::CChunk::T_Data);
    ASSERT_NE(child, nullptr);
    EXPECT_EQ(folder->GetChildCount(), 1u);
    EXPECT_STREQ(folder->GetChild(0)->GetName(), "CHLD");
}

TEST_F(ChunkyFileTest, SetDataOnChunk) {
    chunky.New(3);
    auto* chunk = chunky.AppendNew("DATA", CChunkyFile::CChunk::T_Data);

    auto* dataOut = CMemoryStore::OpenOutputStreamExt();
    const char payload[] = "TestPayload";
    dataOut->VWrite(sizeof(payload) - 1, 1, payload);
    chunk->SetData(dataOut);
    delete dataOut;

    EXPECT_EQ(chunk->GetDataLength(), sizeof(payload) - 1);
    EXPECT_EQ(std::memcmp(chunk->GetDataRaw(), payload, sizeof(payload) - 1), 0);
}

TEST_F(ChunkyFileTest, ChunkVersionAndDescriptor) {
    chunky.New(3);
    auto* chunk = chunky.AppendNew("VERS", CChunkyFile::CChunk::T_Data);
    chunk->SetVersion(42);
    chunk->SetDescriptor("my descriptor");

    EXPECT_EQ(chunk->GetVersion(), 42);
    EXPECT_STREQ(chunk->GetDescriptor(), "my descriptor");
}

TEST_F(ChunkyFileTest, SaveAndLoadRoundTrip) {
    chunky.New(3);
    auto* data1 = chunky.AppendNew("DAT1", CChunkyFile::CChunk::T_Data);
    data1->SetVersion(5);
    data1->SetDescriptor("first chunk");

    auto* dataOut = CMemoryStore::OpenOutputStreamExt();
    const char payload[] = "Hello Chunky";
    dataOut->VWrite(sizeof(payload) - 1, 1, payload);
    data1->SetData(dataOut);
    delete dataOut;

    auto* folder = chunky.AppendNew("PRNT", CChunkyFile::CChunk::T_Folder);
    auto* child = folder->AppendNew("CHLD", CChunkyFile::CChunk::T_Data);
    child->SetVersion(2);

    auto* childData = CMemoryStore::OpenOutputStreamExt();
    const char childPayload[] = "Inner";
    childData->VWrite(sizeof(childPayload) - 1, 1, childPayload);
    child->SetData(childData);
    delete childData;

    // Save
    auto* saveStream = CMemoryStore::OpenOutputStreamExt();
    chunky.Save(saveStream);
    EXPECT_GT(saveStream->GetDataLength(), 0u);

    // Load into new CChunkyFile
    auto* loadStream = CMemoryStore::OpenStreamExt(
        const_cast<char*>(saveStream->GetData()), saveStream->GetDataLength(), false);

    CChunkyFile chunky2;
    chunky2.Load(loadStream);

    // Verify structure
    EXPECT_EQ(chunky2.GetChildCount(), 2u);

    auto* loaded1 = chunky2.GetChild(0);
    EXPECT_EQ(loaded1->GetType(), CChunkyFile::CChunk::T_Data);
    EXPECT_STREQ(loaded1->GetName(), "DAT1");
    EXPECT_EQ(loaded1->GetVersion(), 5);
    EXPECT_STREQ(loaded1->GetDescriptor(), "first chunk");
    EXPECT_EQ(loaded1->GetDataLength(), sizeof(payload) - 1);
    EXPECT_EQ(std::memcmp(loaded1->GetDataRaw(), payload, sizeof(payload) - 1), 0);

    auto* loadedFolder = chunky2.GetChild(1);
    EXPECT_EQ(loadedFolder->GetType(), CChunkyFile::CChunk::T_Folder);
    EXPECT_STREQ(loadedFolder->GetName(), "PRNT");
    EXPECT_EQ(loadedFolder->GetChildCount(), 1u);

    auto* loadedChild = loadedFolder->GetChild(0);
    EXPECT_STREQ(loadedChild->GetName(), "CHLD");
    EXPECT_EQ(loadedChild->GetVersion(), 2);
    EXPECT_EQ(loadedChild->GetDataLength(), sizeof(childPayload) - 1);
    EXPECT_EQ(std::memcmp(loadedChild->GetDataRaw(), childPayload, sizeof(childPayload) - 1), 0);

    delete loadStream;
    delete saveStream;
}

TEST_F(ChunkyFileTest, EmptyDataChunkRoundTrip) {
    chunky.New(3);
    auto* chunk = chunky.AppendNew("EMPT", CChunkyFile::CChunk::T_Data);
    chunk->SetVersion(1);
    // Data length is 0 by default (from AppendNew)

    auto* saveStream = CMemoryStore::OpenOutputStreamExt();
    chunky.Save(saveStream);

    auto* loadStream = CMemoryStore::OpenStreamExt(
        const_cast<char*>(saveStream->GetData()), saveStream->GetDataLength(), false);

    CChunkyFile chunky2;
    chunky2.Load(loadStream);

    EXPECT_EQ(chunky2.GetChildCount(), 1u);
    // The chunk should load with its 1-byte default data from AppendNew
    EXPECT_EQ(chunky2.GetChild(0)->GetVersion(), 1);

    delete loadStream;
    delete saveStream;
}

TEST_F(ChunkyFileTest, LoadInvalidHeaderThrows) {
    CMemoryStore store;
    store.VInit();

    const char garbage[] = "Not a chunky file header data";
    auto* stream = CMemoryStore::OpenStreamExt(
        const_cast<char*>(garbage), sizeof(garbage) - 1, false);

    CChunkyFile cf;
    std::optional<CRainmanException> caught;
    try {
        cf.Load(stream);
    } catch (const CRainmanException &ex) {
        caught = ex;
    }
    ASSERT_TRUE(caught.has_value());
    delete stream;
}

TEST_F(ChunkyFileTest, GetChildByName) {
    chunky.New(3);
    chunky.AppendNew("AAA1", CChunkyFile::CChunk::T_Data);
    chunky.AppendNew("BBB2", CChunkyFile::CChunk::T_Folder);

    auto* found = chunky.GetChildByName("BBB2", CChunkyFile::CChunk::T_Folder);
    ASSERT_NE(found, nullptr);
    EXPECT_STREQ(found->GetName(), "BBB2");

    auto* notFound = chunky.GetChildByName("XXXX", CChunkyFile::CChunk::T_Data);
    EXPECT_EQ(notFound, nullptr);
}

TEST_F(ChunkyFileTest, RemoveChild) {
    chunky.New(3);
    chunky.AppendNew("AAA1", CChunkyFile::CChunk::T_Data);
    chunky.AppendNew("BBB2", CChunkyFile::CChunk::T_Data);
    EXPECT_EQ(chunky.GetChildCount(), 2u);

    chunky.RemoveChild(0);
    EXPECT_EQ(chunky.GetChildCount(), 1u);
    EXPECT_STREQ(chunky.GetChild(0)->GetName(), "BBB2");
}

TEST_F(ChunkyFileTest, DeepNestingRoundTrip) {
    chunky.New(3);
    auto* level1 = chunky.AppendNew("LVL1", CChunkyFile::CChunk::T_Folder);
    auto* level2 = level1->AppendNew("LVL2", CChunkyFile::CChunk::T_Folder);
    auto* level3 = level2->AppendNew("LVL3", CChunkyFile::CChunk::T_Folder);
    auto* leaf = level3->AppendNew("LEAF", CChunkyFile::CChunk::T_Data);

    auto* dataOut = CMemoryStore::OpenOutputStreamExt();
    const char payload[] = "deep";
    dataOut->VWrite(sizeof(payload) - 1, 1, payload);
    leaf->SetData(dataOut);
    delete dataOut;

    auto* saveStream = CMemoryStore::OpenOutputStreamExt();
    chunky.Save(saveStream);

    auto* loadStream = CMemoryStore::OpenStreamExt(
        const_cast<char*>(saveStream->GetData()), saveStream->GetDataLength(), false);
    CChunkyFile chunky2;
    chunky2.Load(loadStream);

    ASSERT_EQ(chunky2.GetChildCount(), 1u);
    auto* l1 = chunky2.GetChild(0);
    ASSERT_EQ(l1->GetChildCount(), 1u);
    auto* l2 = l1->GetChild(0);
    ASSERT_EQ(l2->GetChildCount(), 1u);
    auto* l3 = l2->GetChild(0);
    ASSERT_EQ(l3->GetChildCount(), 1u);
    auto* loaded = l3->GetChild(0);
    EXPECT_STREQ(loaded->GetName(), "LEAF");
    EXPECT_EQ(loaded->GetDataLength(), sizeof(payload) - 1);
    EXPECT_EQ(std::memcmp(loaded->GetDataRaw(), payload, sizeof(payload) - 1), 0);

    delete loadStream;
    delete saveStream;
}

TEST_F(ChunkyFileTest, EmptyDescriptorRoundTrip) {
    chunky.New(3);
    auto* chunk = chunky.AppendNew("TEST", CChunkyFile::CChunk::T_Data);
    chunk->SetDescriptor("");

    auto* dataOut = CMemoryStore::OpenOutputStreamExt();
    dataOut->VWrite(3, 1, "abc");
    chunk->SetData(dataOut);
    delete dataOut;

    auto* saveStream = CMemoryStore::OpenOutputStreamExt();
    chunky.Save(saveStream);

    auto* loadStream = CMemoryStore::OpenStreamExt(
        const_cast<char*>(saveStream->GetData()), saveStream->GetDataLength(), false);
    CChunkyFile chunky2;
    chunky2.Load(loadStream);

    ASSERT_EQ(chunky2.GetChildCount(), 1u);
    EXPECT_STREQ(chunky2.GetChild(0)->GetDescriptor(), "");
    EXPECT_EQ(chunky2.GetChild(0)->GetDataLength(), 3u);

    delete loadStream;
    delete saveStream;
}

TEST_F(ChunkyFileTest, InsertBeforeOrderPreserved) {
    chunky.New(3);
    chunky.AppendNew("BBBB", CChunkyFile::CChunk::T_Data);
    auto* folder = chunky.AppendNew("FOLD", CChunkyFile::CChunk::T_Folder);
    folder->AppendNew("CC", CChunkyFile::CChunk::T_Data);
    folder->InsertBefore(0, "AA", CChunkyFile::CChunk::T_Data);

    ASSERT_EQ(folder->GetChildCount(), 2u);
    EXPECT_STREQ(folder->GetChild(0)->GetName(), "AA\0\0");
    EXPECT_STREQ(folder->GetChild(1)->GetName(), "CC\0\0");
}

TEST_F(ChunkyFileTest, GetDataLengthIsConst) {
    chunky.New(3);
    auto* chunk = chunky.AppendNew("DATA", CChunkyFile::CChunk::T_Data);
    const auto* constChunk = chunk;
    EXPECT_EQ(constChunk->GetDataLength(), 0u);
}
