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

#include "views/IRgdMacroView.h"
#include "async/CTaskRunner.h"
#include "async/CProgressChannel.h"
#include "services/Result.h"
#include <rainman/core/Exception.h>
#include <gtest/gtest.h>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

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

//! Mock implementation of IRgdMacroView that records all calls thread-safely.
class MockRgdMacroView : public IRgdMacroView
{
  public:
    std::mutex mtx;
    bool progressShown = false;
    bool progressHidden = false;
    wxString lastProgressMessage;
    wxString lastErrorMessage;
    bool controlsDisabled = false;
    bool controlsEnabled = false;
    int errorCount = 0;
    std::vector<wxString> outputLines;
    std::vector<wxString> permissionQuestions;
    bool permissionResponse = true;
    bool macroCompleteCalled = false;
    std::vector<wxString> completedFolders;

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

    void AppendOutput(const wxString &sText) override
    {
        std::lock_guard<std::mutex> lock(mtx);
        outputLines.push_back(sText);
    }

    bool RequestPermission(const wxString &sQuestion) override
    {
        std::lock_guard<std::mutex> lock(mtx);
        permissionQuestions.push_back(sQuestion);
        return permissionResponse;
    }

    void OnMacroComplete(const std::vector<wxString> &vFoldersToUpdate) override
    {
        std::lock_guard<std::mutex> lock(mtx);
        macroCompleteCalled = true;
        completedFolders = vFoldersToUpdate;
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
        controlsDisabled = false;
        controlsEnabled = false;
        errorCount = 0;
        outputLines.clear();
        permissionQuestions.clear();
        permissionResponse = true;
        macroCompleteCalled = false;
        completedFolders.clear();
    }
};

// --- Mock view tests ---

TEST(CRgdMacroPresenterTest, MockView_RecordsAllCalls)
{
    MockRgdMacroView view;

    view.ShowProgress(wxT("Running macro..."));
    EXPECT_TRUE(view.progressShown);
    EXPECT_EQ(view.lastProgressMessage, wxT("Running macro..."));

    view.HideProgress();
    EXPECT_TRUE(view.progressHidden);

    view.ShowError(wxT("something went wrong"));
    EXPECT_EQ(view.lastErrorMessage, wxT("something went wrong"));
    EXPECT_EQ(view.errorCount, 1);

    view.AppendOutput(wxT("line 1"));
    view.AppendOutput(wxT("line 2"));
    EXPECT_EQ(view.outputLines.size(), 2u);
    EXPECT_EQ(view.outputLines[0], wxT("line 1"));
    EXPECT_EQ(view.outputLines[1], wxT("line 2"));

    view.permissionResponse = true;
    EXPECT_TRUE(view.RequestPermission(wxT("to do something")));
    EXPECT_EQ(view.permissionQuestions.size(), 1u);

    view.permissionResponse = false;
    EXPECT_FALSE(view.RequestPermission(wxT("to do something else")));
    EXPECT_EQ(view.permissionQuestions.size(), 2u);

    std::vector<wxString> folders = {wxT("folder1"), wxT("folder2")};
    view.OnMacroComplete(folders);
    EXPECT_TRUE(view.macroCompleteCalled);
    EXPECT_EQ(view.completedFolders.size(), 2u);

    view.DisableControls();
    EXPECT_TRUE(view.controlsDisabled);

    view.EnableControls();
    EXPECT_TRUE(view.controlsEnabled);

    view.Reset();
    EXPECT_FALSE(view.progressShown);
    EXPECT_FALSE(view.macroCompleteCalled);
    EXPECT_EQ(view.errorCount, 0);
    EXPECT_TRUE(view.outputLines.empty());
    EXPECT_TRUE(view.permissionQuestions.empty());
}

// --- Async tests using CTaskRunner directly ---
// CWxTaskRunner requires a wxApp for CallAfter. Tests use CTaskRunner
// which invokes callbacks from the background thread.

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

TEST(CRgdMacroPresenterAsyncTest, Cancel_PropagatesToken)
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
