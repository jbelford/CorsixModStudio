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

#include "CMassExtractPresenter.h"
#include "actions/CExtractAction.h"
#include <rainman/core/Exception.h>

CMassExtractPresenter::CMassExtractPresenter(IMassExtractView &view, wxEvtHandler *pHandler)
    : m_view(view), m_taskRunner(pHandler)
{
}

bool CMassExtractPresenter::Extract(std::vector<std::string> vFiles, size_t iCountDiv)
{
    if (m_taskRunner.IsRunning())
        return false;

    m_view.DisableControls();

    return m_taskRunner.RunAsync<void>(
        // Background thread
        [vFiles = std::move(vFiles), iCountDiv](CProgressChannel &progress, CCancellationToken &cancel)
        {
            auto p4mbBuffer = std::make_unique<char[]>(4194304);
            int iGVal = 0;

            for (size_t i = 0; i < vFiles.size(); ++i)
            {
                if (cancel.IsCancelled())
                    return;

                std::vector<char> saFile(vFiles[i].begin(), vFiles[i].end());
                saFile.push_back('\0');

                try
                {
                    CExtractAction::DoExtract(saFile.data(), p4mbBuffer.get());
                }
                catch (const CRainmanException &e)
                {
                }

                int iNGVal = static_cast<int>((i + 1) / iCountDiv);
                if (iNGVal != iGVal)
                {
                    iGVal = iNGVal;
                    progress.Report(std::to_string(iGVal));
                }
            }
        },
        // Progress (main thread) — gauge updates
        [this](const std::string &sMsg)
        {
            try
            {
                int iVal = std::stoi(sMsg);
                m_view.UpdateGauge(iVal);
            }
            catch (...)
            {
            }
        },
        // Done (main thread)
        [this](Result<void> result)
        {
            m_view.EnableControls();

            if (!result.ok())
            {
                m_view.ShowError(result.error());
                return;
            }

            m_view.OnExtractionComplete();
        });
}

void CMassExtractPresenter::Cancel() { m_taskRunner.Cancel(); }

bool CMassExtractPresenter::IsRunning() const { return m_taskRunner.IsRunning(); }
