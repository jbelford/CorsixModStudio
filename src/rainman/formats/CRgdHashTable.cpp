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

#include "rainman/formats/CRgdHashTable.h"
#include "rainman/core/memdebug.h"
#include "rainman/core/Exception.h"
#include <cstdio>
#include <cstring>

extern "C"
{
    using ub4 = unsigned long int; /* unsigned 4-byte quantities */
    using ub1 = unsigned char;
    ub4 hash(ub1 *k, ub4 length, ub4 initval);
    ub4 hash3(ub1 *k, ub4 length, ub4 initval);
}

namespace
{

unsigned long ComputeHash(const char *sValue, size_t iLen)
{
    return hash(reinterpret_cast<ub1 *>(const_cast<char *>(sValue)), static_cast<ub4>(iLen), 0);
}

//! Try to parse "0xHHHHHHHH" (exactly iLen == 10) as a hex literal.
bool TryParseHexLiteral(const char *sValue, size_t iLen, unsigned long &iHash)
{
    if (iLen != 10 || sValue[0] != '0' || (sValue[1] != 'x' && sValue[1] != 'X'))
        return false;

    unsigned long iKey = 0;
    for (int i = 2; i < 10; ++i)
    {
        char c = sValue[i];
        iKey <<= 4;
        if (c >= '0' && c <= '9')
            iKey |= static_cast<unsigned long>(c - '0');
        else if (c >= 'a' && c <= 'f')
            iKey |= static_cast<unsigned long>(c - 'a' + 10);
        else if (c >= 'A' && c <= 'F')
            iKey |= static_cast<unsigned long>(c - 'A' + 10);
        else
            return false;
    }
    iHash = iKey;
    return true;
}

//! Read one line from a FILE*, stripping trailing CR/LF. Returns false at EOF.
bool ReadLine(FILE *f, std::string &line)
{
    line.clear();
    int ch = 0;
    while ((ch = fgetc(f)) != EOF)
    {
        if (ch == '\n')
            break;
        line += static_cast<char>(ch);
    }
    while (!line.empty() && (line.back() == '\r' || line.back() == '\n'))
        line.pop_back();
    return ch != EOF || !line.empty();
}

//! Strip whitespace characters from a string portion.
std::string StripWhitespace(const std::string &s, size_t pos)
{
    std::string result;
    result.reserve(s.size() - pos);
    for (size_t i = pos; i < s.size(); ++i)
    {
        char c = s[i];
        if (c != ' ' && c != '\t' && c != '\r' && c != '\n')
            result += c;
    }
    return result;
}

} // namespace

CRgdHashTable::CRgdHashTable()
{
    for (int i = 1; i < 10000; ++i)
    {
        auto sStr = std::to_string(i);
        _Value val;
        val.bCustom = false;
        val.sString = sStr;
        m_mHashTable[ComputeHash(sStr.c_str(), sStr.size())] = std::move(val);
    }
}

CRgdHashTable::~CRgdHashTable() = default;

void CRgdHashTable::New() { _Clean(); }

void CRgdHashTable::FillUnknownList(std::vector<unsigned long> &oList)
{
    for (const auto &[key, val] : m_mHashTable)
    {
        if (!val.sString.has_value())
            oList.push_back(key);
    }
}

void CRgdHashTable::ExtendWithDictionary(const char *sFile, bool bCustom)
{
    FILE *fFile = fopen(sFile, "rb");
    if (fFile == nullptr)
        throw new CRainmanException(nullptr, __FILE__, __LINE__, "Failed to open file \'%s\'", sFile);

    std::string sLine;
    while (ReadLine(fFile, sLine))
    {
        bool bUnk = false;
        if (sLine.size() >= 34 && sLine.compare(0, 4, "# 0x") == 0 &&
            sLine.compare(12, 22, " is an unknown value!!") == 0)
        {
            bUnk = true;
            sLine[0] = ' ';
            sLine[12] = '=';
        }

        if (sLine.empty() || sLine[0] == '#')
            continue;

        // Parse hex key from "0xHHHHHHHH=..." format
        unsigned long iKey = 0;
        int iBitsRead = -8;
        size_t pos = 0;
        while (pos < sLine.size() && sLine[pos] != '=')
        {
            char c = sLine[pos];
            if (iBitsRead == -8)
            {
                if (c == '0')
                    iBitsRead = -4;
            }
            else if (iBitsRead == -4)
            {
                if (c == 'x' || c == 'X')
                    iBitsRead = 0;
            }
            else if (iBitsRead < 32)
            {
                if (c >= '0' && c <= '9')
                {
                    iKey <<= 4;
                    iKey |= static_cast<unsigned long>(c - '0');
                    iBitsRead += 4;
                }
                else if (c >= 'a' && c <= 'f')
                {
                    iKey <<= 4;
                    iKey |= static_cast<unsigned long>(c - 'a' + 10);
                    iBitsRead += 4;
                }
                else if (c >= 'A' && c <= 'F')
                {
                    iKey <<= 4;
                    iKey |= static_cast<unsigned long>(c - 'A' + 10);
                    iBitsRead += 4;
                }
            }
            ++pos;
        }

        if (pos >= sLine.size() || sLine[pos] != '=')
            continue;
        ++pos; // skip '='

        // Parse value (strip whitespace)
        std::optional<std::string> sValue;
        if (!bUnk)
            sValue = StripWhitespace(sLine, pos);

        auto &existing = m_mHashTable[iKey];
        if (existing.sString.has_value())
        {
            existing.bCustom = existing.bCustom && bCustom;
        }
        else
        {
            existing.sString = std::move(sValue);
            existing.bCustom = bCustom;
        }
    }

    fclose(fFile);
}

void CRgdHashTable::XRefWithStringList(const char *sFile)
{
    FILE *fFile = fopen(sFile, "rb");
    if (fFile == nullptr)
        throw new CRainmanException(nullptr, __FILE__, __LINE__, "Failed to open file \'%s\'", sFile);

    std::string sLine;
    while (ReadLine(fFile, sLine))
    {
        unsigned long iKey = ComputeHash(sLine.c_str(), sLine.size());

        auto it = m_mHashTable.find(iKey);
        if (it != m_mHashTable.end() && !it->second.sString.has_value())
        {
            it->second.sString = sLine;
            it->second.bCustom = true;
        }
    }
    fclose(fFile);
}

void CRgdHashTable::SaveCustomKeys(const char *sFile)
{
    FILE *fFile = fopen(sFile, "wb");
    if (fFile == nullptr)
        throw new CRainmanException(nullptr, __FILE__, __LINE__, "Failed to open file \'%s\'", sFile);

    fputs("#RGD_DIC\n# This dictionary is generated from any keys that are \'discovered\'\n", fFile);

    for (const auto &[iKey, val] : m_mHashTable)
    {
        if (val.bCustom)
        {
            if (!val.sString.has_value())
                fputs("# ", fFile);
            // Output key
            fputs("0x", fFile);
            unsigned long iMask = 0xF0000000;
            for (int i = 7; i >= 0; --i)
            {
                fputc("0123456789ABCDEF"[(iKey & iMask) >> (i << 2)], fFile);
                iMask >>= 4;
            }

            // Output value
            if (!val.sString.has_value())
            {
                fputs(" is an unknown value!!\n", fFile);
            }
            else
            {
                fputc('=', fFile);
                fputs(val.sString->c_str(), fFile);
                fputc('\n', fFile);
            }
        }
    }

    fclose(fFile);
}

const char *CRgdHashTable::HashToValue(unsigned long iHash)
{
    auto &val = m_mHashTable[iHash];
    if (!val.sString.has_value())
    {
        val.bCustom = true;
        return nullptr;
    }
    return val.sString->c_str();
}

unsigned long CRgdHashTable::ValueToHash(const char *sValue)
{
    unsigned long iKey = 0;
    if (TryParseHexLiteral(sValue, std::strlen(sValue), iKey))
        return iKey;

    iKey = ComputeHash(sValue, std::strlen(sValue));
    auto &val = m_mHashTable[iKey];
    if (!val.sString.has_value())
    {
        // Non-numeric strings are flagged as custom
        bool bIsNonNumeric = false;
        for (const char *p = sValue; *p; ++p)
        {
            if (*p < '0' || *p > '9')
            {
                bIsNonNumeric = true;
                break;
            }
        }
        val.bCustom = bIsNonNumeric;
        val.sString = sValue;
    }
    return iKey;
}

unsigned long CRgdHashTable::ValueToHashStatic(const char *sValue, size_t iValueLen)
{
    unsigned long iKey = 0;
    if (TryParseHexLiteral(sValue, iValueLen, iKey))
        return iKey;
    return ComputeHash(sValue, iValueLen);
}

unsigned long CRgdHashTable::ValueToHashStatic(const char *sValue)
{
    return ValueToHashStatic(sValue, std::strlen(sValue));
}

void CRgdHashTable::_Clean() { m_mHashTable.clear(); }
