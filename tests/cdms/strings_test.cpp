#include <gtest/gtest.h>
#include <wx/init.h>
#include "common/strings.h"

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

TEST_F(StringsTest, AllErrorStringsAreNonEmpty) {
    EXPECT_FALSE(AppStr(err_couldnotopenoutput).IsEmpty());
    EXPECT_FALSE(AppStr(err_readmem).IsEmpty());
    EXPECT_FALSE(AppStr(err_luasyntax).IsEmpty());
    EXPECT_FALSE(AppStr(err_luaapi).IsEmpty());
    EXPECT_FALSE(AppStr(err_lualoop).IsEmpty());
    EXPECT_FALSE(AppStr(err_luarun).IsEmpty());
    EXPECT_FALSE(AppStr(err_luaprocess).IsEmpty());
    EXPECT_FALSE(AppStr(err_nogamedata).IsEmpty());
    EXPECT_FALSE(AppStr(err_cannotcopy).IsEmpty());
}

TEST_F(StringsTest, PathStringsAreNonEmpty) {
    EXPECT_FALSE(AppStr(app_filespath).IsEmpty());
    EXPECT_FALSE(AppStr(app_dictionariespath).IsEmpty());
    EXPECT_FALSE(AppStr(app_macrospath).IsEmpty());
    EXPECT_FALSE(AppStr(app_luareffile).IsEmpty());
    EXPECT_FALSE(AppStr(app_bfxmapfile).IsEmpty());
    EXPECT_FALSE(AppStr(app_scarreffile).IsEmpty());
}

TEST_F(StringsTest, CompositeStringsContainComponents) {
    // app_name = app_title + " " + app_version (SE macro)
    wxString name = AppStr(app_name);
    EXPECT_NE(name.Find(AppStr(app_title)), wxNOT_FOUND);
    EXPECT_NE(name.Find(AppStr(app_version)), wxNOT_FOUND);

    // app_dictionariespath should contain app_filespath as prefix
    EXPECT_EQ(AppStr(app_dictionariespath).Find(AppStr(app_filespath)), 0);

    // app_macrospath should contain app_filespath as prefix
    EXPECT_EQ(AppStr(app_macrospath).Find(AppStr(app_filespath)), 0);

    // welcome_caption should contain app_title
    EXPECT_NE(AppStr(welcome_caption).Find(AppStr(app_title)), wxNOT_FOUND);
}

TEST_F(StringsTest, ConfigKeyStringsStartWithSlash) {
    EXPECT_EQ(AppStr(config_firstrun)[0], wxT('/'));
    EXPECT_EQ(AppStr(config_dowfolder)[0], wxT('/'));
    EXPECT_EQ(AppStr(config_dcfolder)[0], wxT('/'));
    EXPECT_EQ(AppStr(config_ssfolder)[0], wxT('/'));
    EXPECT_EQ(AppStr(config_cohfolder)[0], wxT('/'));
    EXPECT_EQ(AppStr(config_modtoolsfolder)[0], wxT('/'));
    EXPECT_EQ(AppStr(config_sashposition)[0], wxT('/'));
    EXPECT_EQ(AppStr(config_initialpath)[0], wxT('/'));
    EXPECT_EQ(AppStr(config_donate)[0], wxT('/'));
}

TEST_F(StringsTest, ColourPostfixStrings) {
    EXPECT_EQ(AppStr(config_colour_rpost), wxT("/r"));
    EXPECT_EQ(AppStr(config_colour_gpost), wxT("/g"));
    EXPECT_EQ(AppStr(config_colour_bpost), wxT("/b"));
}

TEST_F(StringsTest, MenuStringsAreNonEmpty) {
    EXPECT_FALSE(AppStr(file_menu).IsEmpty());
    EXPECT_FALSE(AppStr(tools_menu).IsEmpty());
    EXPECT_FALSE(AppStr(help_menu).IsEmpty());
    EXPECT_FALSE(AppStr(mod_menu).IsEmpty());
    EXPECT_FALSE(AppStr(relic_tools_menu).IsEmpty());
    EXPECT_FALSE(AppStr(play_menu).IsEmpty());

    // Sub-items
    EXPECT_FALSE(AppStr(new_mod).IsEmpty());
    EXPECT_FALSE(AppStr(open_mod).IsEmpty());
    EXPECT_FALSE(AppStr(exit).IsEmpty());
    EXPECT_FALSE(AppStr(about_menu).IsEmpty());
    EXPECT_FALSE(AppStr(credits_menu).IsEmpty());
}

TEST_F(StringsTest, DecimalPointAndQuestionMark) {
    EXPECT_EQ(AppStr(decimal_point), wxT("."));
    EXPECT_EQ(AppStr(question_mark), wxT("?"));
}
