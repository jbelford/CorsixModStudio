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
#include "views/IRgdMacroView.h"
#include <rainman/formats/CRgdFileMacro.h>
#include <string>
#include <vector>
#include <wx/string.h>

class CModuleFile;
class CRgdHashTable;
class IFileStore;

//! Presenter for asynchronous RGD macro execution.
/*!
    Owns a CWxTaskRunner and coordinates background macro execution via
    CRgdFileMacro. All heavy I/O runs on a background thread; IRgdMacroView
    callbacks are invoked on the main thread.

    Permission dialogs (security, load, save) are marshaled to the main thread
    via CallAfter + std::promise/future, blocking the background thread until
    the user responds.
*/
class CRgdMacroPresenter
{
  public:
    CRgdMacroPresenter(IRgdMacroView &view, wxEvtHandler *pHandler);

    //! Start macro execution asynchronously.
    /*!
        \param sMacroText  The Lua macro source code.
        \param vFiles      List of RGD file paths to process.
        \param pFileStore  File store for loading RGD files.
        \param pHashTable  Hash table for RGD key resolution.
        \param pModule     Module for UCS resolution (may be null).
        \param bIsDowMod   True if targeting Dawn of War (vs. Company of Heroes).
        \param sPath       Base path of the macro folder (for sandbox checks).
        \param iPathLen    Length of the base path (including trailing separator).
        \param iPathHash   CRC32 hash of the base path (for sandbox checks).
        \return true if the operation was started, false if one is already running.
    */
    bool RunMacro(const wxString &sMacroText, std::vector<std::string> vFiles, IFileStore *pFileStore,
                  CRgdHashTable *pHashTable, CModuleFile *pModule, bool bIsDowMod, unsigned long iPathLen,
                  unsigned long iPathHash);

    //! Cancel the currently running macro execution.
    void Cancel();

    //! Check whether macro execution is in progress.
    bool IsRunning() const;

  private:
    IRgdMacroView &m_view;
    wxEvtHandler *m_pHandler;
    CWxTaskRunner m_taskRunner;
};
