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

#include "presenters/CRgdEditorPresenter.h"
#include <gtest/gtest.h>

// --- FormatNodeName tests ---

TEST(CRgdEditorPresenter, FormatNodeName_WithName)
{
    auto result = CRgdEditorPresenter::FormatNodeName("GameData", 0);
    EXPECT_EQ(result, wxT("GameData"));
}

TEST(CRgdEditorPresenter, FormatNodeName_EmptyName)
{
    auto result = CRgdEditorPresenter::FormatNodeName("", 0xDEADBEEF);
    EXPECT_EQ(result, wxT("0xDEADBEEF"));
}

TEST(CRgdEditorPresenter, FormatNodeName_NullName)
{
    auto result = CRgdEditorPresenter::FormatNodeName(nullptr, 0x00000042);
    EXPECT_EQ(result, wxT("0x00000042"));
}

TEST(CRgdEditorPresenter, FormatNodeName_ZeroHash)
{
    auto result = CRgdEditorPresenter::FormatNodeName(nullptr, 0x00000000);
    EXPECT_EQ(result, wxT("0x00000000"));
}

TEST(CRgdEditorPresenter, FormatNodeName_MaxHash)
{
    auto result = CRgdEditorPresenter::FormatNodeName(nullptr, 0xFFFFFFFF);
    EXPECT_EQ(result, wxT("0xFFFFFFFF"));
}

// --- FormatTableReference tests ---

TEST(CRgdEditorPresenter, FormatTableReference_NoReference)
{
    auto result = CRgdEditorPresenter::FormatTableReference(wxT(""), false, false);
    EXPECT_EQ(result, wxT("{}"));
}

TEST(CRgdEditorPresenter, FormatTableReference_GameData)
{
    auto result = CRgdEditorPresenter::FormatTableReference(wxT("path/to/data"), true, true);
    EXPECT_EQ(result, wxT("Inherit([[path/to/data]])"));
}

TEST(CRgdEditorPresenter, FormatTableReference_OtherTable)
{
    auto result = CRgdEditorPresenter::FormatTableReference(wxT("path/to/data"), false, true);
    EXPECT_EQ(result, wxT("Reference([[path/to/data]])"));
}

// --- ComputeMultiHash tests ---

TEST(CRgdEditorPresenter, ComputeMultiHash_StripTrailingDigits)
{
    // "unit_01" and "unit_02" should produce the same hash
    // because trailing digits are stripped, leaving "unit_"
    unsigned long h1 = CRgdEditorPresenter::ComputeMultiHash(L"unit_01");
    unsigned long h2 = CRgdEditorPresenter::ComputeMultiHash(L"unit_02");
    EXPECT_EQ(h1, h2);
}

TEST(CRgdEditorPresenter, ComputeMultiHash_NoDigits)
{
    // No trailing digits — hash the full string
    unsigned long h = CRgdEditorPresenter::ComputeMultiHash(L"infantry");
    EXPECT_NE(h, 0u); // just check it produces something
}

TEST(CRgdEditorPresenter, ComputeMultiHash_AllDigits)
{
    // All digits — strips everything, hashes empty prefix
    unsigned long h1 = CRgdEditorPresenter::ComputeMultiHash(L"123");
    unsigned long h2 = CRgdEditorPresenter::ComputeMultiHash(L"456");
    EXPECT_EQ(h1, h2);
}

TEST(CRgdEditorPresenter, ComputeMultiHash_DifferentNames)
{
    unsigned long h1 = CRgdEditorPresenter::ComputeMultiHash(L"alpha");
    unsigned long h2 = CRgdEditorPresenter::ComputeMultiHash(L"beta");
    EXPECT_NE(h1, h2);
}

// --- NormaliseLua2SavePath tests ---

TEST(CRgdEditorPresenter, NormaliseLua2SavePath_GenericAttrib)
{
    auto result = CRgdEditorPresenter::NormaliseLua2SavePath("Generic\\Attrib\\Weapons\\Bolter.lua");
    EXPECT_EQ(result, "weapons\\bolter.lua");
}

TEST(CRgdEditorPresenter, NormaliseLua2SavePath_DataAttrib)
{
    auto result = CRgdEditorPresenter::NormaliseLua2SavePath("Data\\Attrib\\Units\\Marine.lua");
    EXPECT_EQ(result, "units\\marine.lua");
}

TEST(CRgdEditorPresenter, NormaliseLua2SavePath_AttribAttrib)
{
    auto result = CRgdEditorPresenter::NormaliseLua2SavePath("Attrib\\Attrib\\Abilities\\Heal.lua");
    EXPECT_EQ(result, "abilities\\heal.lua");
}

TEST(CRgdEditorPresenter, NormaliseLua2SavePath_NoPrefix)
{
    auto result = CRgdEditorPresenter::NormaliseLua2SavePath("Other\\Path\\File.lua");
    EXPECT_EQ(result, "other\\path\\file.lua");
}

TEST(CRgdEditorPresenter, NormaliseLua2SavePath_AlreadyLowercase)
{
    auto result = CRgdEditorPresenter::NormaliseLua2SavePath("Generic\\Attrib\\already_lower.lua");
    EXPECT_EQ(result, "already_lower.lua");
}
