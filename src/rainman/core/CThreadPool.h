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

#ifndef _C_THREAD_POOL_H_
#define _C_THREAD_POOL_H_

#include "rainman/core/Api.h"
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

//! A fixed-size thread pool for parallel work submission.
/*!
    Provides a drop-in replacement for scattered std::async calls,
    enforcing a global concurrency limit equal to the hardware thread count.

    Usage:
    \code
        CThreadPool& pool = CThreadPool::Instance();
        auto fut = pool.Submit([](int x) { return x * 2; }, 21);
        int result = fut.get();  // 42
    \endcode

    The pool is a process-wide singleton created on first access and
    destroyed at program exit.  Workers block on a condition variable
    when the queue is empty, so an idle pool costs zero CPU.
*/
class RAINMAN_API CThreadPool
{
  public:
    //! Get the process-wide singleton.
    static CThreadPool &Instance();

    //! Submit a callable and its arguments; returns a future for the result.
    template <typename F, typename... Args>
    auto Submit(F &&f, Args &&...args) -> std::future<std::invoke_result_t<F, Args...>>
    {
        using R = std::invoke_result_t<F, Args...>;

        auto task =
            std::make_shared<std::packaged_task<R()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<R> result = task->get_future();

        {
            std::lock_guard<std::mutex> lock(m_mtxQueue);
            m_qTasks.emplace([task]() { (*task)(); });
        }
        m_cvWork.notify_one();
        return result;
    }

    //! Number of worker threads in the pool.
    [[nodiscard]] std::size_t ThreadCount() const noexcept { return m_vWorkers.size(); }

    // Non-copyable, non-movable
    CThreadPool(const CThreadPool &) = delete;
    CThreadPool &operator=(const CThreadPool &) = delete;
    CThreadPool(CThreadPool &&) = delete;
    CThreadPool &operator=(CThreadPool &&) = delete;

  private:
    //! Create a pool with the given number of worker threads.
    explicit CThreadPool(std::size_t iThreadCount);

    //! Destructor signals all workers to stop and joins them.
    ~CThreadPool();

    //! Worker thread main loop.
    void WorkerLoop();

    std::vector<std::thread> m_vWorkers;
    std::queue<std::function<void()>> m_qTasks;
    std::mutex m_mtxQueue;
    std::condition_variable m_cvWork;
    bool m_bStopping = false;
};

#endif // _C_THREAD_POOL_H_
