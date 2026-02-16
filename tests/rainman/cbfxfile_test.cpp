#include <gtest/gtest.h>
#include "rainman/formats/CBfxFile.h"
#include "rainman/io/CMemoryStore.h"
#include "rainman/formats/CRgdHashTable.h"
#include "rainman/io/StreamGuard.h"
#include "rainman/core/Exception.h"
#include <cstring>
#include <string>

class BfxFileTest : public ::testing::Test {
protected:
	CBfxFile bfx;
	CRgdHashTable hashTable;

	void SetUp() override
	{
		bfx.SetHashTable(&hashTable);
	}

	// Helper: get the Lua output as a string
	std::string SaveToString()
	{
		auto *pOut = CMemoryStore::OpenOutputStreamExt();
		bfx.SaveAsBfxLua(pOut, nullptr);
		std::string result(pOut->GetData(), pOut->GetDataLength());
		delete pOut;
		return result;
	}
};

TEST(BfxFileEmpty, DefaultConstructor)
{
	CBfxFile bfx;
	// CBfxFile inherits CRgdFile; should construct without crash
	EXPECT_EQ(bfx.VGetType(), IMetaNode::DT_NoData);
}

TEST_F(BfxFileTest, SaveEmptyBfx)
{
	bfx.New(3);

	std::string output = SaveToString();
	// Empty root table — no children means no output
	EXPECT_TRUE(output.empty());
}

TEST_F(BfxFileTest, SaveFloatValue)
{
	bfx.New(3);
	auto *table = bfx.VGetValueMetatable();
	ASSERT_NE(table, nullptr);

	auto *child = table->VAddChild("damage");
	child->VSetType(IMetaNode::DT_Float);
	child->VSetValueFloat(25.0f);
	delete child;
	delete table;

	std::string output = SaveToString();
	EXPECT_NE(output.find("damage = "), std::string::npos);
	EXPECT_NE(output.find("25"), std::string::npos);
}

TEST_F(BfxFileTest, SaveIntegerValue)
{
	bfx.New(3);
	auto *table = bfx.VGetValueMetatable();
	ASSERT_NE(table, nullptr);

	auto *child = table->VAddChild("count");
	child->VSetType(IMetaNode::DT_Integer);
	child->VSetValueInteger(42);
	delete child;
	delete table;

	std::string output = SaveToString();
	EXPECT_NE(output.find("count = 42,"), std::string::npos);
}

TEST_F(BfxFileTest, SaveBoolValues)
{
	bfx.New(3);
	auto *table = bfx.VGetValueMetatable();
	ASSERT_NE(table, nullptr);

	auto *child1 = table->VAddChild("enabled");
	child1->VSetType(IMetaNode::DT_Bool);
	child1->VSetValueBool(true);
	delete child1;

	auto *child2 = table->VAddChild("hidden");
	child2->VSetType(IMetaNode::DT_Bool);
	child2->VSetValueBool(false);
	delete child2;
	delete table;

	std::string output = SaveToString();
	EXPECT_NE(output.find("enabled = true,"), std::string::npos);
	EXPECT_NE(output.find("hidden = false,"), std::string::npos);
}

TEST_F(BfxFileTest, SaveStringValue)
{
	bfx.New(3);
	auto *table = bfx.VGetValueMetatable();
	ASSERT_NE(table, nullptr);

	auto *child = table->VAddChild("texture");
	child->VSetType(IMetaNode::DT_String);
	child->VSetValueString("soldier_skin");
	delete child;
	delete table;

	std::string output = SaveToString();
	EXPECT_NE(output.find("texture = \"soldier_skin\","), std::string::npos);
}

TEST_F(BfxFileTest, SaveStringWithSpecialChars)
{
	bfx.New(3);
	auto *table = bfx.VGetValueMetatable();
	ASSERT_NE(table, nullptr);

	auto *child = table->VAddChild("path");
	child->VSetType(IMetaNode::DT_String);
	child->VSetValueString("data\\textures\\soldier");
	delete child;
	delete table;

	std::string output = SaveToString();
	// Backslash should be escaped in the output
	EXPECT_NE(output.find("path = "), std::string::npos);
	EXPECT_NE(output.find("\\\\"), std::string::npos);
}

TEST_F(BfxFileTest, SaveNestedTable)
{
	bfx.New(3);
	auto *table = bfx.VGetValueMetatable();
	ASSERT_NE(table, nullptr);

	auto *child = table->VAddChild("properties");
	child->VSetType(IMetaNode::DT_Table);
	auto *subTable = child->VGetValueMetatable();
	ASSERT_NE(subTable, nullptr);

	auto *inner = subTable->VAddChild("health");
	inner->VSetType(IMetaNode::DT_Float);
	inner->VSetValueFloat(100.0f);
	delete inner;
	delete subTable;
	delete child;
	delete table;

	std::string output = SaveToString();
	EXPECT_NE(output.find("properties = "), std::string::npos);
	EXPECT_NE(output.find("{"), std::string::npos);
	EXPECT_NE(output.find("health = "), std::string::npos);
	EXPECT_NE(output.find("}"), std::string::npos);
}

TEST_F(BfxFileTest, SaveMultipleTopLevelEntries)
{
	bfx.New(3);
	auto *table = bfx.VGetValueMetatable();
	ASSERT_NE(table, nullptr);

	auto *c1 = table->VAddChild("alpha");
	c1->VSetType(IMetaNode::DT_Integer);
	c1->VSetValueInteger(1);
	delete c1;

	auto *c2 = table->VAddChild("beta");
	c2->VSetType(IMetaNode::DT_Integer);
	c2->VSetValueInteger(2);
	delete c2;

	auto *c3 = table->VAddChild("gamma");
	c3->VSetType(IMetaNode::DT_Integer);
	c3->VSetValueInteger(3);
	delete c3;
	delete table;

	std::string output = SaveToString();
	EXPECT_NE(output.find("alpha = 1,"), std::string::npos);
	EXPECT_NE(output.find("beta = 2,"), std::string::npos);
	EXPECT_NE(output.find("gamma = 3,"), std::string::npos);
}
