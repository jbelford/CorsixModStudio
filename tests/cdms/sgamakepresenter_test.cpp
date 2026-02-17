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

#include "views/ISgaMakeView.h"
#include "presenters/CSgaMakePresenter.h"
#include "async/CTaskRunner.h"
#include "async/CProgressChannel.h"
#include "services/FileService.h"
#include "common/strconv.h"
#include <rainman/archive/CSgaCreator.h>
#include <rainman/io/CFileSystemStore.h>
#include <rainman/core/Exception.h>
#include <gtest/gtest.h>
#include <chrono>
#include <condition_variable>
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

//! Mock implementation of ISgaMakeView that records all calls thread-safely.
class MockSgaMakeView : public ISgaMakeView
{
  public:
    std::mutex mtx;
    bool progressShown = false;
    bool progressHidden = false;
    wxString lastProgressMessage;
    wxString lastErrorMessage;
    bool sgaCreatedCalled = false;
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

    void OnSgaCreated() override
    {
        std::lock_guard<std::mutex> lock(mtx);
        sgaCreatedCalled = true;
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
        sgaCreatedCalled = false;
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

TEST(CSgaMakePresenterTest, MockView_RecordsAllCalls)
{
    MockSgaMakeView view;

    view.ShowProgress(wxT("Creating..."));
    EXPECT_TRUE(view.progressShown);
    EXPECT_EQ(view.lastProgressMessage, wxT("Creating..."));

    view.HideProgress();
    EXPECT_TRUE(view.progressHidden);

    view.ShowError(wxT("bad things"));
    EXPECT_EQ(view.lastErrorMessage, wxT("bad things"));
    EXPECT_EQ(view.errorCount, 1);

    view.DisableControls();
    EXPECT_TRUE(view.controlsDisabled);

    view.EnableControls();
    EXPECT_TRUE(view.controlsEnabled);

    view.OnSgaCreated();
    EXPECT_TRUE(view.sgaCreatedCalled);

    view.Reset();
    EXPECT_FALSE(view.progressShown);
    EXPECT_EQ(view.errorCount, 0);
}

// --- Async tests using CTaskRunner directly ---
// CWxTaskRunner requires a wxApp for CallAfter. Tests use CTaskRunner
// which invokes callbacks from the background thread.

TEST(CSgaMakePresenterAsyncTest, InvalidInputDir_ReportsError)
{
    CTaskRunner runner;
    MockSgaMakeView view;
    AsyncVoidResult result;

    wxString sInputDir = wxT("C:\\nonexistent_dir_for_testing_12345");
    wxString sOutputFile = wxT("C:\\temp\\test_output.sga");
    wxString sTocName = wxT("Data");
    long iVersion = 4;

    view.DisableControls();
    view.ShowProgress(wxT("Creating SGA archive..."));

    runner.Run<void>(
        [sInputDir, sOutputFile, sTocName, iVersion](CProgressChannel & /*progress*/, CCancellationToken & /*cancel*/)
        {
            CFileSystemStore oFSO;
            oFSO.VInit();

            auto itrResult = FileService::IterateFileSystem(sInputDir);
            if (!itrResult)
                throw new CRainmanException(__FILE__, __LINE__, "Cannot iterate input directory");

            auto *pItr = itrResult.value().release();
            auto saOutput = wxStringToAscii(sOutputFile);
            auto saToc = wxStringToAscii(sTocName);

            try
            {
                CSgaCreator::CreateSga(pItr, &oFSO, saToc.get(), saOutput.get(), iVersion);
            }
            catch (...)
            {
                delete pItr;
                throw;
            }
            delete pItr;
        },
        [](const std::string &) {},
        [&view, &result](Result<void> r)
        {
            view.HideProgress();
            view.EnableControls();
            if (!r.ok())
                view.ShowError(r.error());
            result.Set(std::move(r));
        });

    ASSERT_TRUE(result.Wait());
    EXPECT_FALSE(result.result.ok());
    EXPECT_TRUE(view.progressHidden);
    EXPECT_TRUE(view.controlsEnabled);
    EXPECT_GT(view.errorCount, 0);
    EXPECT_FALSE(view.sgaCreatedCalled);
}

TEST(CSgaMakePresenterAsyncTest, Cancel_PropagatesToken)
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

TEST(CSgaMakePresenterAsyncTest, DoubleRun_Rejected)
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
