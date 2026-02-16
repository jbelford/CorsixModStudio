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

//! Abstract interface for the UCS→DAT conversion dialog's UI operations.
/*!
    Decouples CUcsToDatPresenter from frmUCSToDAT so that the presenter
    can be tested with a mock view.
*/
class IUcsToDatView
{
  public:
    virtual ~IUcsToDatView() = default;

    //! Show the loading/progress indicator with a message.
    virtual void ShowProgress(const wxString &sMessage) = 0;

    //! Hide the loading/progress indicator.
    virtual void HideProgress() = 0;

    //! Display an error message to the user.
    virtual void ShowError(const wxString &sMessage) = 0;

    //! Called when conversion completed successfully.
    virtual void OnConversionComplete() = 0;

    //! Disable UI controls during the async operation.
    virtual void DisableControls() = 0;

    //! Re-enable UI controls after the async operation completes.
    virtual void EnableControls() = 0;
};
