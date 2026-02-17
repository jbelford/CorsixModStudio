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

#include "async/CWxTaskRunner.h"
#include "views/interfaces/IMassExtractView.h"
#include <string>
#include <vector>
#include <wx/string.h>

//! Presenter for asynchronous mass file extraction.
/*!
    Pre-collected file paths are extracted on a background thread with
    gauge progress updates posted to the main thread via CallAfter.
*/
class CMassExtractPresenter
{
  public:
    CMassExtractPresenter(IMassExtractView &view, wxEvtHandler *pHandler);

    //! Start extraction asynchronously.
    /*!
        \param vFiles       File paths to extract (pre-collected from tree).
        \param iCountDiv    Divisor for gauge scaling.
        \return true if started, false if already running.
    */
    bool Extract(std::vector<std::string> vFiles, size_t iCountDiv);

    void Cancel();
    bool IsRunning() const;

  private:
    IMassExtractView &m_view;
    CWxTaskRunner m_taskRunner;
};
