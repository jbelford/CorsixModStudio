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

#include "views/interfaces/IModuleSettingsView.h"
#include "presenters/CModuleSettingsPresenter.h"
#include <gtest/gtest.h>

// --- Mock view ---

class MockModuleSettingsView : public IModuleSettingsView
{
  public:
    wxString lastErrorMessage;
    int errorCount = 0;

    void ShowError(const wxString &sMessage) override
    {
        lastErrorMessage = sMessage;
        errorCount++;
    }
};

// --- IsEngineDirectory tests ---

TEST(CModuleSettingsPresenterTest, IsEngineDirectory_KnownDirs)
{
    EXPECT_TRUE(CModuleSettingsPresenter::IsEngineDirectory(wxT("Engine")));
    EXPECT_TRUE(CModuleSettingsPresenter::IsEngineDirectory(wxT("Badges")));
    EXPECT_TRUE(CModuleSettingsPresenter::IsEngineDirectory(wxT("Profiles")));
    EXPECT_TRUE(CModuleSettingsPresenter::IsEngineDirectory(wxT("Utils")));
}

TEST(CModuleSettingsPresenterTest, IsEngineDirectory_ModDirs)
{
    EXPECT_FALSE(CModuleSettingsPresenter::IsEngineDirectory(wxT("MyMod")));
    EXPECT_FALSE(CModuleSettingsPresenter::IsEngineDirectory(wxT("W40k")));
    EXPECT_FALSE(CModuleSettingsPresenter::IsEngineDirectory(wxT("DXP2")));
}

// --- IsEngineDll tests ---

TEST(CModuleSettingsPresenterTest, IsEngineDll_KnownDlls)
{
    EXPECT_TRUE(CModuleSettingsPresenter::IsEngineDll(wxT("SimEngine")));
    EXPECT_TRUE(CModuleSettingsPresenter::IsEngineDll(wxT("mfc71")));
    EXPECT_TRUE(CModuleSettingsPresenter::IsEngineDll(wxT("luabind")));
}

TEST(CModuleSettingsPresenterTest, IsEngineDll_CaseInsensitive)
{
    EXPECT_TRUE(CModuleSettingsPresenter::IsEngineDll(wxT("simengine")));
    EXPECT_TRUE(CModuleSettingsPresenter::IsEngineDll(wxT("SIMENGINE")));
}

TEST(CModuleSettingsPresenterTest, IsEngineDll_ModDlls)
{
    EXPECT_FALSE(CModuleSettingsPresenter::IsEngineDll(wxT("MyModDll")));
    EXPECT_FALSE(CModuleSettingsPresenter::IsEngineDll(wxT("W40kDataDll")));
}

// --- FilterModFolders tests ---

TEST(CModuleSettingsPresenterTest, FilterModFolders_RemovesEngine)
{
    wxArrayString aAll;
    aAll.Add(wxT("Engine"));
    aAll.Add(wxT("MyMod"));
    aAll.Add(wxT("Profiles"));
    aAll.Add(wxT("DXP2"));

    auto aResult = CModuleSettingsPresenter::FilterModFolders(aAll);
    EXPECT_EQ(aResult.GetCount(), 2u);
    EXPECT_NE(aResult.Index(wxT("MyMod")), wxNOT_FOUND);
    EXPECT_NE(aResult.Index(wxT("DXP2")), wxNOT_FOUND);
    EXPECT_EQ(aResult.Index(wxT("Engine")), wxNOT_FOUND);
}

TEST(CModuleSettingsPresenterTest, FilterModFolders_EmptyInput)
{
    wxArrayString aEmpty;
    auto aResult = CModuleSettingsPresenter::FilterModFolders(aEmpty);
    EXPECT_EQ(aResult.GetCount(), 0u);
}

// --- FilterDllFiles tests ---

TEST(CModuleSettingsPresenterTest, FilterDllFiles_RemovesEngineKeepsCurrent)
{
    wxArrayString aAll;
    aAll.Add(wxT("SimEngine"));
    aAll.Add(wxT("MyModDll"));
    aAll.Add(wxT("mfc71"));
    aAll.Add(wxT("W40kDll"));

    auto aResult = CModuleSettingsPresenter::FilterDllFiles(aAll, wxT("W40kDll"));
    // W40kDll appears once (from current), MyModDll passes filter
    // SimEngine and mfc71 are filtered out
    // W40kDll also appears in input but is filtered as current → added once at end
    EXPECT_NE(aResult.Index(wxT("MyModDll")), wxNOT_FOUND);
    EXPECT_NE(aResult.Index(wxT("W40kDll")), wxNOT_FOUND);
    EXPECT_EQ(aResult.Index(wxT("SimEngine")), wxNOT_FOUND);
    EXPECT_EQ(aResult.Index(wxT("mfc71")), wxNOT_FOUND);
}

TEST(CModuleSettingsPresenterTest, FilterDllFiles_CurrentAlwaysIncluded)
{
    wxArrayString aAll;
    auto aResult = CModuleSettingsPresenter::FilterDllFiles(aAll, wxT("MyDll"));
    EXPECT_EQ(aResult.GetCount(), 1u);
    EXPECT_EQ(aResult[0], wxT("MyDll"));
}

// --- Format tests ---

TEST(CModuleSettingsPresenterTest, FormatFolderEntry)
{
    auto s = CModuleSettingsPresenter::FormatFolderEntry(42, wxT("DataAttrib"));
    EXPECT_EQ(s, wxT("[42] DataAttrib"));
}

TEST(CModuleSettingsPresenterTest, FormatArchiveEntry)
{
    auto s = CModuleSettingsPresenter::FormatArchiveEntry(1, wxT("W40k.sga"));
    EXPECT_EQ(s, wxT("[1] W40k.sga"));
}

// --- Null module tests ---

TEST(CModuleSettingsPresenterTest, GetFolderEntries_NullModule)
{
    auto aResult = CModuleSettingsPresenter::GetFolderEntries(nullptr);
    EXPECT_EQ(aResult.GetCount(), 0u);
}

TEST(CModuleSettingsPresenterTest, GetArchiveEntries_NullModule)
{
    auto aResult = CModuleSettingsPresenter::GetArchiveEntries(nullptr);
    EXPECT_EQ(aResult.GetCount(), 0u);
}

TEST(CModuleSettingsPresenterTest, GetRequiredEntries_NullModule)
{
    auto aResult = CModuleSettingsPresenter::GetRequiredEntries(nullptr);
    EXPECT_EQ(aResult.GetCount(), 0u);
}

TEST(CModuleSettingsPresenterTest, GetCompatibleEntries_NullModule)
{
    auto aResult = CModuleSettingsPresenter::GetCompatibleEntries(nullptr);
    EXPECT_EQ(aResult.GetCount(), 0u);
}
