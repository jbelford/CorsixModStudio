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

#include "views/interfaces/ILuaInheritTreeView.h"
#include "async/CTaskRunner.h"
#include "async/CProgressChannel.h"
#include "services/Result.h"
#include <rainman/core/Exception.h>
#include <gtest/gtest.h>
#include <chrono>
#include <condition_variable>
#include <mutex>
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

//! Mock implementation of ILuaInheritTreeView that records all calls thread-safely.
class MockLuaInheritTreeView : public ILuaInheritTreeView
{
  public:
    std::mutex mtx;
    bool loadingShown = false;
    bool loadingHidden = false;
    wxString lastLoadingMessage;
    wxString lastErrorMessage;
    bool treeDataReadyCalled = false;
    bool controlsDisabled = false;
    bool controlsEnabled = false;
    int errorCount = 0;

    void ShowLoading(const wxString &sMessage) override
    {
        std::lock_guard<std::mutex> lock(mtx);
        loadingShown = true;
        lastLoadingMessage = sMessage;
    }

    void HideLoading() override
    {
        std::lock_guard<std::mutex> lock(mtx);
        loadingHidden = true;
    }

    void ShowError(const wxString &sMessage) override
    {
        std::lock_guard<std::mutex> lock(mtx);
        lastErrorMessage = sMessage;
        errorCount++;
    }

    void OnTreeDataReady() override
    {
        std::lock_guard<std::mutex> lock(mtx);
        treeDataReadyCalled = true;
    }

    void DisableControls() override
    {
        std::lock_guard<std::mutex> lock(mtx);
        controlsDisabled = true;
    }

    void EnableControls() override
    {
        std::lock_guard<std::mutex> lock(mtx);
        controlsEnabled = true;
    }

    void Reset()
    {
        std::lock_guard<std::mutex> lock(mtx);
        loadingShown = false;
        loadingHidden = false;
        lastLoadingMessage.clear();
        lastErrorMessage.clear();
        treeDataReadyCalled = false;
        controlsDisabled = false;
        controlsEnabled = false;
        errorCount = 0;
    }
};

//! Thread-safe async result collector.
struct AsyncVoidResult
{
    std::mutex mtx;
    std::condition_variable cv;
    bool ready = false;
    Result<void> result = Result<void>::Err(wxT("not set"));

    void Set(Result<void> r)
    {
        std::lock_guard<std::mutex> lock(mtx);
        result = std::move(r);
        ready = true;
        cv.notify_all();
    }

    bool Wait(std::chrono::milliseconds timeout = std::chrono::milliseconds(5000))
    {
        std::unique_lock<std::mutex> lock(mtx);
        return cv.wait_for(lock, timeout, [this]() { return ready; });
    }
};

// --- Mock view tests ---

TEST(CLuaInheritTreePresenterTest, MockView_RecordsAllCalls)
{
    MockLuaInheritTreeView view;

    view.ShowLoading(wxT("Generating..."));
    EXPECT_TRUE(view.loadingShown);
    EXPECT_EQ(view.lastLoadingMessage, wxT("Generating..."));

    view.HideLoading();
    EXPECT_TRUE(view.loadingHidden);

    view.ShowError(wxT("failed"));
    EXPECT_EQ(view.lastErrorMessage, wxT("failed"));
    EXPECT_EQ(view.errorCount, 1);

    view.DisableControls();
    EXPECT_TRUE(view.controlsDisabled);

    view.EnableControls();
    EXPECT_TRUE(view.controlsEnabled);

    view.OnTreeDataReady();
    EXPECT_TRUE(view.treeDataReadyCalled);

    view.Reset();
    EXPECT_FALSE(view.loadingShown);
    EXPECT_EQ(view.errorCount, 0);
}

// --- Async tests using CTaskRunner directly ---
// CWxTaskRunner requires a wxApp for CallAfter. Tests use CTaskRunner
// which invokes callbacks from the background thread.

TEST(CLuaInheritTreePresenterAsyncTest, Cancel_PropagatesToken)
{
    CTaskRunner runner;
    std::atomic<bool> taskStarted{false};
    AsyncVoidResult result;

    runner.Run<void>(
        [&taskStarted](CProgressChannel & /*progress*/, CCancellationToken &cancel)
        {
            taskStarted.store(true);
            while (!cancel.IsCancelled())
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
        },
        [](const std::string &) {},
        [&result](Result<void> r) { result.Set(std::move(r)); });

    ASSERT_TRUE(WaitFor([&]() { return taskStarted.load(); }));
    runner.Cancel();
    ASSERT_TRUE(result.Wait());
    EXPECT_TRUE(result.result.ok());
}
