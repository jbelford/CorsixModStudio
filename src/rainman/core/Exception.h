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

#include "rainman/core/gnuc_defines.h"
#include "rainman/core/Api.h"
#include <memory>
#include <string>

class RAINMAN_API CRainmanException
{
  public:
    //! Simple message, no precursor.
    CRainmanException(const char *sFile, unsigned long iLine, const char *sMessage);

    //! Formatted message (printf-style), deep-copies precursor.
    CRainmanException(const CRainmanException &precursor, const char *sFile, unsigned long iLine, const char *sFormat,
                      ...);

    //! Formatted message (printf-style), no precursor (pass \c nullptr).
    CRainmanException(CRainmanException *pPrecursor, const char *sFile, unsigned long iLine, const char *sFormat, ...);

    //! Deep-copy constructor (recursively copies precursor chain).
    CRainmanException(const CRainmanException &other);
    CRainmanException &operator=(const CRainmanException &other);

    CRainmanException(CRainmanException &&) noexcept = default;
    CRainmanException &operator=(CRainmanException &&) noexcept = default;

    virtual ~CRainmanException() = default;

    const char *getFile() const { return m_sFile; }
    unsigned long getLine() const { return m_iLine; }
    const char *getMessage() const { return m_sMessage.c_str(); }

    const CRainmanException *getPrecursor() const { return m_pPrecursor.get(); }

  protected:
    CRainmanException() = default;

    std::string m_sMessage;
    unsigned long m_iLine = 0;
    const char *m_sFile = nullptr;

    std::unique_ptr<CRainmanException> m_pPrecursor;
};

#ifndef EXCEPTION_CLASS
#define EXCEPTION_CLASS CRainmanException
#endif

//! Macro for easy CRainmanException re-throwing
#define CATCH_THROW(msg)                                                                                               \
    catch (const CRainmanException &_rainmanPrecursor)                                                                 \
    {                                                                                                                  \
        throw EXCEPTION_CLASS(_rainmanPrecursor, __FILE__, __LINE__, msg);                                             \
    }

//! Macro for easily throwing a simple error
#define QUICK_THROW(msg) throw EXCEPTION_CLASS(__FILE__, __LINE__, msg);

//! Macro for ignoring all RainmanExceptions from a try{} block
#define IGNORE_EXCEPTIONS                                                                                              \
    catch (const CRainmanException &) {}

//! Will throw an exception if pointer is invalid, returns the value passed otherwise
template <class T> __inline T CheckMem(T pObj, const char *sFile, unsigned long iLine)
{
    if (pObj == nullptr)
        throw EXCEPTION_CLASS(sFile, iLine, "Memory allocation error");
    return pObj;
}

//! Will throw an exception if pointer is invalid, returns the value passed otherwise
template <class T> __inline T CheckString(T pObj, const char *sFile, unsigned long iLine)
{
    if (pObj == nullptr)
        throw EXCEPTION_CLASS(sFile, iLine, "No string");
    return pObj;
}

//! Macro for quick checking of memory allocation errors
/*!
    Wrap this macro around "new" to automatically throw a CRainmanException if "new" fails.
    Example:
    \code
        Class* Variable = CHECK_MEM(new Class);
        Class* Variable_Array = CHECK_MEM(new Class[5]);
    \endcode
*/
#define CHECK_MEM(ptr) CheckMem(ptr, __FILE__, __LINE__)

//! Macro to check string return values
#define CHECK_STR(ptr) CheckString(ptr, __FILE__, __LINE__)

#define PAUSE_THROW auto _pausedEx = EXCEPTION_CLASS
#define UNPAUSE_THROW throw std::move(_pausedEx)
