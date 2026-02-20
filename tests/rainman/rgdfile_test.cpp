#include <gtest/gtest.h>
#include "rainman/formats/CRgdFile.h"
#include "rainman/io/CMemoryStore.h"
#include "rainman/formats/CRgdHashTable.h"
#include "rainman/core/Exception.h"
#include <cstring>
#include <optional>

class RgdFileTest : public ::testing::Test {
protected:
    CRgdFile rgd;
    CRgdHashTable hashTable;

    void SetUp() override {
        rgd.SetHashTable(&hashTable);
    }
};

TEST_F(RgdFileTest, NewCreatesEmptyRootTable) {
    rgd.New(1);

    EXPECT_EQ(rgd.VGetType(), IMetaNode::DT_Table);
    auto* table = rgd.VGetValueMetatable();
    ASSERT_NE(table, nullptr);
    EXPECT_EQ(table->VGetChildCount(), 0u);
    delete table;
}

TEST_F(RgdFileTest, NewVersion3) {
    rgd.New(3);
    EXPECT_EQ(rgd.GetChunkVersion(), 1);
    EXPECT_EQ(rgd.VGetType(), IMetaNode::DT_Table);
}

TEST_F(RgdFileTest, SaveAndLoadRoundTrip) {
    rgd.New(3);

    // Add a float child
    auto* table = rgd.VGetValueMetatable();
    ASSERT_NE(table, nullptr);

    auto* child = table->VAddChild("test_float");
    ASSERT_NE(child, nullptr);
    child->VSetType(IMetaNode::DT_Float);
    child->VSetValueFloat(3.14f);
    delete child;

    // Add a string child
    auto* child2 = table->VAddChild("test_string");
    ASSERT_NE(child2, nullptr);
    child2->VSetType(IMetaNode::DT_String);
    child2->VSetValueString("hello");
    delete child2;

    // Add a bool child
    auto* child3 = table->VAddChild("test_bool");
    ASSERT_NE(child3, nullptr);
    child3->VSetType(IMetaNode::DT_Bool);
    child3->VSetValueBool(true);
    delete child3;

    delete table;

    // Save to memory
    CMemoryStore store;
    store.VInit();
    auto* outStream = CMemoryStore::OpenOutputStreamExt();
    rgd.Save(outStream);

    EXPECT_GT(outStream->GetDataLength(), 0u);

    // Load into new RGD with the same hash table
    CRgdFile rgd2;
    rgd2.SetHashTable(&hashTable);
    auto* inStream = CMemoryStore::OpenStreamExt(
        const_cast<char*>(outStream->GetData()), outStream->GetDataLength(), false);

    rgd2.Load(inStream);

    // Verify round-trip
    EXPECT_EQ(rgd2.VGetType(), IMetaNode::DT_Table);
    auto* table2 = rgd2.VGetValueMetatable();
    ASSERT_NE(table2, nullptr);
    EXPECT_EQ(table2->VGetChildCount(), 3u);

    // Find children and verify both values AND names
    bool foundFloat = false, foundString = false, foundBool = false;
    for (unsigned long i = 0; i < table2->VGetChildCount(); ++i) {
        auto* c = table2->VGetChild(i);
        ASSERT_NE(c->VGetName(), nullptr) << "Child " << i << " name should resolve via hash table";
        if (c->VGetType() == IMetaNode::DT_Float) {
            EXPECT_STREQ(c->VGetName(), "test_float");
            EXPECT_NEAR(c->VGetValueFloat(), 3.14f, 0.001f);
            foundFloat = true;
        } else if (c->VGetType() == IMetaNode::DT_String) {
            EXPECT_STREQ(c->VGetName(), "test_string");
            EXPECT_STREQ(c->VGetValueString(), "hello");
            foundString = true;
        } else if (c->VGetType() == IMetaNode::DT_Bool) {
            EXPECT_STREQ(c->VGetName(), "test_bool");
            EXPECT_TRUE(c->VGetValueBool());
            foundBool = true;
        }
        delete c;
    }
    EXPECT_TRUE(foundFloat);
    EXPECT_TRUE(foundString);
    EXPECT_TRUE(foundBool);

    delete table2;
    delete inStream;
    delete outStream;
}

TEST_F(RgdFileTest, LoadWithoutHashTableLeavesNamesNull) {
    rgd.New(3);
    auto* table = rgd.VGetValueMetatable();
    ASSERT_NE(table, nullptr);

    auto* child = table->VAddChild("some_attribute");
    child->VSetType(IMetaNode::DT_Float);
    child->VSetValueFloat(1.0f);
    delete child;
    delete table;

    // Save
    auto* outStream = CMemoryStore::OpenOutputStreamExt();
    rgd.Save(outStream);

    // Load into a new RGD with NO hash table
    CRgdFile rgd2;
    auto* inStream = CMemoryStore::OpenStreamExt(
        const_cast<char*>(outStream->GetData()), outStream->GetDataLength(), false);
    rgd2.Load(inStream);

    auto* table2 = rgd2.VGetValueMetatable();
    ASSERT_NE(table2, nullptr);
    EXPECT_EQ(table2->VGetChildCount(), 1u);

    auto* c = table2->VGetChild(0);
    // Without a hash table, names cannot be resolved from the binary hashes
    EXPECT_EQ(c->VGetName(), nullptr);
    // But the value should still load correctly
    EXPECT_NEAR(c->VGetValueFloat(), 1.0f, 0.001f);

    delete c;
    delete table2;
    delete inStream;
    delete outStream;
}

TEST_F(RgdFileTest, SaveNullStreamThrows) {
    rgd.New(1);
    std::optional<CRainmanException> caught;
    try {
        rgd.Save(nullptr);
    } catch (const CRainmanException &ex) {
        caught = ex;
    }
    ASSERT_TRUE(caught.has_value());
}

TEST_F(RgdFileTest, IntegerRoundTrip) {
    rgd.New(3);
    auto* table = rgd.VGetValueMetatable();
    ASSERT_NE(table, nullptr);

    auto* child = table->VAddChild("test_int");
    ASSERT_NE(child, nullptr);
    child->VSetType(IMetaNode::DT_Integer);
    child->VSetValueInteger(42);
    delete child;
    delete table;

    // Save
    auto* outStream = CMemoryStore::OpenOutputStreamExt();
    rgd.Save(outStream);

    // Load
    CRgdFile rgd2;
    rgd2.SetHashTable(&hashTable);
    auto* inStream = CMemoryStore::OpenStreamExt(
        const_cast<char*>(outStream->GetData()), outStream->GetDataLength(), false);
    rgd2.Load(inStream);

    auto* table2 = rgd2.VGetValueMetatable();
    ASSERT_NE(table2, nullptr);
    EXPECT_EQ(table2->VGetChildCount(), 1u);
    auto* c = table2->VGetChild(0);
    EXPECT_EQ(c->VGetType(), IMetaNode::DT_Integer);
    EXPECT_EQ(c->VGetValueInteger(), 42u);
    delete c;
    delete table2;
    delete inStream;
    delete outStream;
}

TEST_F(RgdFileTest, SortedChildrenWithNumericSuffix) {
    rgd.New(3);
    auto* root = rgd.VGetValueMetatable();
    ASSERT_NE(root, nullptr);

    // Add children with numeric suffixes in reverse order
    const char* names[] = {"item_3", "item_1", "item_10", "item_2"};
    for (auto* name : names) {
        auto* child = root->VAddChild(name);
        child->VSetType(IMetaNode::DT_Float);
        child->VSetValueFloat(1.0f);
        delete child;
    }
    delete root;

    // Save and reload to get proper hash-based storage
    auto* outStream = CMemoryStore::OpenOutputStreamExt();
    rgd.Save(outStream);

    CRgdFile rgd2;
    rgd2.SetHashTable(&hashTable);
    auto* inStream = CMemoryStore::OpenStreamExt(
        const_cast<char*>(outStream->GetData()), outStream->GetDataLength(), false);
    rgd2.Load(inStream);

    // VGetValueMetatable sorts children — verify numeric suffix order
    auto* root2 = rgd2.VGetValueMetatable();
    ASSERT_NE(root2, nullptr);
    EXPECT_EQ(root2->VGetChildCount(), 4u);

    // Should be sorted as item_1, item_2, item_3, item_10 (numeric, not lexicographic)
    auto* c0 = root2->VGetChild(0);
    auto* c1 = root2->VGetChild(1);
    auto* c2 = root2->VGetChild(2);
    auto* c3 = root2->VGetChild(3);
    EXPECT_STREQ(c0->VGetName(), "item_1");
    EXPECT_STREQ(c1->VGetName(), "item_2");
    EXPECT_STREQ(c2->VGetName(), "item_3");
    EXPECT_STREQ(c3->VGetName(), "item_10");

    // Verify names weren't corrupted by the sort
    EXPECT_STREQ(hashTable.HashToValue(c0->VGetNameHash()), "item_1");
    EXPECT_STREQ(hashTable.HashToValue(c3->VGetNameHash()), "item_10");

    delete c0;
    delete c1;
    delete c2;
    delete c3;
    delete root2;
    delete inStream;
    delete outStream;
}

TEST_F(RgdFileTest, DescriptorString) {
    rgd.New(1);
    rgd.SetDescriptorString("test descriptor");
    EXPECT_STREQ(rgd.GetDescriptorString(), "test descriptor");
}

TEST_F(RgdFileTest, NestedTableRoundTrip) {
    rgd.New(3);
    auto* root = rgd.VGetValueMetatable();
    ASSERT_NE(root, nullptr);

    // Add a sub-table child
    auto* child = root->VAddChild("sub_table");
    ASSERT_NE(child, nullptr);
    child->VSetType(IMetaNode::DT_Table);

    auto* subTable = child->VGetValueMetatable();
    ASSERT_NE(subTable, nullptr);
    auto* inner = subTable->VAddChild("inner_val");
    ASSERT_NE(inner, nullptr);
    inner->VSetType(IMetaNode::DT_Float);
    inner->VSetValueFloat(1.0f);
    delete inner;
    delete subTable;
    delete child;
    delete root;

    // Save and reload
    auto* outStream = CMemoryStore::OpenOutputStreamExt();
    rgd.Save(outStream);

    CRgdFile rgd2;
    rgd2.SetHashTable(&hashTable);
    auto* inStream = CMemoryStore::OpenStreamExt(
        const_cast<char*>(outStream->GetData()), outStream->GetDataLength(), false);
    rgd2.Load(inStream);

    auto* root2 = rgd2.VGetValueMetatable();
    ASSERT_NE(root2, nullptr);
    EXPECT_EQ(root2->VGetChildCount(), 1u);

    auto* child2 = root2->VGetChild(0);
    EXPECT_EQ(child2->VGetType(), IMetaNode::DT_Table);
    auto* subTable2 = child2->VGetValueMetatable();
    ASSERT_NE(subTable2, nullptr);
    EXPECT_EQ(subTable2->VGetChildCount(), 1u);

    auto* inner2 = subTable2->VGetChild(0);
    EXPECT_EQ(inner2->VGetType(), IMetaNode::DT_Float);
    EXPECT_NEAR(inner2->VGetValueFloat(), 1.0f, 0.001f);

    delete inner2;
    delete subTable2;
    delete child2;
    delete root2;
    delete inStream;
    delete outStream;
}
