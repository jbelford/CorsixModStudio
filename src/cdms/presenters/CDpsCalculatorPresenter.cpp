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

#include "CDpsCalculatorPresenter.h"
#include "Tool_AutoDPS.h"
#include "strconv.h"

CDpsCalculatorPresenter::CDpsCalculatorPresenter(IDpsCalculatorView &view, wxEvtHandler *pHandler)
    : m_view(view), m_taskRunner(pHandler)
{
}

bool CDpsCalculatorPresenter::CalculateDps(const wxString &sOutputFile, IDirectoryTraverser::IIterator *pWeaponItr,
                                           IDirectoryTraverser *pModule)
{
    if (m_taskRunner.IsRunning())
        return false;

    m_view.DisableControls();
    m_view.ShowProgress(wxT("Calculating DPS..."));

    return m_taskRunner.RunAsync<void>(
        // Background thread
        [sOutputFile, pWeaponItr, pModule](CProgressChannel & /*progress*/, CCancellationToken & /*cancel*/)
        {
            AutoDPS_Internal::tAutoDPS_Package *pPack = nullptr;

            try
            {
                pPack = AutoDPS::AutoDPS_Scan(pWeaponItr);
                AutoDPS::AutoDPS_Analyse(pPack);
                AutoDPS::AutoDPS_AddUnitList(pPack, pModule);

                auto sOut = wxStringToAscii(sOutputFile);
                AutoDPS::AutoDPS_OutputHTML(pPack, sOut.get());
            }
            catch (...)
            {
                delete pPack;
                delete pWeaponItr;
                throw;
            }

            delete pPack;
            delete pWeaponItr;
        },
        // Progress (main thread)
        [this](const std::string &sMsg) { m_view.ShowProgress(wxString::FromUTF8(sMsg)); },
        // Done (main thread)
        [this](Result<void> result)
        {
            m_view.HideProgress();
            m_view.EnableControls();

            if (!result.ok())
            {
                m_view.ShowError(result.error());
                return;
            }

            m_view.OnDpsComplete();
        });
}

void CDpsCalculatorPresenter::Cancel() { m_taskRunner.Cancel(); }

bool CDpsCalculatorPresenter::IsRunning() const { return m_taskRunner.IsRunning(); }
