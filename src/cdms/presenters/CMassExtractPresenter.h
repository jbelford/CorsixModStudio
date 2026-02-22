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
#include "views/interfaces/IMassExtractView.h"
#include <rainman/module/CModuleFile.h>
#include <string>
#include <vector>
#include <wx/string.h>

//! Source filter for mass extraction — identifies which archive/folder to extract from.
struct SourceFilter
{
    std::string sMod; //!< Module name (empty = match any)
    std::string sSrc; //!< Source archive/folder name
};

//! Presenter for asynchronous mass file extraction.
/*!
    File discovery and extraction both run on a background thread with
    gauge progress updates posted to the main thread via CallAfter.
*/
class CMassExtractPresenter
{
  public:
    CMassExtractPresenter(IMassExtractView &view, wxEvtHandler *pHandler);

    //! Start extraction by discovering files from the module on a background thread.
    /*!
        Walks the module's virtual filesystem via IDirectoryTraverser to discover
        files matching the source filters, then extracts them — all off the main thread.
        \param pModule      The loaded module to iterate.
        \param vSources     Source filters (mod+archive pairs) for file selection.
        \param sBasePath    Starting path (empty = iterate all entry points).
        \return true if started, false if already running.
    */
    bool ExtractFromModule(CModuleFile *pModule, std::vector<SourceFilter> vSources, const std::string &sBasePath);

    void Cancel();
    bool IsRunning() const;

  private:
    IMassExtractView &m_view;
    CWxTaskRunner m_taskRunner;

    //! Recursively collect file paths from a directory iterator, filtering by source.
    static void CollectFilesFromIterator(IDirectoryTraverser::IIterator *pItr, const std::string &sPrefix,
                                         const std::vector<SourceFilter> &vSources, std::vector<std::string> &vFiles);
};
