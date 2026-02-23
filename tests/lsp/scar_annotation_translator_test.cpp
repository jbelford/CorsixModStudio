/*
LSP Client Library
Copyright (C) 2026 CorsixModStudio Contributors

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
#include <lsp/ScarAnnotationTranslator.h>
#include <string>

using lsp::CScarAnnotationTranslator;
using lsp::Position;
using lsp::TranslationResult;

// ---------------------------------------------------------------------------
// Type mapping
// ---------------------------------------------------------------------------

TEST(ScarAnnotationTranslator, MapType_KnownTypes)
{
    EXPECT_EQ(CScarAnnotationTranslator::MapType("Boolean"), "boolean");
    EXPECT_EQ(CScarAnnotationTranslator::MapType("Bool"), "boolean");
    EXPECT_EQ(CScarAnnotationTranslator::MapType("Integer"), "integer");
    EXPECT_EQ(CScarAnnotationTranslator::MapType("Int"), "integer");
    EXPECT_EQ(CScarAnnotationTranslator::MapType("Real"), "number");
    EXPECT_EQ(CScarAnnotationTranslator::MapType("Float"), "number");
    EXPECT_EQ(CScarAnnotationTranslator::MapType("Number"), "number");
    EXPECT_EQ(CScarAnnotationTranslator::MapType("String"), "string");
    EXPECT_EQ(CScarAnnotationTranslator::MapType("Table"), "table");
    EXPECT_EQ(CScarAnnotationTranslator::MapType("Function"), "function");
    EXPECT_EQ(CScarAnnotationTranslator::MapType("LuaFunction"), "function");
    EXPECT_EQ(CScarAnnotationTranslator::MapType("Any"), "any");
    EXPECT_EQ(CScarAnnotationTranslator::MapType("SGroupID"), "SGroupID");
    EXPECT_EQ(CScarAnnotationTranslator::MapType("Position"), "Position");
    EXPECT_EQ(CScarAnnotationTranslator::MapType("ScarPosition"), "Position");
}

TEST(ScarAnnotationTranslator, MapType_VoidReturnsEmpty)
{
    EXPECT_EQ(CScarAnnotationTranslator::MapType("Void"), "");
}

TEST(ScarAnnotationTranslator, MapType_UnknownPassesThrough)
{
    EXPECT_EQ(CScarAnnotationTranslator::MapType("CustomType"), "CustomType");
    EXPECT_EQ(CScarAnnotationTranslator::MapType("MyModClass"), "MyModClass");
}

// ---------------------------------------------------------------------------
// No-op translation (no --? comments)
// ---------------------------------------------------------------------------

TEST(ScarAnnotationTranslator, NoAnnotations_PassesThrough)
{
    std::string source = "local x = 1\nfunction Foo()\nend\n";
    auto result = CScarAnnotationTranslator::Translate(source);
    EXPECT_EQ(result.text, source);
    EXPECT_EQ(result.originalToTranslated.size(), 3u); // 3 lines
    EXPECT_EQ(result.translatedToOriginal.size(), 3u);
}

TEST(ScarAnnotationTranslator, EmptySource)
{
    auto result = CScarAnnotationTranslator::Translate("");
    EXPECT_EQ(result.text, "");
    EXPECT_TRUE(result.originalToTranslated.empty());
    EXPECT_TRUE(result.translatedToOriginal.empty());
}

// ---------------------------------------------------------------------------
// Basic @shortdesc
// ---------------------------------------------------------------------------

TEST(ScarAnnotationTranslator, Shortdesc_ConvertedToDocComment)
{
    std::string source = "--? @shortdesc This is a description.\nfunction Foo()\nend\n";
    auto result = CScarAnnotationTranslator::Translate(source);
    EXPECT_NE(result.text.find("---This is a description."), std::string::npos);
    EXPECT_EQ(result.text.find("--?"), std::string::npos); // No --? left
}

// ---------------------------------------------------------------------------
// @result
// ---------------------------------------------------------------------------

TEST(ScarAnnotationTranslator, Result_ConvertedToReturn)
{
    std::string source = "--? @result Boolean\nfunction IsAlive()\nend\n";
    auto result = CScarAnnotationTranslator::Translate(source);
    EXPECT_NE(result.text.find("---@return boolean"), std::string::npos);
}

TEST(ScarAnnotationTranslator, Result_Void_OmitsReturn)
{
    std::string source = "--? @result Void\nfunction DoStuff()\nend\n";
    auto result = CScarAnnotationTranslator::Translate(source);
    EXPECT_EQ(result.text.find("@return"), std::string::npos);
}

// ---------------------------------------------------------------------------
// @args
// ---------------------------------------------------------------------------

TEST(ScarAnnotationTranslator, Args_SingleParam)
{
    std::string source = "--? @args PlayerID player\nfunction Kill(player)\nend\n";
    auto result = CScarAnnotationTranslator::Translate(source);
    EXPECT_NE(result.text.find("---@param player PlayerID"), std::string::npos);
}

TEST(ScarAnnotationTranslator, Args_MultipleParams)
{
    std::string source = "--? @args ActorTable actor, String sgroupname\nfunction Foo(actor, sgroupname)\nend\n";
    auto result = CScarAnnotationTranslator::Translate(source);
    EXPECT_NE(result.text.find("---@param actor ActorTable"), std::string::npos);
    EXPECT_NE(result.text.find("---@param sgroupname string"), std::string::npos);
}

TEST(ScarAnnotationTranslator, Args_OptionalParam)
{
    std::string source = "--? @args PlayerID player, [Boolean verbose]\nfunction Kill(player, verbose)\nend\n";
    auto result = CScarAnnotationTranslator::Translate(source);
    EXPECT_NE(result.text.find("---@param player PlayerID"), std::string::npos);
    EXPECT_NE(result.text.find("---@param verbose? boolean"), std::string::npos);
}

// ---------------------------------------------------------------------------
// @extdesc and examples
// ---------------------------------------------------------------------------

TEST(ScarAnnotationTranslator, Extdesc_ConvertedToDocComment)
{
    std::string source =
        "--? @shortdesc Do a thing.\n"
        "--? @extdesc\n"
        "--?  MyFunc( arg1, arg2 )\n"
        "function MyFunc(a, b)\nend\n";
    auto result = CScarAnnotationTranslator::Translate(source);
    EXPECT_NE(result.text.find("---Do a thing."), std::string::npos);
    EXPECT_NE(result.text.find("---  MyFunc( arg1, arg2 )"), std::string::npos);
}

// ---------------------------------------------------------------------------
// Full block (matches the screenshot)
// ---------------------------------------------------------------------------

TEST(ScarAnnotationTranslator, FullBlock_ScreenshotExample)
{
    std::string source =
        "--? @shortdesc Assign an sgroup to a actor.  Will take the first matching blueprint from the sgroup.\n"
        "--? @result Void\n"
        "--? @args ActorTable actor, String sgroupname\n"
        "--? @extdesc\n"
        "--?  Actor_SetFromSGroup( ACT.Gabriel, \"sg_Gabriel\" )\n"
        "function Actor_SetFromSGroup( actor, groupname )\n"
        "end\n";

    auto result = CScarAnnotationTranslator::Translate(source);

    // Should contain the description
    EXPECT_NE(result.text.find("---Assign an sgroup to a actor."), std::string::npos);

    // Should have @param annotations
    EXPECT_NE(result.text.find("---@param actor ActorTable"), std::string::npos);
    EXPECT_NE(result.text.find("---@param sgroupname string"), std::string::npos);

    // Should NOT have @return (Void)
    EXPECT_EQ(result.text.find("@return"), std::string::npos);

    // Should have the example as a doc comment
    EXPECT_NE(result.text.find("---  Actor_SetFromSGroup( ACT.Gabriel"), std::string::npos);

    // Should still have the function declaration
    EXPECT_NE(result.text.find("function Actor_SetFromSGroup( actor, groupname )"), std::string::npos);

    // No --? comments should remain
    EXPECT_EQ(result.text.find("--?"), std::string::npos);
}

// ---------------------------------------------------------------------------
// Multiple blocks in one file
// ---------------------------------------------------------------------------

TEST(ScarAnnotationTranslator, MultipleBlocks)
{
    std::string source =
        "-- Header comment\n"
        "\n"
        "--? @shortdesc First function.\n"
        "--? @result Integer\n"
        "--? @args String name\n"
        "function GetId(name)\nend\n"
        "\n"
        "--? @shortdesc Second function.\n"
        "--? @result Boolean\n"
        "function IsValid()\nend\n";

    auto result = CScarAnnotationTranslator::Translate(source);
    EXPECT_NE(result.text.find("---First function."), std::string::npos);
    EXPECT_NE(result.text.find("---@return integer"), std::string::npos);
    EXPECT_NE(result.text.find("---@param name string"), std::string::npos);
    EXPECT_NE(result.text.find("---Second function."), std::string::npos);
    EXPECT_NE(result.text.find("---@return boolean"), std::string::npos);
}

// ---------------------------------------------------------------------------
// Line mapping
// ---------------------------------------------------------------------------

TEST(ScarAnnotationTranslator, LineMapping_NoExpansion)
{
    // 1:1 mapping when no --? comments
    std::string source = "local x = 1\nfunction Foo()\nend\n";
    auto result = CScarAnnotationTranslator::Translate(source);

    ASSERT_EQ(result.originalToTranslated.size(), 3u);
    ASSERT_EQ(result.translatedToOriginal.size(), 3u);

    EXPECT_EQ(result.originalToTranslated[0], 0);
    EXPECT_EQ(result.originalToTranslated[1], 1);
    EXPECT_EQ(result.originalToTranslated[2], 2);

    EXPECT_EQ(result.translatedToOriginal[0], 0);
    EXPECT_EQ(result.translatedToOriginal[1], 1);
    EXPECT_EQ(result.translatedToOriginal[2], 2);
}

TEST(ScarAnnotationTranslator, LineMapping_WithExpansion)
{
    // @args with 2 params on 1 original line expands to 2 translated lines
    std::string source =
        "--? @args Integer a, Integer b\n"
        "function Add(a, b)\nend\n";

    auto result = CScarAnnotationTranslator::Translate(source);

    // Original line 0 (--? @args...) maps to translated line 0
    EXPECT_EQ(result.originalToTranslated[0], 0);
    // Original line 1 (function) should map past the expanded params
    EXPECT_GT(result.originalToTranslated[1], 0);

    // The function line in translated text should map back to original line 1
    int funcTranslated = result.originalToTranslated[1];
    EXPECT_EQ(result.translatedToOriginal[funcTranslated], 1);
}

// ---------------------------------------------------------------------------
// Position mapping
// ---------------------------------------------------------------------------

TEST(ScarAnnotationTranslator, PositionMapping_MapToTranslated)
{
    std::string source = "local x = 1\nfunction Foo()\nend\n";
    auto result = CScarAnnotationTranslator::Translate(source);

    Position orig{1, 5};
    Position mapped = CScarAnnotationTranslator::MapToTranslated(result, orig);
    EXPECT_EQ(mapped.line, 1);
    EXPECT_EQ(mapped.character, 5); // Character unchanged
}

TEST(ScarAnnotationTranslator, PositionMapping_MapToOriginal)
{
    std::string source = "local x = 1\nfunction Foo()\nend\n";
    auto result = CScarAnnotationTranslator::Translate(source);

    Position translated{2, 0};
    Position mapped = CScarAnnotationTranslator::MapToOriginal(result, translated);
    EXPECT_EQ(mapped.line, 2);
    EXPECT_EQ(mapped.character, 0);
}

TEST(ScarAnnotationTranslator, PositionMapping_OutOfRange)
{
    std::string source = "local x = 1\n";
    auto result = CScarAnnotationTranslator::Translate(source);

    Position outOfRange{999, 0};
    Position mapped = CScarAnnotationTranslator::MapToTranslated(result, outOfRange);
    EXPECT_EQ(mapped.line, 999); // Unchanged when out of range
}

// ---------------------------------------------------------------------------
// Edge cases
// ---------------------------------------------------------------------------

TEST(ScarAnnotationTranslator, EmptyArgs)
{
    std::string source = "--? @args\nfunction NoArgs()\nend\n";
    auto result = CScarAnnotationTranslator::Translate(source);
    EXPECT_EQ(result.text.find("@param"), std::string::npos);
}

TEST(ScarAnnotationTranslator, MalformedArg_NoType)
{
    std::string source = "--? @args somearg\nfunction Foo(somearg)\nend\n";
    auto result = CScarAnnotationTranslator::Translate(source);
    // Should use "any" as fallback type
    EXPECT_NE(result.text.find("---@param somearg any"), std::string::npos);
}

TEST(ScarAnnotationTranslator, IndentedBlock)
{
    std::string source = "    --? @shortdesc Indented.\n    function Foo()\n    end\n";
    auto result = CScarAnnotationTranslator::Translate(source);
    EXPECT_NE(result.text.find("    ---Indented."), std::string::npos);
}

TEST(ScarAnnotationTranslator, RegularComments_Preserved)
{
    std::string source = "-- This is a regular comment\n--- This is a LuaLS annotation\nfunction Foo()\nend\n";
    auto result = CScarAnnotationTranslator::Translate(source);
    EXPECT_NE(result.text.find("-- This is a regular comment"), std::string::npos);
    EXPECT_NE(result.text.find("--- This is a LuaLS annotation"), std::string::npos);
}

TEST(ScarAnnotationTranslator, WindowsLineEndings)
{
    std::string source = "--? @shortdesc Hello.\r\nfunction Foo()\r\nend\r\n";
    auto result = CScarAnnotationTranslator::Translate(source);
    EXPECT_NE(result.text.find("---Hello."), std::string::npos);
    EXPECT_EQ(result.text.find("--?"), std::string::npos);
}
