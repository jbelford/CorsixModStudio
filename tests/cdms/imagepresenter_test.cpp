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

#include "views/interfaces/IImageView.h"
#include "presenters/CImagePresenter.h"
#include <gtest/gtest.h>
#include <string>

// --- Mock view ---

class MockImageView : public IImageView
{
  public:
    bool confirmOverwriteResult = true;
    bool confirmOverwriteCalled = false;
    wxString lastErrorMessage;
    int errorCount = 0;
    wxString lastSaveCompletePath;
    bool saveCompleteCalled = false;

    bool ConfirmOverwrite() override
    {
        confirmOverwriteCalled = true;
        return confirmOverwriteResult;
    }

    void ShowError(const wxString &sMessage) override
    {
        lastErrorMessage = sMessage;
        errorCount++;
    }

    void OnSaveComplete(const wxString &sNewFile) override
    {
        saveCompleteCalled = true;
        lastSaveCompletePath = sNewFile;
    }
};

// --- Static utility tests ---

TEST(CImagePresenterTest, ComputeOutputPath_RGT)
{
    auto result = CImagePresenter::ComputeOutputPath(wxT("textures\\tank.rgt"), CImagePresenter::SaveFormat::RGT);
    EXPECT_EQ(result, wxT("textures\\tank.rgt"));
}

TEST(CImagePresenterTest, ComputeOutputPath_DDS)
{
    auto result = CImagePresenter::ComputeOutputPath(wxT("textures\\tank.rgt"), CImagePresenter::SaveFormat::DDS);
    EXPECT_EQ(result, wxT("textures\\tank.dds"));
}

TEST(CImagePresenterTest, ComputeOutputPath_TGA)
{
    auto result = CImagePresenter::ComputeOutputPath(wxT("textures\\tank.rgt"), CImagePresenter::SaveFormat::TGA);
    EXPECT_EQ(result, wxT("textures\\tank.tga"));
}

TEST(CImagePresenterTest, ComputeOutputPath_NoExtension)
{
    auto result = CImagePresenter::ComputeOutputPath(wxT("textures\\tank"), CImagePresenter::SaveFormat::DDS);
    EXPECT_EQ(result, wxT("textures\\tank.dds"));
}

TEST(CImagePresenterTest, CompressionToDxtLevel_RGB)
{
    EXPECT_EQ(CImagePresenter::CompressionToDxtLevel(0), -1);
}

TEST(CImagePresenterTest, CompressionToDxtLevel_RGBA)
{
    EXPECT_EQ(CImagePresenter::CompressionToDxtLevel(1), -1);
}

TEST(CImagePresenterTest, CompressionToDxtLevel_DXT1)
{
    EXPECT_EQ(CImagePresenter::CompressionToDxtLevel(2), 1);
}

TEST(CImagePresenterTest, CompressionToDxtLevel_DXT3)
{
    EXPECT_EQ(CImagePresenter::CompressionToDxtLevel(3), 3);
}

TEST(CImagePresenterTest, CompressionToDxtLevel_DXT5)
{
    EXPECT_EQ(CImagePresenter::CompressionToDxtLevel(4), 5);
}

TEST(CImagePresenterTest, WouldOverwrite_SameFormat)
{
    EXPECT_TRUE(CImagePresenter::WouldOverwrite(0, CImagePresenter::SaveFormat::RGT));
    EXPECT_TRUE(CImagePresenter::WouldOverwrite(1, CImagePresenter::SaveFormat::DDS));
    EXPECT_TRUE(CImagePresenter::WouldOverwrite(2, CImagePresenter::SaveFormat::TGA));
}

TEST(CImagePresenterTest, WouldOverwrite_DifferentFormat)
{
    EXPECT_FALSE(CImagePresenter::WouldOverwrite(0, CImagePresenter::SaveFormat::DDS));
    EXPECT_FALSE(CImagePresenter::WouldOverwrite(0, CImagePresenter::SaveFormat::TGA));
    EXPECT_FALSE(CImagePresenter::WouldOverwrite(1, CImagePresenter::SaveFormat::RGT));
    EXPECT_FALSE(CImagePresenter::WouldOverwrite(2, CImagePresenter::SaveFormat::RGT));
}

TEST(CImagePresenterTest, SaveToStream_NullRgtFile_ReportsError)
{
    MockImageView view;
    CImagePresenter presenter(view);

    // Use a non-null stream placeholder (won't be dereferenced since pRgtFile is null)
    auto *pFakeStream = reinterpret_cast<IFileStore::IOutputStream *>(0x1);

    bool result = presenter.SaveToStream(nullptr, pFakeStream, CImagePresenter::SaveFormat::TGA, 1, false);

    EXPECT_FALSE(result);
    EXPECT_EQ(view.errorCount, 1);
    EXPECT_TRUE(view.lastErrorMessage.Contains(wxT("null")));
}

TEST(CImagePresenterTest, SaveToStream_NullStream_ReportsError)
{
    MockImageView view;
    CImagePresenter presenter(view);

    // Use a non-null RgtFile placeholder (won't be dereferenced since pStream is null)
    auto *pFakeRgt = reinterpret_cast<CRgtFile *>(0x1);

    bool result = presenter.SaveToStream(pFakeRgt, nullptr, CImagePresenter::SaveFormat::TGA, 1, false);

    EXPECT_FALSE(result);
    EXPECT_EQ(view.errorCount, 1);
    EXPECT_TRUE(view.lastErrorMessage.Contains(wxT("null")));
}

TEST(CImagePresenterTest, MockView_RecordsAllCalls)
{
    MockImageView view;

    EXPECT_FALSE(view.confirmOverwriteCalled);
    view.ConfirmOverwrite();
    EXPECT_TRUE(view.confirmOverwriteCalled);

    view.ShowError(wxT("test error"));
    EXPECT_EQ(view.errorCount, 1);
    EXPECT_EQ(view.lastErrorMessage, wxT("test error"));

    EXPECT_FALSE(view.saveCompleteCalled);
    view.OnSaveComplete(wxT("path.dds"));
    EXPECT_TRUE(view.saveCompleteCalled);
    EXPECT_EQ(view.lastSaveCompletePath, wxT("path.dds"));
}
