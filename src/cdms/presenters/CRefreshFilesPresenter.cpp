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

#include "CRefreshFilesPresenter.h"
#include "async/CProgressChannel.h"
#include <rainman/core/Exception.h>

CRefreshFilesPresenter::CRefreshFilesPresenter(IRefreshFilesView &view, wxEvtHandler *pHandler)
    : m_view(view), m_taskRunner(pHandler)
{
}

bool CRefreshFilesPresenter::Refresh(CModuleFile *pModule)
{
    if (m_taskRunner.IsRunning())
        return false;

    m_view.ShowLoading(wxT("Initializing"));

    return m_taskRunner.RunAsync<void>(
        // Background thread: reload resources
        [pModule](CProgressChannel &progress, CCancellationToken &cancel)
        {
            pModule->ReloadResources(CModuleFile::RR_All, CModuleFile::RR_All, CModuleFile::RR_All,
                                     CProgressChannel::RainmanCallback, &progress);
        },
        // Progress (main thread)
        [this](const std::string &sMsg) { m_view.UpdateProgress(wxString::FromUTF8(sMsg)); },
        // Done (main thread)
        [this](Result<void> result)
        {
            if (!result.ok())
            {
                m_view.HideLoading();
                m_view.ShowError(result.error());
                return;
            }

            m_view.UpdateProgress(wxT("Refreshing GUI"));
            m_view.RefreshFileTree();
            m_view.HideLoading();
        });
}

void CRefreshFilesPresenter::Cancel() { m_taskRunner.Cancel(); }

bool CRefreshFilesPresenter::IsRunning() const { return m_taskRunner.IsRunning(); }
