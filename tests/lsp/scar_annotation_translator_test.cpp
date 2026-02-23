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

    // Should have @param annotations using function param names (not @args names)
    EXPECT_NE(result.text.find("---@param actor ActorTable"), std::string::npos);
    EXPECT_NE(result.text.find("---@param groupname string"), std::string::npos);
    // Should NOT use the @args name 'sgroupname' — function param name takes priority
    EXPECT_EQ(result.text.find("sgroupname"), std::string::npos);

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

TEST(ScarAnnotationTranslator, MidLineQuestionMark_NotMatched)
{
    // --? appearing inside a string or mid-comment should NOT be treated as a SCAR annotation
    std::string source =
        "local s = \"What is this --? I wonder\"\n"
        "-- Is this valid? --? maybe\n"
        "function Foo()\nend\n";
    auto result = CScarAnnotationTranslator::Translate(source);
    // Both lines should pass through unchanged (not consumed as --? blocks)
    EXPECT_NE(result.text.find("local s = \"What is this --? I wonder\""), std::string::npos);
    EXPECT_NE(result.text.find("-- Is this valid? --? maybe"), std::string::npos);
    // Line count should be preserved (no expansion or contraction)
    EXPECT_EQ(result.originalToTranslated.size(), 4u);
    EXPECT_EQ(result.translatedToOriginal.size(), 4u);
    for (int i = 0; i < 4; ++i)
    {
        EXPECT_EQ(result.originalToTranslated[i], i);
        EXPECT_EQ(result.translatedToOriginal[i], i);
    }
}

TEST(ScarAnnotationTranslator, WindowsLineEndings)
{
    std::string source = "--? @shortdesc Hello.\r\nfunction Foo()\r\nend\r\n";
    auto result = CScarAnnotationTranslator::Translate(source);
    EXPECT_NE(result.text.find("---Hello."), std::string::npos);
    EXPECT_EQ(result.text.find("--?"), std::string::npos);
}

// ---------------------------------------------------------------------------
// Function param name priority over @args names
// ---------------------------------------------------------------------------

TEST(ScarAnnotationTranslator, Args_UsesFunctionParamNames)
{
    // @args says "sgroupname" but function param is "groupname"
    std::string source =
        "--? @args ActorTable actor, String sgroupname\n"
        "function Foo(actor, groupname)\nend\n";
    auto result = CScarAnnotationTranslator::Translate(source);
    // Should use function param names, not @args names
    EXPECT_NE(result.text.find("---@param actor ActorTable"), std::string::npos);
    EXPECT_NE(result.text.find("---@param groupname string"), std::string::npos);
    EXPECT_EQ(result.text.find("sgroupname"), std::string::npos);
}

TEST(ScarAnnotationTranslator, Args_FallsBackToArgsNamesWithoutFunction)
{
    // No function declaration follows — use @args names as fallback
    std::string source = "--? @args Integer count\nlocal x = 1\n";
    auto result = CScarAnnotationTranslator::Translate(source);
    EXPECT_NE(result.text.find("---@param count integer"), std::string::npos);
}

TEST(ScarAnnotationTranslator, Args_MoreArgsThanFunctionParams)
{
    // @args has 3 params but function only has 2 — use function names for first 2, @args for 3rd
    std::string source =
        "--? @args Integer a, Integer b, Integer c\n"
        "function Add(x, y)\nend\n";
    auto result = CScarAnnotationTranslator::Translate(source);
    EXPECT_NE(result.text.find("---@param x integer"), std::string::npos);
    EXPECT_NE(result.text.find("---@param y integer"), std::string::npos);
    EXPECT_NE(result.text.find("---@param c integer"), std::string::npos);
}

// ---------------------------------------------------------------------------
// Per-tag line mapping accuracy
// ---------------------------------------------------------------------------

TEST(ScarAnnotationTranslator, LineMapping_PerTagAccuracy)
{
    // Each tag should map its emitted line back to its own original line
    std::string source =
        "--? @shortdesc Description here.\n"  // line 0
        "--? @result Boolean\n"               // line 1
        "--? @args Integer x\n"               // line 2
        "--? @extdesc\n"                      // line 3 (no output)
        "--?  Example()\n"                    // line 4
        "function Foo(x)\nend\n";             // line 5, 6

    auto result = CScarAnnotationTranslator::Translate(source);

    // Find which translated line each annotation ended up on
    // Emitted order: description(line0), @param(line2), @return(line1), extra(line4)
    // Then: function Foo(x) (line5), end (line6)

    // The @param line should map back to original line 2 (the @args line)
    // The @return line should map back to original line 1 (the @result line)
    // The description should map back to original line 0

    // Find the @param line in translated output
    std::istringstream iss(result.text);
    std::string line;
    int translatedLineIdx = 0;
    while (std::getline(iss, line))
    {
        if (line.find("---@param") != std::string::npos)
        {
            EXPECT_EQ(result.translatedToOriginal[translatedLineIdx], 2)
                << "@param should map to original @args line (2)";
        }
        else if (line.find("---@return") != std::string::npos)
        {
            EXPECT_EQ(result.translatedToOriginal[translatedLineIdx], 1)
                << "@return should map to original @result line (1)";
        }
        else if (line.find("---Description") != std::string::npos)
        {
            EXPECT_EQ(result.translatedToOriginal[translatedLineIdx], 0)
                << "description should map to original @shortdesc line (0)";
        }
        else if (line.find("function Foo") != std::string::npos)
        {
            EXPECT_EQ(result.translatedToOriginal[translatedLineIdx], 5)
                << "function line should map to original line 5";
        }
        translatedLineIdx++;
    }
}

TEST(ScarAnnotationTranslator, MapToOriginal_BlockLineColumnMapping)
{
    // When a diagnostic comes from a translated ---@param line, the character
    // position should be mapped to where the type name appears on the original
    // --? @args line (not preserved as-is, and not reset to 0).
    std::string input =
        "--? @shortdesc Do something.\n"
        "--? @args PlayerID id, StringTable list\n"
        "function Foo(id, list)\n"
        "end\n";
    auto result = CScarAnnotationTranslator::Translate(input);

    // Find the translated lines for each ---@param
    int paramIdLine = -1;
    int paramListLine = -1;
    std::istringstream iss(result.text);
    std::string line;
    for (int idx = 0; std::getline(iss, line); ++idx)
    {
        if (line.find("---@param id ") != std::string::npos)
            paramIdLine = idx;
        if (line.find("---@param list ") != std::string::npos)
            paramListLine = idx;
    }
    ASSERT_GE(paramIdLine, 0) << "Should find ---@param id line";
    ASSERT_GE(paramListLine, 0) << "Should find ---@param list line";

    // These should have Delta column mapping
    ASSERT_LT(paramIdLine, static_cast<int>(result.columnMapKind.size()));
    ASSERT_LT(paramListLine, static_cast<int>(result.columnMapKind.size()));
    EXPECT_EQ(result.columnMapKind[paramIdLine], lsp::ColumnMapKind::Delta);
    EXPECT_EQ(result.columnMapKind[paramListLine], lsp::ColumnMapKind::Delta);

    // On translated line "---@param id PlayerID":
    //   "PlayerID" starts at column: 0 + 10 + 2 + 0 + 1 = 13
    // On original line "--? @args PlayerID id, StringTable list":
    //   "PlayerID" starts at column 10
    // So delta = 10 - 13 = -3
    Position diagId{paramIdLine, 13};
    Position mappedId = CScarAnnotationTranslator::MapToOriginal(result, diagId);
    EXPECT_EQ(mappedId.line, 1);
    EXPECT_EQ(mappedId.character, 10) << "PlayerID should map to column 10 on @args line";

    // On translated line "---@param list StringTable":
    //   "StringTable" starts at column: 0 + 10 + 4 + 0 + 1 = 15
    // On original line "--? @args PlayerID id, StringTable list":
    //   "StringTable" starts at column 23
    // So delta = 23 - 15 = 8
    Position diagList{paramListLine, 15};
    Position mappedList = CScarAnnotationTranslator::MapToOriginal(result, diagList);
    EXPECT_EQ(mappedList.line, 1);
    EXPECT_EQ(mappedList.character, 23) << "StringTable should map to column 23 on @args line";

    // End of "StringTable" range (column 15+11=26) should map correctly too
    Position diagListEnd{paramListLine, 26};
    Position mappedListEnd = CScarAnnotationTranslator::MapToOriginal(result, diagListEnd);
    EXPECT_EQ(mappedListEnd.character, 34) << "End of StringTable should map to column 34";

    // Passthrough lines should preserve their character position
    int funcLine = -1;
    iss.clear();
    iss.str(result.text);
    for (int idx = 0; std::getline(iss, line); ++idx)
    {
        if (line.find("function Foo") != std::string::npos)
        {
            funcLine = idx;
            break;
        }
    }
    ASSERT_GE(funcLine, 0);
    EXPECT_EQ(result.columnMapKind[funcLine], lsp::ColumnMapKind::Passthrough);
    Position funcPos{funcLine, 9};
    Position funcMapped = CScarAnnotationTranslator::MapToOriginal(result, funcPos);
    EXPECT_EQ(funcMapped.line, 2);
    EXPECT_EQ(funcMapped.character, 9) << "Character should be preserved for passthrough lines";

    // Description lines should have Reset mapping
    iss.clear();
    iss.str(result.text);
    for (int idx = 0; std::getline(iss, line); ++idx)
    {
        if (line.find("---Do something") != std::string::npos)
        {
            EXPECT_EQ(result.columnMapKind[idx], lsp::ColumnMapKind::Reset);
            break;
        }
    }
}
