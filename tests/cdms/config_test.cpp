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

TEST_F(ConfigTest, ConfGetColourBoundaryZeros) {
    wxColour colour = ConfGetColour(wxT("/test/zeros"), 0, 0, 0);
    EXPECT_EQ(colour.Red(), 0);
    EXPECT_EQ(colour.Green(), 0);
    EXPECT_EQ(colour.Blue(), 0);
}

TEST_F(ConfigTest, ConfGetColourBoundaryMax) {
    wxColour colour = ConfGetColour(wxT("/test/max"), 255, 255, 255);
    EXPECT_EQ(colour.Red(), 255);
    EXPECT_EQ(colour.Green(), 255);
    EXPECT_EQ(colour.Blue(), 255);
}

TEST_F(ConfigTest, ConfGetColourAllOverridden) {
    wxConfigBase::Get()->Write(wxT("/test/full/r"), 11);
    wxConfigBase::Get()->Write(wxT("/test/full/g"), 22);
    wxConfigBase::Get()->Write(wxT("/test/full/b"), 33);
    wxColour colour = ConfGetColour(wxT("/test/full"), 0, 0, 0);
    EXPECT_EQ(colour.Red(), 11);
    EXPECT_EQ(colour.Green(), 22);
    EXPECT_EQ(colour.Blue(), 33);
}

TEST_F(ConfigTest, ConfGetColourMultipleKeysIndependent) {
    wxConfigBase::Get()->Write(wxT("/test/keyA/r"), 10);
    wxConfigBase::Get()->Write(wxT("/test/keyA/g"), 20);
    wxConfigBase::Get()->Write(wxT("/test/keyA/b"), 30);
    wxConfigBase::Get()->Write(wxT("/test/keyB/r"), 200);
    wxConfigBase::Get()->Write(wxT("/test/keyB/g"), 210);
    wxConfigBase::Get()->Write(wxT("/test/keyB/b"), 220);

    wxColour colourA = ConfGetColour(wxT("/test/keyA"), 0, 0, 0);
    wxColour colourB = ConfGetColour(wxT("/test/keyB"), 0, 0, 0);

    EXPECT_EQ(colourA.Red(), 10);
    EXPECT_EQ(colourA.Green(), 20);
    EXPECT_EQ(colourA.Blue(), 30);
    EXPECT_EQ(colourB.Red(), 200);
    EXPECT_EQ(colourB.Green(), 210);
    EXPECT_EQ(colourB.Blue(), 220);
}
