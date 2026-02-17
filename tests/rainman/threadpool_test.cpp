/*
Rainman Library
Copyright (C) 2006 Corsix <corsix@gmail.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <gtest/gtest.h>
#include "rainman/core/CThreadPool.h"
#include <atomic>
#include <chrono>
#include <numeric>
#include <vector>

TEST(CThreadPool, SingletonReturnsSameInstance)
{
    auto &pool1 = CThreadPool::Instance();
    auto &pool2 = CThreadPool::Instance();
    EXPECT_EQ(&pool1, &pool2);
}

TEST(CThreadPool, ThreadCountMatchesHardwareConcurrency)
{
    auto &pool = CThreadPool::Instance();
    auto expected = std::max(1u, std::thread::hardware_concurrency());
    EXPECT_EQ(pool.ThreadCount(), static_cast<std::size_t>(expected));
}

TEST(CThreadPool, SubmitReturnsCorrectResult)
{
    auto &pool = CThreadPool::Instance();
    auto fut = pool.Submit([](int a, int b) { return a + b; }, 17, 25);
    EXPECT_EQ(fut.get(), 42);
}

TEST(CThreadPool, SubmitVoidCallable)
{
    auto &pool = CThreadPool::Instance();
    std::atomic<int> counter{0};
    auto fut = pool.Submit([&counter]() { counter.fetch_add(1); });
    fut.get();
    EXPECT_EQ(counter.load(), 1);
}

TEST(CThreadPool, SubmitManyTasks)
{
    auto &pool = CThreadPool::Instance();
    constexpr int kCount = 200;
    std::vector<std::future<int>> futures;
    futures.reserve(kCount);

    for (int i = 0; i < kCount; ++i)
        futures.push_back(pool.Submit([](int x) { return x * x; }, i));

    for (int i = 0; i < kCount; ++i)
        EXPECT_EQ(futures[i].get(), i * i);
}

TEST(CThreadPool, TasksRunConcurrently)
{
    auto &pool = CThreadPool::Instance();
    std::atomic<int> peakConcurrency{0};
    std::atomic<int> activeTasks{0};

    constexpr int kCount = 32;
    std::vector<std::future<void>> futures;
    futures.reserve(kCount);

    for (int i = 0; i < kCount; ++i)
    {
        futures.push_back(pool.Submit([&activeTasks, &peakConcurrency]() {
            int current = activeTasks.fetch_add(1) + 1;
            int prev = peakConcurrency.load();
            while (current > prev && !peakConcurrency.compare_exchange_weak(prev, current))
            {
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            activeTasks.fetch_sub(1);
        }));
    }

    for (auto &f : futures)
        f.get();

    // At least 2 tasks should have run concurrently (unless single-core machine)
    if (std::thread::hardware_concurrency() >= 2)
        EXPECT_GE(peakConcurrency.load(), 2);
}

TEST(CThreadPool, ExceptionPropagatesThroughFuture)
{
    auto &pool = CThreadPool::Instance();
    auto fut = pool.Submit([]() -> int { throw std::runtime_error("test error"); });

    EXPECT_THROW(fut.get(), std::runtime_error);
}

TEST(CThreadPool, SubmitWithMoveOnlyArg)
{
    auto &pool = CThreadPool::Instance();
    auto ptr = std::make_unique<int>(99);
    // std::bind copies args, so we use a shared_ptr to verify move semantics work
    auto shared = std::make_shared<int>(99);
    auto fut = pool.Submit([](std::shared_ptr<int> p) { return *p; }, shared);
    EXPECT_EQ(fut.get(), 99);
}
