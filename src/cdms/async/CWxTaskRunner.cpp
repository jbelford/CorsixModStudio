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

#include "CWxTaskRunner.h"

CWxTaskRunner::CWxTaskRunner(wxEvtHandler *pHandler)
    : m_pHandler(pHandler), m_pAlive(std::make_shared<std::atomic<bool>>(true))
{
}

CWxTaskRunner::~CWxTaskRunner() { m_pAlive->store(false, std::memory_order_release); }

void CWxTaskRunner::Cancel() { m_runner.Cancel(); }

bool CWxTaskRunner::IsRunning() const { return m_runner.IsRunning(); }
