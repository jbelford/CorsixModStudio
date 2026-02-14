#include <gtest/gtest.h>
#include <wx/init.h>
#include <wx/fileconf.h>
#include "config.h"

class ConfigTest : public ::testing::Test {
protected:
    static void SetUpTestSuite() { wxInitialize(); }
    static void TearDownTestSuite() { wxUninitialize(); }

    wxFileConfig* m_pOldConfig = nullptr;

    void SetUp() override {
        // Install an empty in-memory wxFileConfig so reads return defaults
        auto* memConfig = new wxFileConfig(wxEmptyString, wxEmptyString,
                                           wxEmptyString, wxEmptyString,
                                           wxCONFIG_USE_LOCAL_FILE);
        m_pOldConfig = static_cast<wxFileConfig*>(wxConfigBase::Set(memConfig));
    }

    void TearDown() override {
        // Restore original config (deletes the one we installed)
        delete wxConfigBase::Set(m_pOldConfig);
    }
};

TEST_F(ConfigTest, ConfGetColourReturnsDefaultWhenKeyMissing) {
    wxColour colour = ConfGetColour(wxT("/test/missing"), 10, 20, 30);
    EXPECT_EQ(colour.Red(), 10);
    EXPECT_EQ(colour.Green(), 20);
    EXPECT_EQ(colour.Blue(), 30);
}

TEST_F(ConfigTest, ConfGetColourReturnsStoredValues) {
    wxConfigBase::Get()->Write(wxT("/test/colour/r"), 100);
    wxConfigBase::Get()->Write(wxT("/test/colour/g"), 150);
    wxConfigBase::Get()->Write(wxT("/test/colour/b"), 200);
    wxColour colour = ConfGetColour(wxT("/test/colour"), 0, 0, 0);
    EXPECT_EQ(colour.Red(), 100);
    EXPECT_EQ(colour.Green(), 150);
    EXPECT_EQ(colour.Blue(), 200);
}

TEST_F(ConfigTest, ConfGetColourPartialOverride) {
    // Only set red, leave green and blue as defaults
    wxConfigBase::Get()->Write(wxT("/test/partial/r"), 255);
    wxColour colour = ConfGetColour(wxT("/test/partial"), 0, 50, 100);
    EXPECT_EQ(colour.Red(), 255);
    EXPECT_EQ(colour.Green(), 50);
    EXPECT_EQ(colour.Blue(), 100);
}
