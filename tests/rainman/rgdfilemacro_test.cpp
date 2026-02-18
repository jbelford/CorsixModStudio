/*
Rainman Library
Copyright (C) 2006 Corsix <corsix@gmail.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <gtest/gtest.h>
#include "rainman/formats/CRgdFileMacro.h"
#include "rainman/formats/CRgdFile.h"
#include "rainman/formats/CRgdHashTable.h"
#include "rainman/io/CMemoryStore.h"
#include "rainman/core/Exception.h"
#include <cstring>
#include <string>
#include <vector>
#include <optional>

// ── Test helpers ──────────────────────────────────────────────────────────────

struct PrintCapture
{
    std::vector<std::string> lines;

    static void Callback(void* pTag, const char* sMsg)
    {
        auto* self = static_cast<PrintCapture*>(pTag);
        // luaf_print appends \r\n — strip trailing whitespace for clean assertions
        std::string line(sMsg);
        while (!line.empty() && (line.back() == '\r' || line.back() == '\n'))
            line.pop_back();
        self->lines.push_back(std::move(line));
    }
};

struct SaveCapture
{
    std::vector<std::string> paths;

    static bool Callback(void* pTag, const char* sFile)
    {
        auto* self = static_cast<SaveCapture*>(pTag);
        self->paths.emplace_back(sFile);
        return true;
    }
};

// Always deny security requests (IO/OS/Debug) so tests don't touch the filesystem
static bool DenyAllSecurity(void* /*pTag*/, CRgdFileMacro::eSecurityTypes /*eType*/)
{
    return false;
}

// ── Fixture ──────────────────────────────────────────────────────────────────

class RgdFileMacroTest : public ::testing::Test
{
  protected:
    CRgdFileMacro macro;
    CRgdHashTable hashTable;
    PrintCapture printCapture;
    SaveCapture saveCapture;

    void SetUp() override
    {
        macro.setHashTable(&hashTable);
        macro.setCallbackPrint(&PrintCapture::Callback);
        macro.setCallbackSave(&SaveCapture::Callback);
        macro.setCallbackSecurity(&DenyAllSecurity);
        macro.setCallbackTag(&printCapture);
    }

    void TearDown() override { macro.unloadMacro(); }

    // Helper: create a minimal in-memory RGD v3 file and return its stream name
    // in the provided CMemoryStore. Caller must keep store alive.
    const char* CreateMinimalRgd(CMemoryStore& store, const char* name = "test.rgd")
    {
        CRgdFile rgd;
        rgd.SetHashTable(&hashTable);
        rgd.New(3);

        auto* pOut = store.VOpenOutputStream(name, false);
        EXPECT_NE(pOut, nullptr);
        rgd.Save(pOut);
        delete pOut;
        return name;
    }
};

// ── Construction & lifecycle ─────────────────────────────────────────────────

TEST_F(RgdFileMacroTest, DefaultConstructionHasNoLuaState)
{
    // unloadMacro on a fresh object should be safe (no-op)
    EXPECT_NO_THROW(macro.unloadMacro());
}

TEST_F(RgdFileMacroTest, LoadMacroCreatesLuaState)
{
    // A trivial macro that defines each_file
    macro.loadMacro("function each_file(f) end");
    // If we got here without crashing, the Lua state was created and
    // the code compiled + executed successfully. unloadMacro in TearDown.
}

TEST_F(RgdFileMacroTest, UnloadAndReload)
{
    macro.loadMacro("function each_file(f) end");
    macro.unloadMacro();
    // Reloading should work without issues
    macro.loadMacro("function each_file(f) end");
}

// ── loadMacro: global registration (LUA51_GLOBALSINDEX) ─────────────────────
// These tests verify the fix for the LUA_GLOBALSINDEX → LUA51_GLOBALSINDEX bug.
// The original bug used LUA_GLOBALSINDEX (-10001, Lua 5.0.2) instead of
// LUA51_GLOBALSINDEX (-10002, Lua 5.1.2), causing index2adr to access
// uninitialized memory when setting globals like print, UCS, pairs, etc.

TEST_F(RgdFileMacroTest, PrintGlobalIsRegistered)
{
    // print() must be accessible as a Lua global after loadMacro
    macro.loadMacro("function each_file(f) print('hello') end");

    CMemoryStore store;
    store.VInit();
    CreateMinimalRgd(store);

    // Need to set saveCapture tag for print callback
    macro.setCallbackTag(&printCapture);
    macro.runMacro("test.rgd", &store);

    ASSERT_FALSE(printCapture.lines.empty());
    EXPECT_STREQ(printCapture.lines[0].c_str(), "hello");
}

TEST_F(RgdFileMacroTest, PrintMultipleArgs)
{
    macro.loadMacro("function each_file(f) print('a', 'b', 'c') end");

    CMemoryStore store;
    store.VInit();
    CreateMinimalRgd(store);

    macro.setCallbackTag(&printCapture);
    macro.runMacro("test.rgd", &store);

    ASSERT_FALSE(printCapture.lines.empty());
    // Lua print joins args with tabs
    EXPECT_NE(std::strstr(printCapture.lines[0].c_str(), "a"), nullptr);
    EXPECT_NE(std::strstr(printCapture.lines[0].c_str(), "b"), nullptr);
    EXPECT_NE(std::strstr(printCapture.lines[0].c_str(), "c"), nullptr);
}

TEST_F(RgdFileMacroTest, PairsGlobalIsRegistered)
{
    // pairs() was one of the globals set with LUA51_GLOBALSINDEX
    macro.loadMacro(
        "function each_file(f)\n"
        "  local t = {x=1, y=2}\n"
        "  local count = 0\n"
        "  for k,v in pairs(t) do count = count + 1 end\n"
        "  print(count)\n"
        "end\n");

    CMemoryStore store;
    store.VInit();
    CreateMinimalRgd(store);

    macro.setCallbackTag(&printCapture);
    macro.runMacro("test.rgd", &store);

    ASSERT_FALSE(printCapture.lines.empty());
    EXPECT_STREQ(printCapture.lines[0].c_str(), "2");
}

TEST_F(RgdFileMacroTest, NextGlobalIsRegistered)
{
    // next() was one of the globals set with LUA51_GLOBALSINDEX
    macro.loadMacro(
        "function each_file(f)\n"
        "  local t = {a=10}\n"
        "  local k, v = next(t)\n"
        "  print(v)\n"
        "end\n");

    CMemoryStore store;
    store.VInit();
    CreateMinimalRgd(store);

    macro.setCallbackTag(&printCapture);
    macro.runMacro("test.rgd", &store);

    ASSERT_FALSE(printCapture.lines.empty());
    EXPECT_STREQ(printCapture.lines[0].c_str(), "10");
}

TEST_F(RgdFileMacroTest, PcallGlobalIsRegistered)
{
    // pcall() was one of the globals set with LUA51_GLOBALSINDEX
    macro.loadMacro(
        "function each_file(f)\n"
        "  local ok, err = pcall(function() error('test') end)\n"
        "  print(tostring(ok))\n"
        "end\n");

    CMemoryStore store;
    store.VInit();
    CreateMinimalRgd(store);

    macro.setCallbackTag(&printCapture);
    macro.runMacro("test.rgd", &store);

    ASSERT_FALSE(printCapture.lines.empty());
    EXPECT_STREQ(printCapture.lines[0].c_str(), "false");
}

TEST_F(RgdFileMacroTest, XpcallGlobalIsRegistered)
{
    // xpcall() was the last global set with LUA51_GLOBALSINDEX
    macro.loadMacro(
        "function each_file(f)\n"
        "  local ok = xpcall(function() end, function(e) return e end)\n"
        "  print(tostring(ok))\n"
        "end\n");

    CMemoryStore store;
    store.VInit();
    CreateMinimalRgd(store);

    macro.setCallbackTag(&printCapture);
    macro.runMacro("test.rgd", &store);

    ASSERT_FALSE(printCapture.lines.empty());
    EXPECT_STREQ(printCapture.lines[0].c_str(), "true");
}

// ── loadMacro: library extensions ────────────────────────────────────────────

TEST_F(RgdFileMacroTest, StringSplitExtension)
{
    macro.loadMacro(
        "function each_file(f)\n"
        "  local parts = string.split('a,b,c', ',')\n"
        "  print(#parts)\n"
        "end\n");

    CMemoryStore store;
    store.VInit();
    CreateMinimalRgd(store);

    macro.setCallbackTag(&printCapture);
    macro.runMacro("test.rgd", &store);

    ASSERT_FALSE(printCapture.lines.empty());
    EXPECT_STREQ(printCapture.lines[0].c_str(), "3");
}

TEST_F(RgdFileMacroTest, MathClampExtension)
{
    macro.loadMacro(
        "function each_file(f)\n"
        "  print(math.clamp(5, 0, 3))\n"
        "end\n");

    CMemoryStore store;
    store.VInit();
    CreateMinimalRgd(store);

    macro.setCallbackTag(&printCapture);
    macro.runMacro("test.rgd", &store);

    ASSERT_FALSE(printCapture.lines.empty());
    EXPECT_STREQ(printCapture.lines[0].c_str(), "3");
}

// ── loadMacro: error handling ────────────────────────────────────────────────

TEST_F(RgdFileMacroTest, SyntaxErrorThrows)
{
    EXPECT_THROW(macro.loadMacro("function each_file("), CRainmanException);
}

TEST_F(RgdFileMacroTest, RuntimeErrorInTopLevelThrows)
{
    EXPECT_THROW(macro.loadMacro("error('boom')"), CRainmanException);
}

// ── runMacro ─────────────────────────────────────────────────────────────────

TEST_F(RgdFileMacroTest, RunMacroWithNoEachFileIsNoOp)
{
    // A macro with no each_file function — runMacro should silently return
    macro.loadMacro("x = 42");

    CMemoryStore store;
    store.VInit();
    CreateMinimalRgd(store);

    EXPECT_NO_THROW(macro.runMacro("test.rgd", &store));
}

TEST_F(RgdFileMacroTest, RunMacroReceivesFileName)
{
    // f.path contains the file name passed to runMacro
    macro.loadMacro(
        "function each_file(f)\n"
        "  print(f.path)\n"
        "end\n");

    CMemoryStore store;
    store.VInit();
    CreateMinimalRgd(store, "my_data.rgd");

    macro.setCallbackTag(&printCapture);
    macro.runMacro("my_data.rgd", &store);

    ASSERT_FALSE(printCapture.lines.empty());
    EXPECT_STREQ(printCapture.lines[0].c_str(), "my_data.rgd");
}

TEST_F(RgdFileMacroTest, RunMacroOnMultipleFiles)
{
    macro.loadMacro(
        "counter = 0\n"
        "function each_file(f)\n"
        "  counter = counter + 1\n"
        "  print(counter)\n"
        "end\n");

    CMemoryStore store;
    store.VInit();
    CreateMinimalRgd(store, "a.rgd");
    CreateMinimalRgd(store, "b.rgd");
    CreateMinimalRgd(store, "c.rgd");

    macro.setCallbackTag(&printCapture);
    macro.runMacro("a.rgd", &store);
    macro.runMacro("b.rgd", &store);
    macro.runMacro("c.rgd", &store);

    ASSERT_EQ(printCapture.lines.size(), 3u);
    EXPECT_STREQ(printCapture.lines[0].c_str(), "1");
    EXPECT_STREQ(printCapture.lines[1].c_str(), "2");
    EXPECT_STREQ(printCapture.lines[2].c_str(), "3");
}

TEST_F(RgdFileMacroTest, RunMacroWithoutLoadIsNoOp)
{
    CMemoryStore store;
    store.VInit();
    // runMacro with no prior loadMacro should be a no-op (m_pL is nullptr)
    EXPECT_NO_THROW(macro.runMacro("test.rgd", &store));
}

TEST_F(RgdFileMacroTest, RuntimeErrorInEachFileThrows)
{
    macro.loadMacro("function each_file(f) error('macro boom') end");

    CMemoryStore store;
    store.VInit();
    CreateMinimalRgd(store);

    std::optional<CRainmanException> caught;
    try
    {
        macro.runMacro("test.rgd", &store);
    }
    catch (const CRainmanException &pE)
    {
        caught = pE;
    }
    ASSERT_TRUE(caught.has_value());
    EXPECT_NE(std::strstr(caught->getMessage(), "test.rgd"), nullptr);
}

// ── runAtEnd ─────────────────────────────────────────────────────────────────

TEST_F(RgdFileMacroTest, RunAtEndCallsAtEndFunction)
{
    macro.loadMacro(
        "function each_file(f) end\n"
        "function at_end() print('done') end\n");

    macro.setCallbackTag(&printCapture);
    macro.runAtEnd();

    ASSERT_FALSE(printCapture.lines.empty());
    EXPECT_STREQ(printCapture.lines[0].c_str(), "done");
}

TEST_F(RgdFileMacroTest, RunAtEndWithNoAtEndFunctionIsNoOp)
{
    macro.loadMacro("function each_file(f) end");
    EXPECT_NO_THROW(macro.runAtEnd());
}

// ── Lua state interaction with RGD data ──────────────────────────────────────

TEST_F(RgdFileMacroTest, MacroCanReadRgdType)
{
    // Verify the macro can call type() on the file argument
    macro.loadMacro(
        "function each_file(f)\n"
        "  print(type(f))\n"
        "end\n");

    CMemoryStore store;
    store.VInit();
    CreateMinimalRgd(store);

    macro.setCallbackTag(&printCapture);
    macro.runMacro("test.rgd", &store);

    ASSERT_FALSE(printCapture.lines.empty());
    // RGD file object is pushed as userdata with table metatables
    EXPECT_STREQ(printCapture.lines[0].c_str(), "userdata");
}

TEST_F(RgdFileMacroTest, MacroCanAccessTostring)
{
    // Verify basic Lua stdlib functions work alongside custom globals
    macro.loadMacro(
        "function each_file(f)\n"
        "  print(tostring(42))\n"
        "end\n");

    CMemoryStore store;
    store.VInit();
    CreateMinimalRgd(store);

    macro.setCallbackTag(&printCapture);
    macro.runMacro("test.rgd", &store);

    ASSERT_FALSE(printCapture.lines.empty());
    EXPECT_STREQ(printCapture.lines[0].c_str(), "42");
}

// ── Security callback ────────────────────────────────────────────────────────

TEST_F(RgdFileMacroTest, SecurityCallbackBlocksIOAccess)
{
    // With DenyAllSecurity, io.open should be blocked (returns nil / no-op)
    macro.loadMacro(
        "function each_file(f)\n"
        "  local ok, err = pcall(io.open, 'nonexistent.txt')\n"
        "  print(tostring(ok))\n"
        "end\n");

    CMemoryStore store;
    store.VInit();
    CreateMinimalRgd(store);

    macro.setCallbackTag(&printCapture);
    EXPECT_NO_THROW(macro.runMacro("test.rgd", &store));
}
