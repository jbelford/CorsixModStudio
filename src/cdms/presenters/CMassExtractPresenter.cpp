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

void CMassExtractPresenter::CollectFilesFromIterator(IDirectoryTraverser::IIterator *pItr, const std::string &sPrefix,
                                                     const std::vector<SourceFilter> &vSources,
                                                     std::vector<std::string> &vFiles)
{
    if (!pItr || pItr->VGetType() == IDirectoryTraverser::IIterator::T_Nothing)
    {
        return;
    }

    do
    {
        switch (pItr->VGetType())
        {
        case IDirectoryTraverser::IIterator::T_File:
        {
            const auto *sMod = static_cast<const char *>(pItr->VGetTag(0));
            const auto *sSrc = static_cast<const char *>(pItr->VGetTag(1));
            for (const auto &src : vSources)
            {
                if ((src.sMod.empty() || (sMod && stricmp(sMod, src.sMod.c_str()) == 0)) &&
                    (sSrc && stricmp(sSrc, src.sSrc.c_str()) == 0))
                {
                    std::string sFile = sPrefix;
                    if (!sFile.empty())
                    {
                        sFile += '\\';
                    }
                    sFile += pItr->VGetName();
                    vFiles.push_back(std::move(sFile));
                    break;
                }
            }
            break;
        }
        case IDirectoryTraverser::IIterator::T_Directory:
        {
            std::string sNewPrefix = sPrefix;
            if (!sNewPrefix.empty())
            {
                sNewPrefix += '\\';
            }
            sNewPrefix += pItr->VGetName();

            IDirectoryTraverser::IIterator *pSubDir = pItr->VOpenSubDir();
            if (pSubDir)
            {
                CollectFilesFromIterator(pSubDir, sNewPrefix, vSources, vFiles);
                delete pSubDir;
            }
            break;
        }
        default:
            break;
        }
    } while (pItr->VNextItem() == IDirectoryTraverser::IIterator::E_OK);
}

bool CMassExtractPresenter::ExtractFromModule(CModuleFile *pModule, std::vector<SourceFilter> vSources,
                                              const std::string &sBasePath)
{
    if (m_taskRunner.IsRunning())
    {
        return false;
    }

    m_view.DisableControls();

    return m_taskRunner.RunAsync<void>(
        // Background thread: discover files then extract
        [pModule, vSources = std::move(vSources), sBasePath](CProgressChannel &progress, CCancellationToken &cancel)
        {
            // Phase 1: Discover files by walking the module's virtual filesystem
            std::vector<std::string> vFiles;

            if (sBasePath.empty())
            {
                // Root-level extraction: iterate all entry points
                unsigned long iEntryCount = pModule->VGetEntryPointCount();
                for (unsigned long i = 0; i < iEntryCount; ++i)
                {
                    if (cancel.IsCancelled())
                    {
                        return;
                    }
                    const char *sEntry = pModule->VGetEntryPoint(i);
                    IDirectoryTraverser::IIterator *pItr = pModule->VIterate(sEntry);
                    if (pItr)
                    {
                        CollectFilesFromIterator(pItr, sEntry, vSources, vFiles);
                        delete pItr;
                    }
                }
            }
            else
            {
                IDirectoryTraverser::IIterator *pItr = pModule->VIterate(sBasePath.c_str());
                if (pItr)
                {
                    CollectFilesFromIterator(pItr, sBasePath, vSources, vFiles);
                    delete pItr;
                }
            }

            if (vFiles.empty() || cancel.IsCancelled())
            {
                return;
            }

            // Phase 2: Set gauge range
            size_t iCountDiv = 1;
            while ((vFiles.size() / iCountDiv) > 0xFF)
            {
                ++iCountDiv;
            }

            progress.Report("range:" + std::to_string(vFiles.size() / iCountDiv));

            // Phase 3: Extract files
            auto p4mbBuffer = std::make_unique<char[]>(4194304);
            int iGVal = 0;

            for (size_t i = 0; i < vFiles.size(); ++i)
            {
                if (cancel.IsCancelled())
                {
                    return;
                }

                std::vector<char> saFile(vFiles[i].begin(), vFiles[i].end());
                saFile.push_back('\0');

                try
                {
                    CExtractAction::DoExtract(saFile.data(), p4mbBuffer.get());
                }
                catch (const CRainmanException &)
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
            if (sMsg.compare(0, 6, "range:") == 0)
            {
                try
                {
                    int iRange = std::stoi(sMsg.substr(6));
                    m_view.SetGaugeRange(iRange);
                }
                catch (...)
                {
                }
            }
            else
            {
                try
                {
                    int iVal = std::stoi(sMsg);
                    m_view.UpdateGauge(iVal);
                }
                catch (...)
                {
                }
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
