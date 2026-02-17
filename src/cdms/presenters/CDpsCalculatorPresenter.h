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
#include "views/interfaces/IDpsCalculatorView.h"
#include <rainman/io/IDirectoryTraverser.h>
#include <wx/string.h>

//! Presenter for asynchronous DPS calculation.
/*!
    Owns a CWxTaskRunner and coordinates background AutoDPS processing.
    The four AutoDPS steps (Scan, Analyse, AddUnitList, OutputHTML) run on
    a background thread; IDpsCalculatorView callbacks are invoked on the
    main thread.

    The presenter takes ownership of the weapon iterator (pWeaponItr) and
    deletes it after the operation completes. The module pointer is accessed
    read-only on the background thread; the caller must ensure the module
    is not modified or deleted during calculation.
*/
class CDpsCalculatorPresenter
{
  public:
    CDpsCalculatorPresenter(IDpsCalculatorView &view, wxEvtHandler *pHandler);

    //! Start DPS calculation asynchronously.
    /*!
        \param sOutputFile  Path for the output .html file.
        \param pWeaponItr   Iterator over weapon files (ownership transferred to presenter).
        \param pModule      Module for unit list lookup (not owned, must outlive operation).
        \return true if the operation was started, false if one is already running.
    */
    bool CalculateDps(const wxString &sOutputFile, IDirectoryTraverser::IIterator *pWeaponItr,
                      IDirectoryTraverser *pModule);

    //! Cancel the currently running DPS calculation.
    void Cancel();

    //! Check whether DPS calculation is in progress.
    bool IsRunning() const;

  private:
    IDpsCalculatorView &m_view;
    CWxTaskRunner m_taskRunner;
};
