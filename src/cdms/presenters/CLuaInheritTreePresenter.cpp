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

#include "CLuaInheritTreePresenter.h"
#include "Tools.h"
#include "common/strconv.h"
#include <rainman/lua/CInheritTable.h>

CLuaInheritTreePresenter::CLuaInheritTreePresenter(ILuaInheritTreeView &view, wxEvtHandler *pHandler)
    : m_view(view), m_taskRunner(pHandler)
{
}

bool CLuaInheritTreePresenter::BuildTree(CInheritTable *pTable, const wxString &sAttribPath)
{
    if (m_taskRunner.IsRunning())
        return false;

    m_view.DisableControls();
    m_view.ShowLoading(wxT("Generating LUA inheritance tree"));

    return m_taskRunner.RunAsync<void>(
        // Background thread
        [pTable, sAttribPath](CProgressChannel & /*progress*/, CCancellationToken & /*cancel*/)
        {
            auto saPath = wxStringToAscii(sAttribPath);

            CMakeLuaInheritTree *pTool = new CMakeLuaInheritTree;
            pTool->pTable = pTable;
            try
            {
                pTool->Do(saPath.get());
            }
            catch (...)
            {
                delete pTool;
                throw;
            }
            delete pTool;
        },
        // Progress (main thread)
        [this](const std::string &sMsg) { m_view.ShowLoading(wxString::FromUTF8(sMsg)); },
        // Done (main thread)
        [this](Result<void> result)
        {
            m_view.HideLoading();
            m_view.EnableControls();

            if (!result.ok())
            {
                m_view.ShowError(result.error());
                return;
            }

            m_view.OnTreeDataReady();
        });
}

void CLuaInheritTreePresenter::Cancel() { m_taskRunner.Cancel(); }

bool CLuaInheritTreePresenter::IsRunning() const { return m_taskRunner.IsRunning(); }
