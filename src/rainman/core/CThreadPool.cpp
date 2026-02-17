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

#include "rainman/core/CThreadPool.h"
#include "rainman/core/RainmanLog.h"
#include <algorithm>

CThreadPool &CThreadPool::Instance()
{
    static CThreadPool sInstance((std::max)(static_cast<unsigned int>(1), std::thread::hardware_concurrency()));
    return sInstance;
}

CThreadPool::CThreadPool(std::size_t iThreadCount)
{
    RAINMAN_LOG_INFO("CThreadPool: creating {} worker threads", iThreadCount);
    m_vWorkers.reserve(iThreadCount);
    for (std::size_t i = 0; i < iThreadCount; ++i)
        m_vWorkers.emplace_back(&CThreadPool::WorkerLoop, this);
}

CThreadPool::~CThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(m_mtxQueue);
        m_bStopping = true;
    }
    m_cvWork.notify_all();
    for (auto &worker : m_vWorkers)
    {
        if (worker.joinable())
            worker.join();
    }
}

void CThreadPool::WorkerLoop()
{
    for (;;)
    {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(m_mtxQueue);
            m_cvWork.wait(lock, [this] { return m_bStopping || !m_qTasks.empty(); });

            if (m_bStopping && m_qTasks.empty())
                return;

            task = std::move(m_qTasks.front());
            m_qTasks.pop();
        }
        task();
    }
}
