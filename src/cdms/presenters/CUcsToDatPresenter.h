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
#include "views/IUcsToDatView.h"
#include <wx/string.h>

class CModuleFile;

//! Presenter for asynchronous UCS→DAT conversion.
/*!
    Owns a CWxTaskRunner and coordinates background UCS→DAT conversion via
    UCSToDATConvertor. The conversion runs on a background thread; IUcsToDatView
    callbacks are invoked on the main thread.

    The module pointer is accessed read-only on the background thread.
    The caller must ensure the module is not modified or deleted during conversion.
*/
class CUcsToDatPresenter
{
  public:
    CUcsToDatPresenter(IUcsToDatView &view, wxEvtHandler *pHandler);

    //! Start UCS→DAT conversion asynchronously.
    /*!
        \param sOutputFile  Path for the output .dat file.
        \param iRangeStart  UCS range start (e.g. 90000).
        \param iRangeEnd    UCS range end (e.g. 199999).
        \param pModule      Module to extract UCS data from (not owned, must outlive operation).
        \return true if the operation was started, false if one is already running.
    */
    bool Convert(const wxString &sOutputFile, unsigned long iRangeStart, unsigned long iRangeEnd, CModuleFile *pModule);

    //! Cancel the currently running conversion.
    void Cancel();

    //! Check whether conversion is in progress.
    bool IsRunning() const;

  private:
    IUcsToDatView &m_view;
    CWxTaskRunner m_taskRunner;
};
