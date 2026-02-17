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

#include "views/interfaces/IDpsCalculatorView.h"
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

//! Mock implementation of IDpsCalculatorView that records all calls thread-safely.
class MockDpsCalculatorView : public IDpsCalculatorView
{
  public:
    std::mutex mtx;
    bool progressShown = false;
    bool progressHidden = false;
    wxString lastProgressMessage;
    wxString lastErrorMessage;
    bool dpsCompleteCalled = false;
    bool controlsDisabled = false;
    bool controlsEnabled = false;
    int errorCount = 0;

    void ShowProgress(const wxString &sMessage) override
    {
        std::lock_guard<std::mutex> lock(mtx);
        progressShown = true;
        lastProgressMessage = sMessage;
    }

    void HideProgress() override
    {
        std::lock_guard<std::mutex> lock(mtx);
        progressHidden = true;
    }

    void ShowError(const wxString &sMessage) override
    {
        std::lock_guard<std::mutex> lock(mtx);
        lastErrorMessage = sMessage;
        errorCount++;
    }

    void OnDpsComplete() override
    {
        std::lock_guard<std::mutex> lock(mtx);
        dpsCompleteCalled = true;
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
        progressShown = false;
        progressHidden = false;
        lastProgressMessage.clear();
        lastErrorMessage.clear();
        dpsCompleteCalled = false;
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

TEST(CDpsCalculatorPresenterTest, MockView_RecordsAllCalls)
{
    MockDpsCalculatorView view;

    view.ShowProgress(wxT("Calculating..."));
    EXPECT_TRUE(view.progressShown);
    EXPECT_EQ(view.lastProgressMessage, wxT("Calculating..."));

    view.HideProgress();
    EXPECT_TRUE(view.progressHidden);

    view.ShowError(wxT("failed"));
    EXPECT_EQ(view.lastErrorMessage, wxT("failed"));
    EXPECT_EQ(view.errorCount, 1);

    view.DisableControls();
    EXPECT_TRUE(view.controlsDisabled);

    view.EnableControls();
    EXPECT_TRUE(view.controlsEnabled);

    view.OnDpsComplete();
    EXPECT_TRUE(view.dpsCompleteCalled);

    view.Reset();
    EXPECT_FALSE(view.progressShown);
    EXPECT_EQ(view.errorCount, 0);
}

// --- Async tests using CTaskRunner directly ---

TEST(CDpsCalculatorPresenterAsyncTest, Cancel_PropagatesToken)
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

TEST(CDpsCalculatorPresenterAsyncTest, DoubleRun_Rejected)
{
    CTaskRunner runner;
    std::atomic<bool> taskStarted{false};
    AsyncVoidResult result1;
    AsyncVoidResult result2;

    bool first = runner.Run<void>(
        [&taskStarted](CProgressChannel & /*progress*/, CCancellationToken &cancel)
        {
            taskStarted.store(true);
            while (!cancel.IsCancelled())
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
        },
        [](const std::string &) {},
        [&result1](Result<void> r) { result1.Set(std::move(r)); });

    EXPECT_TRUE(first);
    ASSERT_TRUE(WaitFor([&]() { return taskStarted.load(); }));

    bool second = runner.Run<void>(
        [](CProgressChannel &, CCancellationToken &) {},
        [](const std::string &) {},
        [&result2](Result<void> r) { result2.Set(std::move(r)); });

    EXPECT_FALSE(second);

    runner.Cancel();
    ASSERT_TRUE(result1.Wait());
}
