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

#pragma once

#include <atomic>

//! Shared cancellation flag for cooperative task cancellation.
/*!
    Passed to background tasks so they can check for cancellation requests.
    Thread-safe: Cancel() can be called from any thread.
*/
class CCancellationToken
{
  public:
    //! Request cancellation of the associated task.
    void Cancel() { m_bCancelled.store(true, std::memory_order_release); }

    //! Check whether cancellation has been requested.
    bool IsCancelled() const { return m_bCancelled.load(std::memory_order_acquire); }

    //! Reset to non-cancelled state (for reuse).
    void Reset() { m_bCancelled.store(false, std::memory_order_release); }

  private:
    std::atomic<bool> m_bCancelled{false};
};
