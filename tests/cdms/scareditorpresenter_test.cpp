/*
    This file is part of Corsix's Mod Studio.

    Corsix's Mod Studio is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Corsix's Mod Studio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Corsix's Mod Studio; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "presenters/CScarEditorPresenter.h"
#include <gtest/gtest.h>

// --- ParseFunctionDefinitions tests ---

TEST(CScarEditorPresenter, ParseFunctionDefinitions_Empty)
{
    auto result = CScarEditorPresenter::ParseFunctionDefinitions(wxString());
    EXPECT_TRUE(result.empty());
}

TEST(CScarEditorPresenter, ParseFunctionDefinitions_NoFunctions)
{
    auto result = CScarEditorPresenter::ParseFunctionDefinitions(wxT("local x = 42\nprint(x)\n"));
    EXPECT_TRUE(result.empty());
}

TEST(CScarEditorPresenter, ParseFunctionDefinitions_SingleFunction)
{
    wxString src = wxT("function MyFunc()\nend\n");
    auto result = CScarEditorPresenter::ParseFunctionDefinitions(src);
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].sName, wxT("MyFunc"));
    EXPECT_EQ(result[0].iPosition, 0);
}

TEST(CScarEditorPresenter, ParseFunctionDefinitions_MultipleFunctions)
{
    wxString src = wxT("function Alpha()\nend\nfunction Beta()\nend\n");
    auto result = CScarEditorPresenter::ParseFunctionDefinitions(src);
    ASSERT_EQ(result.size(), 2u);
    // Results are sorted alphabetically
    EXPECT_EQ(result[0].sName, wxT("Alpha"));
    EXPECT_EQ(result[1].sName, wxT("Beta"));
}

TEST(CScarEditorPresenter, ParseFunctionDefinitions_SortedAlphabetically)
{
    wxString src = wxT("function Zebra()\nend\nfunction Apple()\nend\nfunction Middle()\nend\n");
    auto result = CScarEditorPresenter::ParseFunctionDefinitions(src);
    ASSERT_EQ(result.size(), 3u);
    EXPECT_EQ(result[0].sName, wxT("Apple"));
    EXPECT_EQ(result[1].sName, wxT("Middle"));
    EXPECT_EQ(result[2].sName, wxT("Zebra"));
}

TEST(CScarEditorPresenter, ParseFunctionDefinitions_PositionIsKeywordStart)
{
    // Position should be the offset of the 'f' in 'function', not the name start
    wxString src = wxT("-- comment\nfunction MyFunc()\nend\n");
    auto result = CScarEditorPresenter::ParseFunctionDefinitions(src);
    ASSERT_EQ(result.size(), 1u);
    int expectedPos = static_cast<int>(wxString(wxT("-- comment\n")).length());
    EXPECT_EQ(result[0].iPosition, expectedPos);
}

TEST(CScarEditorPresenter, ParseFunctionDefinitions_DottedName)
{
    wxString src = wxT("function Module.Init()\nend\n");
    auto result = CScarEditorPresenter::ParseFunctionDefinitions(src);
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].sName, wxT("Module.Init"));
}

TEST(CScarEditorPresenter, ParseFunctionDefinitions_ColonName)
{
    wxString src = wxT("function obj:Method()\nend\n");
    auto result = CScarEditorPresenter::ParseFunctionDefinitions(src);
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].sName, wxT("obj:Method"));
}

TEST(CScarEditorPresenter, ParseFunctionDefinitions_SkipsAnonymous)
{
    // "function(" with no name between function and ( should be skipped
    wxString src = wxT("local f = function()\nend\n");
    auto result = CScarEditorPresenter::ParseFunctionDefinitions(src);
    EXPECT_TRUE(result.empty());
}

TEST(CScarEditorPresenter, ParseFunctionDefinitions_SkipsDashedNames)
{
    // Names containing dashes are invalid Lua identifiers and should be skipped
    wxString src = wxT("function my-func()\nend\n");
    auto result = CScarEditorPresenter::ParseFunctionDefinitions(src);
    EXPECT_TRUE(result.empty());
}

TEST(CScarEditorPresenter, ParseFunctionDefinitions_WhitespaceAfterKeyword)
{
    wxString src = wxT("function   SpacedFunc()\nend\n");
    auto result = CScarEditorPresenter::ParseFunctionDefinitions(src);
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].sName, wxT("SpacedFunc"));
}

TEST(CScarEditorPresenter, ParseFunctionDefinitions_TabAfterKeyword)
{
    wxString src = wxT("function\tTabbedFunc()\nend\n");
    auto result = CScarEditorPresenter::ParseFunctionDefinitions(src);
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].sName, wxT("TabbedFunc"));
}

// --- ParseLuaError tests ---

TEST(CScarEditorPresenter, ParseLuaError_NullInput)
{
    CScarEditorPresenter::LuaError err;
    EXPECT_FALSE(CScarEditorPresenter::ParseLuaError(nullptr, err));
}

TEST(CScarEditorPresenter, ParseLuaError_NoColon)
{
    CScarEditorPresenter::LuaError err;
    EXPECT_FALSE(CScarEditorPresenter::ParseLuaError("no colon here", err));
}

TEST(CScarEditorPresenter, ParseLuaError_ValidError)
{
    CScarEditorPresenter::LuaError err;
    EXPECT_TRUE(CScarEditorPresenter::ParseLuaError(":42: unexpected symbol near '+'", err));
    EXPECT_EQ(err.iLine, 42u);
    EXPECT_EQ(err.sMessage, wxT(" unexpected symbol near '+'"));
}

TEST(CScarEditorPresenter, ParseLuaError_ChunkNamePrefix)
{
    // Lua errors often look like "[string \"\"]:10: ..."
    CScarEditorPresenter::LuaError err;
    EXPECT_TRUE(CScarEditorPresenter::ParseLuaError("[string \"\"]:10: syntax error", err));
    EXPECT_EQ(err.iLine, 10u);
    EXPECT_EQ(err.sMessage, wxT(" syntax error"));
}

TEST(CScarEditorPresenter, ParseLuaError_LineOne)
{
    CScarEditorPresenter::LuaError err;
    EXPECT_TRUE(CScarEditorPresenter::ParseLuaError(":1: '<eof>' expected", err));
    EXPECT_EQ(err.iLine, 1u);
}
