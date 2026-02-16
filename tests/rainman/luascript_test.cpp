#include <gtest/gtest.h>
#include "rainman/lua/CLuaScript.h"
#include "rainman/core/Exception.h"
#include <cstring>
#include <filesystem>
#include <fstream>
#include <process.h>
#include <string>

class LuaScriptTest : public ::testing::Test {
protected:
	CLuaScript script;
	std::filesystem::path tempDir;

	void SetUp() override
	{
		tempDir = std::filesystem::temp_directory_path() /
		    ("luascript_test_" + std::to_string(_getpid()) + "_" +
		     std::to_string(reinterpret_cast<uintptr_t>(this)));
		std::filesystem::create_directories(tempDir);
	}

	void TearDown() override { std::filesystem::remove_all(tempDir); }

	std::string writeLuaFile(const std::string& name, const std::string& content)
	{
		auto path = (tempDir / name).string();
		std::ofstream ofs(path);
		ofs << content;
		ofs.close();
		return path;
	}
};

TEST(LuaScriptEmpty, DefaultConstructor)
{
	CLuaScript s;
	// No lua state loaded yet — GetLuaError should be null
	EXPECT_EQ(s.GetLuaError(), nullptr);
}

TEST_F(LuaScriptTest, LoadAndExecuteSimpleScript)
{
	auto path = writeLuaFile("simple.lua", "x = 1 + 2");
	EXPECT_NO_THROW({
		script.Load(path.c_str());
		script.Execute();
	});
}

TEST_F(LuaScriptTest, LoadAndExecuteMathScript)
{
	auto path = writeLuaFile("math.lua",
	    "local result = math.sqrt(144)\n"
	    "local s = string.format('answer: %d', result)\n");
	EXPECT_NO_THROW({
		script.Load(path.c_str());
		script.Execute();
	});
}

TEST_F(LuaScriptTest, LoadNonexistentFileThrows)
{
	auto path = (tempDir / "nonexistent.lua").string();
	CRainmanException* caught = nullptr;
	try
	{
		script.Load(path.c_str());
	}
	catch (CRainmanException* ex)
	{
		caught = ex;
	}
	ASSERT_NE(caught, nullptr);
	caught->destroy();
	// GetLuaError should be populated
	EXPECT_NE(script.GetLuaError(), nullptr);
}

TEST_F(LuaScriptTest, ExecuteWithoutLoadThrows)
{
	CRainmanException* caught = nullptr;
	try
	{
		script.Execute();
	}
	catch (CRainmanException* ex)
	{
		caught = ex;
	}
	ASSERT_NE(caught, nullptr);
	caught->destroy();
}

TEST_F(LuaScriptTest, ExecuteWithSyntaxErrorThrows)
{
	auto path = writeLuaFile("bad.lua", "this is not valid lua !!!");
	CRainmanException* caught = nullptr;
	try
	{
		script.Load(path.c_str());
	}
	catch (CRainmanException* ex)
	{
		caught = ex;
	}
	ASSERT_NE(caught, nullptr);
	EXPECT_NE(script.GetLuaError(), nullptr);
	caught->destroy();
}

TEST_F(LuaScriptTest, ExecuteWithRuntimeErrorThrows)
{
	// Valid syntax but will error at runtime (calling nil as function)
	auto path = writeLuaFile("runtime_err.lua", "local f = nil\nf()");
	CRainmanException* caught = nullptr;
	try
	{
		script.Load(path.c_str());
		script.Execute();
	}
	catch (CRainmanException* ex)
	{
		caught = ex;
	}
	ASSERT_NE(caught, nullptr);
	EXPECT_NE(script.GetLuaError(), nullptr);
	caught->destroy();
}

TEST_F(LuaScriptTest, LoadReplacesState)
{
	auto path1 = writeLuaFile("first.lua", "x = 10");
	auto path2 = writeLuaFile("second.lua", "y = 20");

	script.Load(path1.c_str());
	script.Execute();

	// Loading a second file should replace the state
	EXPECT_NO_THROW({
		script.Load(path2.c_str());
		script.Execute();
	});
}
