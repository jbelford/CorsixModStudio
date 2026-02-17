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

#include <wx/string.h>

class CModuleFile;

//! Abstract interface for the main frame's loading-related UI operations.
/*!
    Decouples the CModuleLoadPresenter from ConstructFrame so that presenters
    can be tested with a mock view. Phase H defines only the module-loading
    subset; additional methods will be added in later phases.
*/
class IMainFrameView
{
  public:
    virtual ~IMainFrameView() = default;

    //! Show the loading dialog with an initial message.
    virtual void ShowLoadingDialog(const wxString &sMessage) = 0;

    //! Close and destroy the loading dialog.
    virtual void HideLoadingDialog() = 0;

    //! Update the progress text in the loading dialog.
    virtual void UpdateLoadingProgress(const wxString &sMessage) = 0;

    //! Display an error message box.
    virtual void ShowError(const wxString &sMessage) = 0;

    //! Called when a module has been loaded successfully on the background thread.
    /*!
        \param pMod   Ownership is transferred to the view. The view (ConstructFrame)
                      passes it to SetModule() for GUI integration.
        \param sPath  Path to the .module or .sga file that was loaded.
        \param bIsSga true if the module was loaded as an SGA faux-module.
    */
    virtual void OnModuleLoaded(CModuleFile *pMod, const wxString &sPath, bool bIsSga) = 0;

    //! Disable File > Open / New menu items during a load operation.
    virtual void DisableLoadMenuItems() = 0;

    //! Re-enable File > Open / New menu items after a load operation completes.
    virtual void EnableLoadMenuItems() = 0;
};
