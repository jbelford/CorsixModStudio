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
#include "views/ILuaInheritTreeView.h"
#include <wx/string.h>

class CInheritTable;

//! Presenter for asynchronous Lua inheritance tree generation.
/*!
    Owns a CWxTaskRunner and coordinates background tree building via
    CMakeLuaInheritTree::Do(). The heavy I/O runs on a background thread;
    ILuaInheritTreeView callbacks are invoked on the main thread so that
    tree population (_AddChildren) stays on the UI thread.
*/
class CLuaInheritTreePresenter
{
  public:
    CLuaInheritTreePresenter(ILuaInheritTreeView &view, wxEvtHandler *pHandler);

    //! Start tree generation asynchronously.
    /*!
        \param pTable       Inherit table to populate (not owned, must outlive operation).
        \param sAttribPath  Attrib path prefix (e.g. "Generic\\Attrib\\").
        \return true if the operation was started, false if one is already running.
    */
    bool BuildTree(CInheritTable *pTable, const wxString &sAttribPath);

    //! Cancel the currently running tree generation.
    void Cancel();

    //! Check whether tree generation is in progress.
    bool IsRunning() const;

  private:
    ILuaInheritTreeView &m_view;
    CWxTaskRunner m_taskRunner;
};
