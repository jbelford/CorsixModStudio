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

#include "rainman/core/Exception.h"
#include "rainman/core/RainmanLog.h"
#include <cstdio>
#include <cstdarg>
#include "rainman/core/memdebug.h"

CRainmanException::CRainmanException(const char *sFile, unsigned long iLine, const char *sMessage)
    : m_sMessage(sMessage ? sMessage : ""), m_iLine(iLine), m_sFile(sFile)
{
    RAINMAN_LOG_ERROR("Exception at {}:{} — {}", sFile ? sFile : "?", iLine, m_sMessage);
}

CRainmanException::CRainmanException(const CRainmanException &precursor, const char *sFile, unsigned long iLine,
                                     const char *sFormat, ...)
    : m_iLine(iLine), m_sFile(sFile), m_pPrecursor(std::make_unique<CRainmanException>(precursor))
{
    size_t iL = 128;
    va_list marker;
    while (true)
    {
        m_sMessage.resize(iL);
        va_start(marker, sFormat);
        int n = _vsnprintf(m_sMessage.data(), iL, sFormat, marker);
        va_end(marker);
        if (n >= 0 && static_cast<size_t>(n) < iL)
        {
            m_sMessage.resize(static_cast<size_t>(n));
            break;
        }
        iL <<= 1;
    }
    RAINMAN_LOG_ERROR("Exception at {}:{} — {}", sFile ? sFile : "?", iLine, m_sMessage);
}

CRainmanException::CRainmanException(CRainmanException *pPrecursor, const char *sFile, unsigned long iLine,
                                     const char *sFormat, ...)
    : m_iLine(iLine), m_sFile(sFile), m_pPrecursor(pPrecursor)
{
    size_t iL = 128;
    va_list marker;
    while (true)
    {
        m_sMessage.resize(iL);
        va_start(marker, sFormat);
        int n = _vsnprintf(m_sMessage.data(), iL, sFormat, marker);
        va_end(marker);
        if (n >= 0 && static_cast<size_t>(n) < iL)
        {
            m_sMessage.resize(static_cast<size_t>(n));
            break;
        }
        iL <<= 1;
    }
    RAINMAN_LOG_ERROR("Exception at {}:{} — {}", sFile ? sFile : "?", iLine, m_sMessage);
}

CRainmanException::CRainmanException(const CRainmanException &other)
    : m_sMessage(other.m_sMessage), m_iLine(other.m_iLine), m_sFile(other.m_sFile),
      m_pPrecursor(other.m_pPrecursor ? std::make_unique<CRainmanException>(*other.m_pPrecursor) : nullptr)
{
}

CRainmanException &CRainmanException::operator=(const CRainmanException &other)
{
    if (this != &other)
    {
        m_sMessage = other.m_sMessage;
        m_iLine = other.m_iLine;
        m_sFile = other.m_sFile;
        m_pPrecursor = other.m_pPrecursor ? std::make_unique<CRainmanException>(*other.m_pPrecursor) : nullptr;
    }
    return *this;
}
