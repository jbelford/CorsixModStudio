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

#pragma once

#include <memory>
#include <map>
#include <unordered_map>
#include "rainman/core/Api.h"

//! Dual-map container for UCS string entries.
/*!
    Stores UCS entries in both an unordered_map (O(1) lookup) and a sorted
    std::map (ordered iteration for UI display and file serialization).
    String values are shared_ptr<wchar_t[]> to allow cheap sharing between
    CUcsTransaction combination maps and the underlying CUcsFile.
*/
class RAINMAN_API CUcsMap
{
  public:
    using SortedUcsMap = std::map<unsigned long, std::shared_ptr<wchar_t[]>>;
    using iterator = SortedUcsMap::iterator;
    using const_iterator = SortedUcsMap::const_iterator;

    CUcsMap() = default;
    ~CUcsMap() = default;

    //! Lookup by ID. Returns nullptr if key does not exist (does not insert).
    const std::shared_ptr<wchar_t[]> &operator[](unsigned long iID) const
    {
        static const std::shared_ptr<wchar_t[]> s_null;
        auto it = m_mapValues.find(iID);
        if (it == m_mapValues.end())
            return s_null;
        return it->second;
    }

    void Add(unsigned long iID, std::unique_ptr<wchar_t[]> pString)
    {
        if (pString == nullptr)
        {
            Remove(iID);
            return;
        }
        m_mapValues[iID] = std::shared_ptr<wchar_t[]>(std::move(pString));
        m_bSortedDirty = true;
    }

    void Add(unsigned long iID, std::shared_ptr<wchar_t[]> pString)
    {
        if (pString == nullptr)
        {
            Remove(iID);
            return;
        }
        m_mapValues[iID] = std::move(pString);
        m_bSortedDirty = true;
    }

    void Remove(unsigned long iID)
    {
        m_mapValues.erase(iID);
        m_bSortedDirty = true;
    }

    bool Empty() const { return m_mapValues.empty(); }
    size_t Size() const { return m_mapValues.size(); }

    SortedUcsMap &GetSortedMap()
    {
        EnsureSorted();
        return m_sortedMapValues;
    }
    const SortedUcsMap &GetSortedMap() const
    {
        EnsureSorted();
        return m_sortedMapValues;
    }

    iterator begin()
    {
        EnsureSorted();
        return m_sortedMapValues.begin();
    }
    iterator end()
    {
        EnsureSorted();
        return m_sortedMapValues.end();
    }
    const_iterator begin() const
    {
        EnsureSorted();
        return m_sortedMapValues.begin();
    }
    const_iterator end() const
    {
        EnsureSorted();
        return m_sortedMapValues.end();
    }

    iterator Find(unsigned long iID)
    {
        EnsureSorted();
        return m_sortedMapValues.find(iID);
    }
    const_iterator Find(unsigned long iID) const
    {
        EnsureSorted();
        return m_sortedMapValues.find(iID);
    }

    void Clear()
    {
        m_sortedMapValues.clear();
        m_mapValues.clear();
        m_bSortedDirty = false;
    }

  private:
    void EnsureSorted() const
    {
        if (!m_bSortedDirty)
            return;
        m_sortedMapValues.clear();
        m_sortedMapValues.insert(m_mapValues.begin(), m_mapValues.end());
        m_bSortedDirty = false;
    }

    std::unordered_map<unsigned long, std::shared_ptr<wchar_t[]>> m_mapValues;
    mutable SortedUcsMap m_sortedMapValues;
    mutable bool m_bSortedDirty = false;
};
