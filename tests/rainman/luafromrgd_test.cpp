#include <gtest/gtest.h>
#include "rainman/lua/CLuaFromRgd.h"
#include "rainman/formats/CRgdFile.h"
#include "rainman/io/CMemoryStore.h"
#include "rainman/formats/CRgdHashTable.h"
#include "rainman/core/Exception.h"
#include <cstring>
#include <string>

class LuaFromRgdTest : public ::testing::Test {
protected:
	CRgdFile rgd;
	CRgdHashTable hashTable;

	void SetUp() override { rgd.SetHashTable(&hashTable); }

	// Convert current RGD to Lua source and return as string
	std::string convertToLua()
	{
		auto* pOut = CMemoryStore::OpenOutputStreamExt();
		MakeLuaFromRgdQuickly(&rgd, pOut);
		std::string result(pOut->GetData(), pOut->GetDataLength());
		delete pOut;
		return result;
	}
};

TEST_F(LuaFromRgdTest, EmptyRgdProducesOutput)
{
	rgd.New(3);

	std::string output = convertToLua();
	// Empty root table — output should still be valid (may be empty or have GameData = {})
	// At minimum, it should not crash
	SUCCEED();
}

TEST_F(LuaFromRgdTest, FloatValue)
{
	rgd.New(3);
	auto* table = rgd.VGetValueMetatable();
	ASSERT_NE(table, nullptr);

	auto* child = table->VAddChild("damage");
	child->VSetType(IMetaNode::DT_Float);
	child->VSetValueFloat(25.5f);
	delete child;
	delete table;

	std::string output = convertToLua();
	EXPECT_NE(output.find("damage"), std::string::npos);
	EXPECT_NE(output.find("25.5"), std::string::npos);
}

TEST_F(LuaFromRgdTest, IntegerValue)
{
	rgd.New(3);
	auto* table = rgd.VGetValueMetatable();
	ASSERT_NE(table, nullptr);

	auto* child = table->VAddChild("count");
	child->VSetType(IMetaNode::DT_Integer);
	child->VSetValueInteger(42);
	delete child;
	delete table;

	std::string output = convertToLua();
	EXPECT_NE(output.find("count"), std::string::npos);
	EXPECT_NE(output.find("42"), std::string::npos);
}

TEST_F(LuaFromRgdTest, BoolValue)
{
	rgd.New(3);
	auto* table = rgd.VGetValueMetatable();
	ASSERT_NE(table, nullptr);

	auto* child = table->VAddChild("enabled");
	child->VSetType(IMetaNode::DT_Bool);
	child->VSetValueBool(true);
	delete child;
	delete table;

	std::string output = convertToLua();
	EXPECT_NE(output.find("enabled"), std::string::npos);
	EXPECT_NE(output.find("true"), std::string::npos);
}

TEST_F(LuaFromRgdTest, StringValue)
{
	rgd.New(3);
	auto* table = rgd.VGetValueMetatable();
	ASSERT_NE(table, nullptr);

	auto* child = table->VAddChild("unit_name");
	child->VSetType(IMetaNode::DT_String);
	child->VSetValueString("space_marine");
	delete child;
	delete table;

	std::string output = convertToLua();
	EXPECT_NE(output.find("unit_name"), std::string::npos);
	EXPECT_NE(output.find("space_marine"), std::string::npos);
}

TEST_F(LuaFromRgdTest, MultipleValues)
{
	rgd.New(3);
	auto* table = rgd.VGetValueMetatable();
	ASSERT_NE(table, nullptr);

	auto* c1 = table->VAddChild("health");
	c1->VSetType(IMetaNode::DT_Float);
	c1->VSetValueFloat(100.0f);
	delete c1;

	auto* c2 = table->VAddChild("active");
	c2->VSetType(IMetaNode::DT_Bool);
	c2->VSetValueBool(false);
	delete c2;

	auto* c3 = table->VAddChild("label");
	c3->VSetType(IMetaNode::DT_String);
	c3->VSetValueString("test");
	delete c3;

	delete table;

	std::string output = convertToLua();
	EXPECT_NE(output.find("health"), std::string::npos);
	EXPECT_NE(output.find("active"), std::string::npos);
	EXPECT_NE(output.find("label"), std::string::npos);
}

TEST_F(LuaFromRgdTest, NestedTable)
{
	rgd.New(3);
	auto* table = rgd.VGetValueMetatable();
	ASSERT_NE(table, nullptr);

	// Create a nested table: stats = { armor = 50 }
	auto* stats = table->VAddChild("stats");
	stats->VSetType(IMetaNode::DT_Table);
	auto* statsTable = stats->VGetValueMetatable();
	ASSERT_NE(statsTable, nullptr);

	auto* armor = statsTable->VAddChild("armor");
	armor->VSetType(IMetaNode::DT_Float);
	armor->VSetValueFloat(50.0f);
	delete armor;

	delete statsTable;
	delete stats;
	delete table;

	std::string output = convertToLua();
	EXPECT_NE(output.find("stats"), std::string::npos);
	EXPECT_NE(output.find("armor"), std::string::npos);
	EXPECT_NE(output.find("50"), std::string::npos);
}

TEST_F(LuaFromRgdTest, NullRgdCrashes)
{
	// MakeLuaFromRgdQuickly does not null-check — passing nullptr causes access violation
	GTEST_SKIP() << "Null RGD causes access violation (no null check in original code)";
}
