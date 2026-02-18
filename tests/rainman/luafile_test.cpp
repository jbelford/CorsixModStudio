#include <gtest/gtest.h>
#include "rainman/lua/CLuaFile.h"
#include "rainman/io/CMemoryStore.h"
#include <memory>
#include "rainman/core/Exception.h"
#include <cstring>
#include <string>
#include <optional>

class LuaFileTest : public ::testing::Test {
protected:
	CLuaFile lua;
	CMemoryStore store;

	void SetUp() override { store.VInit(); }

	void loadScript(const char* sScript, const char* sFileName = "test.lua")
	{
		std::unique_ptr<IFileStore::IStream> inStream(CMemoryStore::OpenStreamExt(
		    const_cast<char*>(sScript), static_cast<unsigned long>(strlen(sScript)), false));
		lua.Load(inStream.get(), &store, sFileName);
	}

	// Find a child by name; caller must delete the returned node
	IMetaNode* findChild(IMetaNode::IMetaTable* table, const char* name)
	{
		for (unsigned long i = 0; i < table->VGetChildCount(); ++i)
		{
			auto* child = table->VGetChild(i);
			if (child->VGetName() && strcmp(child->VGetName(), name) == 0)
				return child;
			delete child;
		}
		return nullptr;
	}
};

TEST(LuaFileEmpty, DefaultConstructor)
{
	CLuaFile lua;
	EXPECT_EQ(lua.VGetChildCount(), 0u);
}

TEST_F(LuaFileTest, NewCreatesEmptyState)
{
	lua.New();
	// New() doesn't populate globals list — only Load() does via _TableToNodeList
	EXPECT_EQ(lua.VGetChildCount(), 0u);
}

TEST_F(LuaFileTest, LoadSimpleTable)
{
	loadScript("GameData = { health = 100.0, name = \"soldier\" }");

	// Should have at least one global
	EXPECT_GT(lua.VGetChildCount(), 0u);

	// Find GameData
	auto* gameData = findChild(&lua, "GameData");
	ASSERT_NE(gameData, nullptr);
	EXPECT_EQ(gameData->VGetType(), IMetaNode::DT_Table);

	auto* table = gameData->VGetValueMetatable();
	ASSERT_NE(table, nullptr);
	EXPECT_GT(table->VGetChildCount(), 0u);

	// Find health inside GameData
	auto* health = findChild(table, "health");
	ASSERT_NE(health, nullptr);
	EXPECT_FLOAT_EQ(health->VGetValueFloat(), 100.0f);
	delete health;

	// Find name inside GameData
	auto* name = findChild(table, "name");
	ASSERT_NE(name, nullptr);
	EXPECT_STREQ(name->VGetValueString(), "soldier");
	delete name;

	delete table;
	delete gameData;
}

TEST_F(LuaFileTest, LoadNestedTable)
{
	const char* script =
	    "GameData = {\n"
	    "  stats = {\n"
	    "    armor = 50.0,\n"
	    "  },\n"
	    "}";
	loadScript(script);

	auto* gameData = findChild(&lua, "GameData");
	ASSERT_NE(gameData, nullptr);

	auto* gdTable = gameData->VGetValueMetatable();
	ASSERT_NE(gdTable, nullptr);

	auto* stats = findChild(gdTable, "stats");
	ASSERT_NE(stats, nullptr);
	EXPECT_EQ(stats->VGetType(), IMetaNode::DT_Table);

	auto* statsTable = stats->VGetValueMetatable();
	ASSERT_NE(statsTable, nullptr);

	auto* armor = findChild(statsTable, "armor");
	ASSERT_NE(armor, nullptr);
	EXPECT_FLOAT_EQ(armor->VGetValueFloat(), 50.0f);
	delete armor;

	delete statsTable;
	delete stats;
	delete gdTable;
	delete gameData;
}

TEST_F(LuaFileTest, LoadBoolAndStringValues)
{
	loadScript("GameData = { enabled = true, label = \"hello\" }");

	auto* gameData = findChild(&lua, "GameData");
	ASSERT_NE(gameData, nullptr);
	auto* table = gameData->VGetValueMetatable();
	ASSERT_NE(table, nullptr);

	auto* enabled = findChild(table, "enabled");
	ASSERT_NE(enabled, nullptr);
	EXPECT_EQ(enabled->VGetValueBool(), true);
	delete enabled;

	auto* label = findChild(table, "label");
	ASSERT_NE(label, nullptr);
	EXPECT_STREQ(label->VGetValueString(), "hello");
	delete label;

	delete table;
	delete gameData;
}

TEST_F(LuaFileTest, LoadMultipleGlobals)
{
	loadScript("GameData = { x = 1 }\nMetaData = { y = 2 }");

	auto* gameData = findChild(&lua, "GameData");
	ASSERT_NE(gameData, nullptr);
	delete gameData;

	auto* metaData = findChild(&lua, "MetaData");
	ASSERT_NE(metaData, nullptr);
	delete metaData;
}

TEST_F(LuaFileTest, LoadNullStreamThrows)
{
	std::optional<CRainmanException> caught;
	try
	{
		lua.Load(nullptr, &store, "test.lua");
	}
	catch (const CRainmanException &ex)
	{
		caught = ex;
	}
	ASSERT_TRUE(caught.has_value());
}

TEST_F(LuaFileTest, LoadInvalidSyntaxThrows)
{
	std::optional<CRainmanException> caught;
	try
	{
		loadScript("GameData = { this is not valid !!!");
	}
	catch (const CRainmanException &ex)
	{
		caught = ex;
	}
	ASSERT_TRUE(caught.has_value());
}

TEST_F(LuaFileTest, VAddChildThrows)
{
	lua.New();
	std::optional<CRainmanException> caught;
	try
	{
		lua.VAddChild("test");
	}
	catch (const CRainmanException &ex)
	{
		caught = ex;
	}
	// VAddChild is not implemented — it throws
	ASSERT_TRUE(caught.has_value());
}

TEST_F(LuaFileTest, VDeleteChildThrows)
{
	lua.New();
	std::optional<CRainmanException> caught;
	try
	{
		lua.VDeleteChild(0);
	}
	catch (const CRainmanException &ex)
	{
		caught = ex;
	}
	ASSERT_TRUE(caught.has_value());
}

TEST_F(LuaFileTest, GetChildOutOfRangeThrows)
{
	loadScript("GameData = {}");
	unsigned long count = lua.VGetChildCount();

	std::optional<CRainmanException> caught;
	try
	{
		lua.VGetChild(count);
	}
	catch (const CRainmanException &ex)
	{
		caught = ex;
	}
	ASSERT_TRUE(caught.has_value());
}

TEST_F(LuaFileTest, ReferenceTypeIsNoData)
{
	loadScript("GameData = {}");
	EXPECT_EQ(lua.VGetReferenceType(), IMetaNode::DT_NoData);
}
