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
#include "views/interfaces/ISgaMakeView.h"
#include <wx/string.h>

//! Presenter for asynchronous SGA archive creation.
/*!
    Owns a CWxTaskRunner and coordinates background SGA creation via
    CSgaCreator::CreateSga(). All heavy I/O and compression runs on a
    background thread; ISgaMakeView callbacks are invoked on the main thread.
*/
class CSgaMakePresenter
{
  public:
    CSgaMakePresenter(ISgaMakeView &view, wxEvtHandler *pHandler);

    //! Start SGA creation asynchronously.
    /*!
        \param sInputDir    Path to the directory to pack.
        \param sOutputFile  Path for the output .sga file.
        \param sTocName     Table of contents name (e.g. "Data", "Attrib").
        \param iVersion     SGA version (2 = DoW, 4 = CoH).
        \return true if the operation was started, false if one is already running.
    */
    bool CreateSga(const wxString &sInputDir, const wxString &sOutputFile, const wxString &sTocName, long iVersion);

    //! Cancel the currently running SGA creation.
    void Cancel();

    //! Check whether SGA creation is in progress.
    bool IsRunning() const;

  private:
    ISgaMakeView &m_view;
    CWxTaskRunner m_taskRunner;
};
