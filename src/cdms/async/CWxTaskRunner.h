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

#ifndef _C_WX_TASK_RUNNER_H_
#define _C_WX_TASK_RUNNER_H_

#include "CTaskRunner.h"
#include <wx/event.h>

//! wx-aware wrapper around CTaskRunner.
/*!
    Marshals onProgress and onDone callbacks to the main thread via
    wxEvtHandler::CallAfter(). All callbacks execute on the main thread,
    making it safe to update GUI controls from within them.
*/
class CWxTaskRunner
{
  public:
    explicit CWxTaskRunner(wxEvtHandler *pHandler);

    //! Run a task on a background thread with main-thread callbacks.
    /*!
        \param task       Work function executed on the background thread.
        \param onProgress Called on the MAIN thread with progress messages.
        \param onDone     Called on the MAIN thread with the result.
        \return true if the task was launched, false if one is already running.
    */
    template <typename T>
    bool RunAsync(std::function<T(CProgressChannel &, CCancellationToken &)> task,
                  std::function<void(const std::string &)> onProgress, std::function<void(Result<T>)> onDone)
    {
        auto *pHandler = m_pHandler;

        // Wrap onProgress to post to main thread
        auto wrappedProgress = [pHandler, onProgress = std::move(onProgress)](const std::string &sMsg)
        { pHandler->CallAfter([onProgress, sMsg]() { onProgress(sMsg); }); };

        // Wrap onDone to post to main thread
        auto wrappedDone = [pHandler, onDone = std::move(onDone)](Result<T> result)
        { pHandler->CallAfter([onDone, result = std::move(result)]() mutable { onDone(std::move(result)); }); };

        return m_runner.Run<T>(std::move(task), std::move(wrappedProgress), std::move(wrappedDone));
    }

    //! Request cancellation of the running task.
    void Cancel();

    //! Check whether a task is currently running.
    bool IsRunning() const;

  private:
    wxEvtHandler *m_pHandler;
    CTaskRunner m_runner;
};

#endif
