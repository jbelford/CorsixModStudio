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

#include "views/interfaces/IUcsEditorView.h"
#include "presenters/CUcsEditorPresenter.h"
#include <rainman/localization/CUcsFile.h>
#include <gtest/gtest.h>

// --- Mock view ---

class MockUcsEditorView : public IUcsEditorView
{
  public:
    wxString lastErrorMessage;
    int errorCount = 0;
    wxString lastInfoTitle;
    wxString lastInfoMessage;
    bool confirmResult = true;

    void ShowError(const wxString &sMessage) override
    {
        lastErrorMessage = sMessage;
        errorCount++;
    }

    void ShowInfo(const wxString &sTitle, const wxString &sMessage) override
    {
        lastInfoTitle = sTitle;
        lastInfoMessage = sMessage;
    }

    bool ConfirmOutOfRangeId(unsigned long /*iId*/) override { return confirmResult; }
};

// --- IsIdInRecommendedRange tests ---

TEST(CUcsEditorPresenterTest, IsIdInRange_InRange)
{
    EXPECT_TRUE(CUcsEditorPresenter::IsIdInRecommendedRange(15000000));
    EXPECT_TRUE(CUcsEditorPresenter::IsIdInRecommendedRange(17500000));
    EXPECT_TRUE(CUcsEditorPresenter::IsIdInRecommendedRange(20000000));
}

TEST(CUcsEditorPresenterTest, IsIdInRange_OutOfRange)
{
    EXPECT_FALSE(CUcsEditorPresenter::IsIdInRecommendedRange(14999999));
    EXPECT_FALSE(CUcsEditorPresenter::IsIdInRecommendedRange(20000001));
    EXPECT_FALSE(CUcsEditorPresenter::IsIdInRecommendedRange(0));
    EXPECT_FALSE(CUcsEditorPresenter::IsIdInRecommendedRange(1000));
}

// --- SuggestNextId tests ---

TEST(CUcsEditorPresenterTest, SuggestNextId_EmptyMap)
{
    CUcsFile::UcsMap entries;
    EXPECT_EQ(CUcsEditorPresenter::SuggestNextId(entries), CUcsEditorPresenter::kMinRecommendedId);
}

TEST(CUcsEditorPresenterTest, SuggestNextId_WithEntries)
{
    wchar_t buf1[] = L"hello";
    wchar_t buf2[] = L"world";
    CUcsFile::UcsMap entries;
    entries[15000001] = buf1;
    entries[15000005] = buf2;

    EXPECT_EQ(CUcsEditorPresenter::SuggestNextId(entries), 15000006UL);
}

// --- ValidateNewId tests ---

TEST(CUcsEditorPresenterTest, ValidateNewId_Valid)
{
    wchar_t buf[] = L"hello";
    CUcsFile::UcsMap entries;
    entries[15000001] = buf;

    EXPECT_EQ(CUcsEditorPresenter::ValidateNewId(15000002, entries), CUcsEditorPresenter::IdValidation::Valid);
}

TEST(CUcsEditorPresenterTest, ValidateNewId_Duplicate)
{
    wchar_t buf[] = L"hello";
    CUcsFile::UcsMap entries;
    entries[15000001] = buf;

    EXPECT_EQ(CUcsEditorPresenter::ValidateNewId(15000001, entries), CUcsEditorPresenter::IdValidation::Duplicate);
}

TEST(CUcsEditorPresenterTest, ValidateNewId_NullEntry_NotDuplicate)
{
    CUcsFile::UcsMap entries;
    entries[15000001] = nullptr;

    EXPECT_EQ(CUcsEditorPresenter::ValidateNewId(15000001, entries), CUcsEditorPresenter::IdValidation::Valid);
}

// --- ParseIdFromInput tests ---

TEST(CUcsEditorPresenterTest, ParseIdFromInput_WithDollarPrefix)
{
    unsigned long iId = 0;
    EXPECT_TRUE(CUcsEditorPresenter::ParseIdFromInput(wxT("$15000001"), iId));
    EXPECT_EQ(iId, 15000001UL);
}

TEST(CUcsEditorPresenterTest, ParseIdFromInput_PlainNumber)
{
    unsigned long iId = 0;
    EXPECT_TRUE(CUcsEditorPresenter::ParseIdFromInput(wxT("15000001"), iId));
    EXPECT_EQ(iId, 15000001UL);
}

TEST(CUcsEditorPresenterTest, ParseIdFromInput_NoDigits)
{
    unsigned long iId = 0;
    EXPECT_FALSE(CUcsEditorPresenter::ParseIdFromInput(wxT("$abc"), iId));
}

TEST(CUcsEditorPresenterTest, ParseIdFromInput_Empty)
{
    unsigned long iId = 0;
    EXPECT_FALSE(CUcsEditorPresenter::ParseIdFromInput(wxT(""), iId));
}

// --- BuildLocaleBasePath tests ---

TEST(CUcsEditorPresenterTest, BuildLocaleBasePath_CoH)
{
    auto path = CUcsEditorPresenter::BuildLocaleBasePath(wxT("C:\\Games\\CoH"), CModuleFile::MT_CompanyOfHeroes,
                                                         wxT(""), wxT("MyMod"), wxT("english"));
    EXPECT_EQ(path, wxT("C:\\Games\\CoH\\Engine\\Locale\\english\\"));
}

TEST(CUcsEditorPresenterTest, BuildLocaleBasePath_DoW_WithLocaleFolder)
{
    auto path = CUcsEditorPresenter::BuildLocaleBasePath(wxT("C:\\Games\\DoW"), CModuleFile::MT_DawnOfWar,
                                                         wxT("LocaleData"), wxT("MyMod"), wxT("english"));
    EXPECT_EQ(path, wxT("C:\\Games\\DoW\\LocaleData\\english\\"));
}

TEST(CUcsEditorPresenterTest, BuildLocaleBasePath_DoW_NoLocaleFolder)
{
    auto path = CUcsEditorPresenter::BuildLocaleBasePath(wxT("C:\\Games\\DoW"), CModuleFile::MT_DawnOfWar, wxT(""),
                                                         wxT("MyMod"), wxT("english"));
    EXPECT_EQ(path, wxT("C:\\Games\\DoW\\MyMod\\Locale\\english\\"));
}

TEST(CUcsEditorPresenterTest, BuildLocaleBasePath_TrailingBackslash)
{
    auto path = CUcsEditorPresenter::BuildLocaleBasePath(wxT("C:\\Games\\DoW\\"), CModuleFile::MT_DawnOfWar, wxT(""),
                                                         wxT("MyMod"), wxT("french"));
    EXPECT_EQ(path, wxT("C:\\Games\\DoW\\MyMod\\Locale\\french\\"));
}
