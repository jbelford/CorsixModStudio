#include <gtest/gtest.h>
#include <wx/init.h>
#include "strings.h"

class StringsTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() { wxInitialize(); }
    static void TearDownTestSuite() { wxUninitialize(); }
};

TEST_F(StringsTest, AppTitleContainsModStudio) {
    EXPECT_NE(AppStr(app_title).Find(wxT("Mod Studio")), wxNOT_FOUND);
}

TEST_F(StringsTest, AppVersionIsNonEmpty) {
    EXPECT_FALSE(AppStr(app_version).IsEmpty());
}

TEST_F(StringsTest, ConfigFileNameIsNonEmpty) {
    EXPECT_FALSE(AppStr(configfile_name).IsEmpty());
}

TEST_F(StringsTest, FileMenuStringIsNonEmpty) {
    EXPECT_FALSE(AppStr(file_menu).IsEmpty());
}

TEST_F(StringsTest, ToolsMenuStringIsNonEmpty) {
    EXPECT_FALSE(AppStr(tools_menu).IsEmpty());
}

TEST_F(StringsTest, HelpMenuStringIsNonEmpty) {
    EXPECT_FALSE(AppStr(help_menu).IsEmpty());
}

TEST_F(StringsTest, ScarBadContainsFormatPlaceholder) {
    wxString s = AppStr(scar_bad);
    EXPECT_NE(s.Find(wxT("%")), wxNOT_FOUND);
}

TEST_F(StringsTest, AppNameContainsTitleAndVersion) {
    wxString name = AppStr(app_name);
    EXPECT_NE(name.Find(AppStr(app_title)), wxNOT_FOUND);
    EXPECT_NE(name.Find(AppStr(app_version)), wxNOT_FOUND);
}

TEST_F(StringsTest, ErrorStringsAreNonEmpty) {
    EXPECT_FALSE(AppStr(err_memory).IsEmpty());
    EXPECT_FALSE(AppStr(err_unknown).IsEmpty());
    EXPECT_FALSE(AppStr(err_read).IsEmpty());
    EXPECT_FALSE(AppStr(err_write).IsEmpty());
}
