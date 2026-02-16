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
#include "views/IRefreshFilesView.h"
#include <rainman/module/CModuleFile.h>

//! Presenter for asynchronous resource reload and file tree refresh.
/*!
    Runs CModuleFile::ReloadResources() on a background thread with progress
    reporting via CProgressChannel. On completion, calls the view to refresh
    the file tree on the main thread.
*/
class CRefreshFilesPresenter
{
  public:
    CRefreshFilesPresenter(IRefreshFilesView &view, wxEvtHandler *pHandler);

    //! Start the refresh operation asynchronously.
    /*!
        \param pModule  The module whose resources should be reloaded (not owned).
        \return true if the operation was started, false if already running.
    */
    bool Refresh(CModuleFile *pModule);

    void Cancel();
    bool IsRunning() const;

  private:
    IRefreshFilesView &m_view;
    CWxTaskRunner m_taskRunner;
};
