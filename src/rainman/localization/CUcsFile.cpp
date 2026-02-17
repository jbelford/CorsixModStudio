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

#include "rainman/localization/CUcsFile.h"
#include "rainman/core/memdebug.h"
#include "rainman/core/Exception.h"
#include "rainman/core/RainmanLog.h"
#include <memory>
#include <vector>

CUcsFile::CUcsFile() {}

CUcsFile::~CUcsFile() { _Clean(); }

bool CUcsFile::IsDollarString(const char *s)
{
    if (!s || *s != '$')
        return false;
    ++s;
    if (*s == 0)
        return false;
    while (*s)
    {
        if (*s < '0' || *s > '9')
            return false;
        ++s;
    }
    return true;
}

bool CUcsFile::IsDollarString(const wchar_t *s)
{
    if (!s || *s != '$')
        return false;
    ++s;
    if (*s == 0)
        return false;
    while (*s)
    {
        if (*s < '0' || *s > '9')
            return false;
        ++s;
    }
    return true;
}

void CUcsFile::New()
{
    _Clean();
    return;
}

void CUcsFile::Save(const char *sFile)
{
    RAINMAN_LOG_INFO("CUcsFile::Save() — writing UCS file");
    FILE *f = fopen(sFile, "wb");
    if (!f)
        throw new CRainmanException(nullptr, __FILE__, __LINE__, "Cannot open file \'%s\' in mode \'wb\'", sFile);
    unsigned short iHeader = 0xFEFF;
    fwrite(&iHeader, 2, 1, f);
    for (auto itr = m_mapValues.begin(); itr != m_mapValues.end(); ++itr)
    {
        if (itr->second)
        {
            // get the string representation of the number and write it
            wchar_t sBuffer[35];
            _ltow((long)itr->first, sBuffer, 10);
            fputws(sBuffer, f);
            wchar_t t;

            // write the tab delimeter and value
            t = 0x09;
            fwrite(&t, 2, 1, f);
            fputws(itr->second, f);

            // write newline
            t = 0x0D;
            fwrite(&t, 2, 1, f);
            t = 0x0A;
            fwrite(&t, 2, 1, f);
        }
    }
    fclose(f);
}

std::map<unsigned long, wchar_t *> *CUcsFile::GetRawMap() { return &m_mapValues; }

const std::map<unsigned long, wchar_t *> *CUcsFile::GetRawMap() const { return &m_mapValues; }

static wchar_t *mywcsdup(const char *sStr)
{
    auto *s = new wchar_t[strlen(sStr) + 1];
    if (s == nullptr)
        return nullptr;
    for (size_t i = 0; i <= strlen(sStr); ++i)
        s[i] = sStr[i];
    return s;
}

static wchar_t *mywcsdup(const wchar_t *sStr)
{
    /*
        Equivalent to wcsdup(), except uses "new" instead of "malloc"
        (and thus "delete" instead of "free")
    */
    auto *s = new wchar_t[wcslen(sStr) + 1];
    if (s == nullptr)
        return nullptr;
    wcscpy(s, sStr);
    return s;
}

static wchar_t *readwideline(IFileStore::IStream *pStream, unsigned long iInitSize = 32)
{
    /*
        reads one unicode line from the stream
        does not throw CRainmanException

        Words == 2 bytes (eg. computing "word", not linguistic "word")
    */
    wchar_t *sBuffer = CHECK_MEM(new wchar_t[iInitSize]);
    memset(sBuffer, 0, sizeof(wchar_t) * iInitSize);
    unsigned long iWordsRead = 0;

    try
    {
        pStream->VRead(1, sizeof(uint16_t), sBuffer + iWordsRead);
    }
    catch (CRainmanException *pE)
    {
        auto guard = std::unique_ptr<CRainmanException, ExceptionDeleter>(pE);
        delete[] sBuffer;
        return nullptr;
    }

    try
    {
        do
        {
            if (++iWordsRead >= (iInitSize - 1)) // _shouldn't_ be '>' but you never know...
            {
                auto *sTmp = new wchar_t[iInitSize <<= 1];
                if (sTmp == nullptr)
                {
                    delete[] sBuffer;
                    return nullptr;
                }
                memset(sTmp, 0, sizeof(wchar_t) * iInitSize);
                wcscpy(sTmp, sBuffer);
                delete[] sBuffer;
                sBuffer = sTmp;
            }
            if ((char)sBuffer[iWordsRead - 1] == '\x0A')
                break;
            pStream->VRead(1, sizeof(uint16_t), sBuffer + iWordsRead);
        } while (1);
    }
    IGNORE_EXCEPTIONS

    return sBuffer;
}

static char *readasciiline(IFileStore::IStream *pStream, unsigned long iInitSize = 32)
{
    /*
        reads one unicode line from the stream
        does not throw CRainmanException

        Words == 2 bytes (eg. computing "word", not linguistic "word")
    */
    char *sBuffer = CHECK_MEM(new char[iInitSize]);
    memset(sBuffer, 0, sizeof(char) * iInitSize);
    unsigned long iWordsRead = 0;

    try
    {
        pStream->VRead(1, sizeof(char), sBuffer + iWordsRead);
    }
    catch (CRainmanException *pE)
    {
        auto guard = std::unique_ptr<CRainmanException, ExceptionDeleter>(pE);
        delete[] sBuffer;
        return nullptr;
    }

    try
    {
        do
        {
            if (++iWordsRead >= (iInitSize - 1)) // _shouldn't_ be '>' but you never know...
            {
                char *sTmp = new char[iInitSize <<= 1];
                if (sTmp == nullptr)
                {
                    delete[] sBuffer;
                    return nullptr;
                }
                memset(sTmp, 0, sizeof(char) * iInitSize);
                strcpy(sTmp, sBuffer);
                delete[] sBuffer;
                sBuffer = sTmp;
            }
            if ((char)sBuffer[iWordsRead - 1] == '\x0A')
                break;
            pStream->VRead(1, sizeof(char), sBuffer + iWordsRead);
        } while (1);
    }
    IGNORE_EXCEPTIONS

    return sBuffer;
}

void CUcsFile::LoadDat(IFileStore::IStream *pStream)
{
    _Clean();

    char *sLine = nullptr;
    while (sLine = readasciiline(pStream))
    {
        unsigned long iNumber = 0;
        unsigned long i = 0;
        if (sLine[i] >= '0' && sLine[i] <= '9')
        {
            while (sLine[i] && sLine[i] >= '0' && sLine[i] <= '9')
            {
                iNumber *= 10;
                iNumber += (sLine[i] - '0');
                ++i;
            }
            if (i != 0 && sLine[i] && sLine[i + 1] &&
                m_mapValues[iNumber] == nullptr) // silenty ignore duplicate values
            {
                ++i;
                wchar_t *sString = mywcsdup(sLine + i);
                if (sString)
                {
                    wchar_t *sTmp;
                    if (sTmp = wcschr(sString, 0x0D))
                        *sTmp = 0;
                    if (sTmp = wcschr(sString, 0x0A))
                        *sTmp = 0;

                    m_mapValues[iNumber] = sString;
                }
            }
        }

        delete[] sLine;
    }
}

void CUcsFile::Load(IFileStore::IStream *pStream)
{
    RAINMAN_LOG_INFO("CUcsFile::Load() — parsing UCS string table");
    _Clean();

    unsigned short iHeader;
    try
    {
        pStream->VRead(1, sizeof(short), &iHeader);
    }
    CATCH_THROW("Failed to read header")

    if (iHeader != 0xFEFF)
    {
        throw new CRainmanException(nullptr, __FILE__, __LINE__, "Invalid header (%u)", iHeader);
    }

    // Bulk-read the entire file into memory after the BOM
    long iStartPos = pStream->VTell();
    pStream->VSeek(0, IFileStore::IStream::SL_End);
    long iEndPos = pStream->VTell();
    long iDataBytes = iEndPos - iStartPos;
    pStream->VSeek(iStartPos, IFileStore::IStream::SL_Root);

    if (iDataBytes <= 0)
        return;

    // Data is UTF-16LE: each character is 2 bytes (uint16_t)
    auto iCharCount = static_cast<unsigned long>(iDataBytes / sizeof(uint16_t));
    auto buffer = std::make_unique<uint16_t[]>(iCharCount);
    try
    {
        pStream->VRead(iCharCount, sizeof(uint16_t), buffer.get());
    }
    CATCH_THROW("Failed to bulk-read UCS file data")

    // Parse lines from the in-memory buffer
    uint16_t *pCur = buffer.get();
    uint16_t *pEnd = pCur + iCharCount;

    while (pCur < pEnd)
    {
        // Find end of line (scan for \n = 0x000A)
        uint16_t *pLineStart = pCur;
        while (pCur < pEnd && *pCur != 0x000A)
            ++pCur;

        // pCur now points at \n or past end
        uint16_t *pLineEnd = pCur;
        if (pCur < pEnd)
            ++pCur; // skip \n

        // Strip trailing \r (0x000D)
        if (pLineEnd > pLineStart && *(pLineEnd - 1) == 0x000D)
            --pLineEnd;

        // Skip empty lines
        if (pLineEnd == pLineStart)
            continue;

        // Parse: "<number>\t<string>"
        unsigned long iNumber = 0;
        uint16_t *p = pLineStart;
        bool hasDigits = false;
        while (p < pLineEnd && *p >= '0' && *p <= '9')
        {
            iNumber = iNumber * 10 + (*p - '0');
            ++p;
            hasDigits = true;
        }

        if (!hasDigits || p >= pLineEnd || *(p) == 0)
            continue;

        ++p; // skip tab separator

        if (p >= pLineEnd)
            continue;

        // Only store if not a duplicate
        if (m_mapValues[iNumber] != nullptr)
            continue;

        // Copy the string value
        size_t iLen = static_cast<size_t>(pLineEnd - p);
        auto *sString = new wchar_t[iLen + 1];
        for (size_t j = 0; j < iLen; ++j)
            sString[j] = static_cast<wchar_t>(p[j]);
        sString[iLen] = 0;

        m_mapValues[iNumber] = sString;
    }
}

const wchar_t *CUcsFile::ResolveStringID(unsigned long iID)
{
    const wchar_t *pS = m_mapValues[iID];
    if (!pS)
        return nullptr; // throw new CRainmanException(0, __FILE__, __LINE__, "$%ul no key", iID);
    return pS;
}

void CUcsFile::SetString(unsigned long iID, const wchar_t *pString)
{
    delete[] m_mapValues[iID];
    if (pString == nullptr)
    {
        m_mapValues.erase(iID);
        return;
    }
    wchar_t *pTmp = mywcsdup(pString);
    if (pTmp == nullptr)
        throw new CRainmanException(__FILE__, __LINE__, "Cannot duplicate string (out of memory?)");
    m_mapValues[iID] = pTmp;
}

void CUcsFile::ReplaceString(unsigned long iID, wchar_t *pString)
{
    delete[] m_mapValues[iID];
    if (pString == nullptr)
    {
        m_mapValues.erase(iID);
        return;
    }
    m_mapValues[iID] = pString;
}

void CUcsFile::_Clean()
{
    for (auto itr = m_mapValues.begin(); itr != m_mapValues.end(); ++itr)
        delete[] itr->second;
    m_mapValues.erase(m_mapValues.begin(), m_mapValues.end());
}
