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

#include "async/CTaskRunner.h"
#include <rainman/core/Exception.h>
#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

// Helper: waits for a condition with timeout, returns true if condition was met.
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

// Helper: synchronization for receiving async results in tests.
template <typename T> struct AsyncResult
{
    std::mutex mtx;
    std::condition_variable cv;
    bool ready = false;
    Result<T> result = Result<T>::Err(wxT("not set"));

    void Set(Result<T> r)
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

TEST(CTaskRunnerTest, SuccessfulTaskDeliversOkResult)
{
    CTaskRunner runner;
    AsyncResult<int> ar;

    runner.Run<int>(
        [](CProgressChannel &, CCancellationToken &) -> int { return 42; },
        [](const std::string &) {},
        [&ar](Result<int> result) { ar.Set(std::move(result)); });

    ASSERT_TRUE(ar.Wait());
    EXPECT_TRUE(ar.result.ok());
    EXPECT_EQ(ar.result.value(), 42);
}

TEST(CTaskRunnerTest, VoidTaskDeliversOkResult)
{
    CTaskRunner runner;
    AsyncResult<void> ar;

    runner.Run<void>(
        [](CProgressChannel &, CCancellationToken &) { /* nothing */ },
        [](const std::string &) {},
        [&ar](Result<void> result) { ar.Set(std::move(result)); });

    ASSERT_TRUE(ar.Wait());
    EXPECT_TRUE(ar.result.ok());
}

TEST(CTaskRunnerTest, RainmanExceptionDeliversErrResult)
{
    CTaskRunner runner;
    AsyncResult<int> ar;

    runner.Run<int>(
        [](CProgressChannel &, CCancellationToken &) -> int {
            throw CRainmanException(__FILE__, __LINE__, "test rainman error");
        },
        [](const std::string &) {},
        [&ar](Result<int> result) { ar.Set(std::move(result)); });

    ASSERT_TRUE(ar.Wait());
    EXPECT_FALSE(ar.result.ok());
    EXPECT_TRUE(ar.result.error().Contains(wxT("test rainman error")));
}

TEST(CTaskRunnerTest, StdExceptionDeliversErrResult)
{
    CTaskRunner runner;
    AsyncResult<int> ar;

    runner.Run<int>(
        [](CProgressChannel &, CCancellationToken &) -> int {
            throw std::runtime_error("std error");
        },
        [](const std::string &) {},
        [&ar](Result<int> result) { ar.Set(std::move(result)); });

    ASSERT_TRUE(ar.Wait());
    EXPECT_FALSE(ar.result.ok());
    EXPECT_TRUE(ar.result.error().Contains(wxT("std error")));
}

TEST(CTaskRunnerTest, ChainedRainmanExceptionFormatsFullChain)
{
    CTaskRunner runner;
    AsyncResult<int> ar;

    runner.Run<int>(
        [](CProgressChannel &, CCancellationToken &) -> int {
            auto *pInner =
                new CRainmanException(__FILE__, __LINE__, "inner cause");
            throw CRainmanException(pInner, __FILE__, __LINE__,
                                        "outer error");
        },
        [](const std::string &) {},
        [&ar](Result<int> result) { ar.Set(std::move(result)); });

    ASSERT_TRUE(ar.Wait());
    EXPECT_FALSE(ar.result.ok());
    EXPECT_TRUE(ar.result.error().Contains(wxT("outer error")));
    EXPECT_TRUE(ar.result.error().Contains(wxT("inner cause")));
}

TEST(CTaskRunnerTest, ProgressMessagesDelivered)
{
    CTaskRunner runner;
    AsyncResult<int> ar;
    std::mutex mtx;
    std::vector<std::string> messages;

    runner.Run<int>(
        [](CProgressChannel &progress, CCancellationToken &) -> int {
            progress.Report("step 1");
            progress.Report("step 2");
            return 0;
        },
        [&](const std::string &msg) {
            std::lock_guard<std::mutex> lock(mtx);
            messages.push_back(msg);
        },
        [&ar](Result<int> result) { ar.Set(std::move(result)); });

    ASSERT_TRUE(ar.Wait());
    std::lock_guard<std::mutex> lock(mtx);
    ASSERT_EQ(messages.size(), 2u);
    EXPECT_EQ(messages[0], "step 1");
    EXPECT_EQ(messages[1], "step 2");
}

TEST(CTaskRunnerTest, CancellationTokenIsRespected)
{
    CTaskRunner runner;
    AsyncResult<int> ar;
    std::atomic<bool> taskStarted{false};

    runner.Run<int>(
        [&taskStarted](CProgressChannel &, CCancellationToken &cancel) -> int {
            taskStarted.store(true);
            // Spin until cancelled
            while (!cancel.IsCancelled())
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            return -1;
        },
        [](const std::string &) {},
        [&ar](Result<int> result) { ar.Set(std::move(result)); });

    ASSERT_TRUE(WaitFor([&]() { return taskStarted.load(); }));
    runner.Cancel();

    ASSERT_TRUE(ar.Wait());
    EXPECT_TRUE(ar.result.ok());
    EXPECT_EQ(ar.result.value(), -1);
}

TEST(CTaskRunnerTest, IsRunningReflectsState)
{
    CTaskRunner runner;
    EXPECT_FALSE(runner.IsRunning());

    AsyncResult<int> ar;
    std::atomic<bool> proceed{false};

    runner.Run<int>(
        [&proceed](CProgressChannel &, CCancellationToken &) -> int {
            while (!proceed.load())
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            return 0;
        },
        [](const std::string &) {},
        [&ar](Result<int> result) { ar.Set(std::move(result)); });

    EXPECT_TRUE(runner.IsRunning());

    proceed.store(true);
    ASSERT_TRUE(ar.Wait());
    ASSERT_TRUE(WaitFor([&]() { return !runner.IsRunning(); }));
    EXPECT_FALSE(runner.IsRunning());
}

TEST(CTaskRunnerTest, RunWhileRunningReturnsFalse)
{
    CTaskRunner runner;
    AsyncResult<int> ar;
    std::atomic<bool> proceed{false};

    runner.Run<int>(
        [&proceed](CProgressChannel &, CCancellationToken &) -> int {
            while (!proceed.load())
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            return 0;
        },
        [](const std::string &) {},
        [&ar](Result<int> result) { ar.Set(std::move(result)); });

    // Second Run() should fail while first is still running
    bool launched = runner.Run<int>(
        [](CProgressChannel &, CCancellationToken &) -> int { return 99; },
        [](const std::string &) {}, [](Result<int>) {});
    EXPECT_FALSE(launched);

    proceed.store(true);
    ASSERT_TRUE(ar.Wait());
}

TEST(CTaskRunnerTest, SequentialRunsWork)
{
    CTaskRunner runner;

    // First run
    {
        AsyncResult<int> ar;
        runner.Run<int>(
            [](CProgressChannel &, CCancellationToken &) -> int { return 1; },
            [](const std::string &) {},
            [&ar](Result<int> result) { ar.Set(std::move(result)); });
        ASSERT_TRUE(ar.Wait());
        EXPECT_EQ(ar.result.value(), 1);
    }

    // Wait for IsRunning to go false
    ASSERT_TRUE(WaitFor([&]() { return !runner.IsRunning(); }));

    // Second run on same instance
    {
        AsyncResult<int> ar;
        runner.Run<int>(
            [](CProgressChannel &, CCancellationToken &) -> int { return 2; },
            [](const std::string &) {},
            [&ar](Result<int> result) { ar.Set(std::move(result)); });
        ASSERT_TRUE(ar.Wait());
        EXPECT_EQ(ar.result.value(), 2);
    }
}

TEST(CTaskRunnerTest, DestructorJoinsRunningTask)
{
    std::atomic<bool> taskFinished{false};

    {
        CTaskRunner runner;
        runner.Run<void>(
            [&taskFinished](CProgressChannel &, CCancellationToken &cancel) {
                // Simulate work, check cancellation
                for (int i = 0; i < 50 && !cancel.IsCancelled(); ++i)
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                taskFinished.store(true);
            },
            [](const std::string &) {}, [](Result<void>) {});
        // Destructor cancels and joins
    }

    EXPECT_TRUE(taskFinished.load());
}

// ---------------------------------------------------------------------------
// CWxTaskRunner alive-flag tests
// ---------------------------------------------------------------------------

#include "async/CWxTaskRunner.h"
#include <wx/event.h>

TEST(CWxTaskRunnerTest, DestroyIdleRunnerDoesNotCrash)
{
    wxEvtHandler handler;
    { CWxTaskRunner runner(&handler); }
    // No crash = success
}

TEST(CWxTaskRunnerTest, DestroyRunnerWhileTaskRunningDoesNotCrash)
{
    wxEvtHandler handler;
    std::atomic<bool> taskFinished{false};

    {
        CWxTaskRunner runner(&handler);
        runner.RunAsync<void>(
            [&taskFinished](CProgressChannel &progress, CCancellationToken &cancel) {
                for (int i = 0; i < 20 && !cancel.IsCancelled(); ++i)
                {
                    progress.Report("step");
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
                taskFinished.store(true);
            },
            [](const std::string &) {}, [](Result<void>) {});
        // Destructor should cancel, join, and mark alive=false.
        // Any stale CallAfter lambdas in the handler's queue will
        // see the dead flag and skip — no use-after-free.
    }

    EXPECT_TRUE(taskFinished.load());
}

TEST(CWxTaskRunnerTest, SequentialRunsAfterDestructionWork)
{
    wxEvtHandler handler;

    // First runner — destroyed while task may still be posting CallAfter
    {
        CWxTaskRunner runner(&handler);
        runner.RunAsync<int>(
            [](CProgressChannel &progress, CCancellationToken &) -> int {
                progress.Report("hello");
                return 42;
            },
            [](const std::string &) {}, [](Result<int>) {});
    }

    // Second runner on the same handler — must work cleanly
    {
        CWxTaskRunner runner(&handler);
        std::atomic<bool> done{false};
        runner.RunAsync<void>(
            [&done](CProgressChannel &, CCancellationToken &) { done.store(true); },
            [](const std::string &) {}, [](Result<void>) {});

        ASSERT_TRUE(WaitFor([&]() { return done.load(); }));
    }
}
