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

#include "CSgaMakePresenter.h"
#include "async/CProgressChannel.h"
#include "services/FileService.h"
#include "strconv.h"
#include <rainman/archive/CSgaCreator.h>
#include <rainman/io/CFileSystemStore.h>

CSgaMakePresenter::CSgaMakePresenter(ISgaMakeView &view, wxEvtHandler *pHandler) : m_view(view), m_taskRunner(pHandler)
{
}

bool CSgaMakePresenter::CreateSga(const wxString &sInputDir, const wxString &sOutputFile, const wxString &sTocName,
                                  long iVersion)
{
    if (m_taskRunner.IsRunning())
        return false;

    m_view.DisableControls();
    m_view.ShowProgress(wxT("Creating SGA archive..."));

    return m_taskRunner.RunAsync<void>(
        // Background thread
        [sInputDir, sOutputFile, sTocName, iVersion](CProgressChannel & /*progress*/, CCancellationToken & /*cancel*/)
        {
            CFileSystemStore oFSO;
            oFSO.VInit();

            auto itrResult = FileService::IterateFileSystem(sInputDir);
            if (!itrResult)
                throw new CRainmanException(__FILE__, __LINE__, "Cannot iterate input directory");

            auto pItr = itrResult.value().release();

            auto saOutput = wxStringToAscii(sOutputFile);
            auto saToc = wxStringToAscii(sTocName);

            try
            {
                CSgaCreator::CreateSga(pItr, &oFSO, saToc.get(), saOutput.get(), iVersion);
            }
            catch (...)
            {
                delete pItr;
                throw;
            }

            delete pItr;
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

            m_view.OnSgaCreated();
        });
}

void CSgaMakePresenter::Cancel() { m_taskRunner.Cancel(); }

bool CSgaMakePresenter::IsRunning() const { return m_taskRunner.IsRunning(); }
