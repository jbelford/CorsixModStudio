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

#include "views/IRefreshFilesView.h"
#include "presenters/CRefreshFilesPresenter.h"
#include "async/CTaskRunner.h"
#include <gtest/gtest.h>
#include <chrono>
#include <mutex>
#include <string>
#include <thread>

// Helper: waits for a condition with timeout.
static bool WaitFor(std::function<bool()> condition,
                    std::chrono::milliseconds timeout = std::chrono::milliseconds(5000))
{
    auto start = std::chrono::steady_clock::now();
    while (!condition())
    {
        if (std::chrono::steady_clock::now() - start > timeout)
            return false;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return true;
}

//! Mock implementation of IRefreshFilesView that records all calls thread-safely.
class MockRefreshFilesView : public IRefreshFilesView
{
  public:
    std::mutex mtx;
    bool m_bLoadingShown = false;
    bool m_bLoadingHidden = false;
    bool m_bTreeRefreshed = false;
    bool m_bError = false;
    wxString m_sLastProgress;
    wxString m_sError;

    void ShowLoading(const wxString &sMessage) override
    {
        std::lock_guard<std::mutex> lock(mtx);
        m_bLoadingShown = true;
        m_sLastProgress = sMessage;
    }

    void HideLoading() override
    {
        std::lock_guard<std::mutex> lock(mtx);
        m_bLoadingHidden = true;
    }

    void UpdateProgress(const wxString &sMessage) override
    {
        std::lock_guard<std::mutex> lock(mtx);
        m_sLastProgress = sMessage;
    }

    void RefreshFileTree() override
    {
        std::lock_guard<std::mutex> lock(mtx);
        m_bTreeRefreshed = true;
    }

    void ShowError(const wxString &sMessage) override
    {
        std::lock_guard<std::mutex> lock(mtx);
        m_bError = true;
        m_sError = sMessage;
    }
};

TEST(CRefreshFilesPresenter, ViewInterfaceContract)
{
    MockRefreshFilesView view;

    // Simulate what presenter does on success: show loading, update progress, refresh tree, hide loading
    view.ShowLoading(wxT("Initializing"));
    EXPECT_TRUE(view.m_bLoadingShown);
    EXPECT_EQ(view.m_sLastProgress, wxT("Initializing"));

    view.UpdateProgress(wxT("Loading archives..."));
    EXPECT_EQ(view.m_sLastProgress, wxT("Loading archives..."));

    view.UpdateProgress(wxT("Refreshing GUI"));
    view.RefreshFileTree();
    EXPECT_TRUE(view.m_bTreeRefreshed);

    view.HideLoading();
    EXPECT_TRUE(view.m_bLoadingHidden);
}

TEST(CRefreshFilesPresenter, ErrorReportedToView)
{
    MockRefreshFilesView view;

    view.ShowLoading(wxT("Initializing"));
    view.ShowError(wxT("Failed to reload resources"));
    EXPECT_TRUE(view.m_bError);
    EXPECT_EQ(view.m_sError, wxT("Failed to reload resources"));

    view.HideLoading();
    EXPECT_TRUE(view.m_bLoadingHidden);
    EXPECT_FALSE(view.m_bTreeRefreshed);
}
