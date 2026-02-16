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

#ifndef _C_TASK_RUNNER_H_
#define _C_TASK_RUNNER_H_

#include "CancellationToken.h"
#include "CProgressChannel.h"
#include "services/Result.h"
#include <rainman/core/Exception.h>
#include <atomic>
#include <functional>
#include <memory>
#include <thread>
#include <type_traits>

//! Runs a task on a background thread and delivers the result via callback.
/*!
    Pure C++ implementation — no wxWidgets dependency. The onDone callback
    is invoked from the background thread; use CWxTaskRunner for main-thread
    delivery via CallAfter().

    One task per instance. Cancel the running task before starting a new one.
    The destructor joins any outstanding thread.
*/
class CTaskRunner
{
  public:
    ~CTaskRunner();

    CTaskRunner() = default;
    CTaskRunner(const CTaskRunner &) = delete;
    CTaskRunner &operator=(const CTaskRunner &) = delete;

    //! Run a task on a background thread.
    /*!
        \param task       Work function executed on the background thread.
        \param onProgress Called (from the background thread) with progress messages.
        \param onDone     Called (from the background thread) with the result.
        \return true if the task was launched, false if one is already running.
    */
    template <typename T>
    bool Run(std::function<T(CProgressChannel &, CCancellationToken &)> task,
             std::function<void(const std::string &)> onProgress, std::function<void(Result<T>)> onDone)
    {
        if (m_bRunning.load(std::memory_order_acquire))
            return false;

        // Join the previous thread if it finished but hasn't been joined yet
        if (m_thread.joinable())
            m_thread.join();

        m_bRunning.store(true, std::memory_order_release);
        m_pToken = std::make_shared<CCancellationToken>();

        auto pToken = m_pToken;
        auto *pRunning = &m_bRunning;

        m_thread = std::thread(
            [task = std::move(task), onProgress = std::move(onProgress), onDone = std::move(onDone), pToken,
             pRunning]() mutable
            {
                CProgressChannel progress(std::move(onProgress));

                Result<T> result = Result<T>::Err(wxT("Task did not complete"));

                try
                {
                    if constexpr (std::is_void_v<T>)
                    {
                        task(progress, *pToken);
                        result = Result<void>::Ok();
                    }
                    else
                    {
                        T value = task(progress, *pToken);
                        result = Result<T>::Ok(std::move(value));
                    }
                }
                catch (CRainmanException *pE)
                {
                    result = ResultFromExceptionT<T>(pE);
                }
                catch (const std::exception &e)
                {
                    result = Result<T>::Err(wxString::FromUTF8(e.what()));
                }

                pRunning->store(false, std::memory_order_release);
                onDone(std::move(result));
            });

        return true;
    }

    //! Request cancellation of the running task.
    void Cancel();

    //! Check whether a task is currently running.
    bool IsRunning() const;

  private:
    std::shared_ptr<CCancellationToken> m_pToken;
    std::thread m_thread;
    std::atomic<bool> m_bRunning{false};
};

#endif
