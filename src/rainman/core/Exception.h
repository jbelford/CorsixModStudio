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

class RAINMAN_API CRainmanException
{
  public:
    CRainmanException(const char *sFile, unsigned long iLine, const char *sMessage, CRainmanException *pPrecursor = 0);
    CRainmanException(CRainmanException *pPrecursor, const char *sFile, unsigned long iLine, const char *sFormat, ...);

    virtual void destroy();

    inline const char *getFile() const { return m_sFile; }
    inline unsigned long getLine() const { return m_iLine; }
    inline const char *getMessage() const { return m_sMessage; }

    inline const CRainmanException *getPrecursor() const { return m_pPrecursor; }

  protected:
    CRainmanException();
    ~CRainmanException(); //!< DO NOT CALL THIS, use destroy() instead.

    char *m_sMessage;
    unsigned long m_iLine;
    const char *m_sFile;

    CRainmanException *m_pPrecursor;
};

//! Custom deleter for use with std::unique_ptr<CRainmanException, ExceptionDeleter>.
struct ExceptionDeleter
{
    void operator()(CRainmanException *p) const noexcept
    {
        if (p)
            p->destroy();
    }
};

#ifndef EXCEPTION_CLASS
#define EXCEPTION_CLASS CRainmanException
#endif

//! Macro for easy CRainmanException re-throwing
#define CATCH_THROW(msg)                                                                                               \
    catch (CRainmanException * pE) { throw new EXCEPTION_CLASS(__FILE__, __LINE__, msg, pE); }

//! Macro for easily throwing a simple error
#define QUICK_THROW(msg) throw new EXCEPTION_CLASS(__FILE__, __LINE__, msg);

//! Macro for ignoring all RainmanExceptions from a try{} block
#define IGNORE_EXCEPTIONS                                                                                              \
    catch (CRainmanException * pE) { std::unique_ptr<CRainmanException, ExceptionDeleter> _ignored(pE); }

//! Will throw an exception if pointer is invalid, returns the value passed otherwise
template <class T> __inline T CheckMem(T pObj, const char *sFile, unsigned long iLine)
{
    if (pObj == 0)
        throw new EXCEPTION_CLASS(sFile, iLine, "Memory allocation error");
    return pObj;
}

//! Will throw an exception if pointer is invalid, returns the value passed otherwise
template <class T> __inline T CheckString(T pObj, const char *sFile, unsigned long iLine)
{
    if (pObj == 0)
        throw new EXCEPTION_CLASS(sFile, iLine, "No string");
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

#define PAUSE_THROW CRainmanException *pPausedException = new EXCEPTION_CLASS
#define UNPAUSE_THROW throw pPausedException
