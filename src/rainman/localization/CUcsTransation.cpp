/*
Rainman Library
Copyright (C) 2006 Corsix <corsix@gmail.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "rainman/localization/CUcsTransaction.h"
#include "rainman/core/memdebug.h"
#include "rainman/core/Exception.h"

CUcsTransaction::CUcsTransaction(std::shared_ptr<CUcsFile> pUcsObject) { m_pRawFile = std::move(pUcsObject); }

void CUcsTransaction::Save(const char *sFile)
{
    for (auto &itr : m_map.GetSortedMap())
    {
        m_pRawFile->ReplaceString(itr.first, std::move(itr.second));
    }
    m_map.Clear();

    try
    {
        m_pRawFile->Save(sFile);
    }
    CATCH_THROW("Raw object failed to write")
}

const CUcsMap &CUcsTransaction::GetRawMap()
{
    if (m_map.Empty())
        return m_pRawFile->GetRawMap();

    // Need to make a faux map of a combination of our edits and the original
    m_mapCombinationValues.Clear();
    m_mapCombinationValues = m_pRawFile->GetRawMap();

    for (auto &itr : m_map.GetSortedMap())
    {
        m_mapCombinationValues.Add(itr.first, itr.second);
    }

    return m_mapCombinationValues;
}

void CUcsTransaction::Load(IFileStore::IStream *pStream)
{
    m_map.Clear();

    try
    {
        m_pRawFile->Load(pStream);
    }
    CATCH_THROW("Raw object failed to load")
}

const wchar_t *CUcsTransaction::ResolveStringID(unsigned long iID)
{
    const std::shared_ptr<wchar_t[]> &pS = m_map[iID];
    if (!pS)
        return m_pRawFile->ResolveStringID(iID);
    return pS.get();
}

CUcsFile *CUcsTransaction::GetRawObject() { return m_pRawFile.get(); }
