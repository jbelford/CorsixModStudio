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

#include <wx/string.h>
#include <rainman/core/Exception.h>
#include <type_traits>
#include <utility>

//! Lightweight result type for service-layer error handling.
/*!
    Wraps either a success value of type T, or an error message (wxString).
    Used by CDMS service classes to avoid throwing exceptions across the
    service boundary — callers check ok() and handle errors with GUI feedback.

    \code
        Result<int> r = Result<int>::Ok(42);
        if (r.ok()) use(r.value());

        Result<void> v = Result<void>::Err(wxT("something failed"));
        if (!v.ok()) showError(v.error());
    \endcode
*/
template <typename T> class Result
{
  public:
    static Result Ok(T val) { return Result(OkTag{}, std::move(val)); }
    static Result Err(wxString msg) { return Result(ErrTag{}, std::move(msg)); }

    bool ok() const noexcept { return m_ok; }
    explicit operator bool() const noexcept { return m_ok; }

    T &value() &
    {
        wxASSERT(m_ok);
        return m_value;
    }
    const T &value() const &
    {
        wxASSERT(m_ok);
        return m_value;
    }
    T &&value() &&
    {
        wxASSERT(m_ok);
        return std::move(m_value);
    }

    const wxString &error() const
    {
        wxASSERT(!m_ok);
        return m_error;
    }

  private:
    struct OkTag
    {
    };
    struct ErrTag
    {
    };
    Result(OkTag, T val) : m_ok(true), m_value(std::move(val)) {}
    Result(ErrTag, wxString msg) : m_ok(false), m_error(std::move(msg)) {}

    bool m_ok;
    T m_value{};
    wxString m_error;
};

//! Specialization of Result for void (no success value).
template <> class Result<void>
{
  public:
    static Result Ok() { return Result(true); }
    static Result Err(wxString msg) { return Result(std::move(msg)); }

    bool ok() const noexcept { return m_ok; }
    explicit operator bool() const noexcept { return m_ok; }

    const wxString &error() const
    {
        wxASSERT(!m_ok);
        return m_error;
    }

  private:
    explicit Result(bool ok) : m_ok(ok) {}
    explicit Result(wxString msg) : m_ok(false), m_error(std::move(msg)) {}

    bool m_ok;
    wxString m_error;
};

//! Helper to build a Result<void>::Err from a CRainmanException chain.
/*!
    Formats the full exception chain into a single wxString and calls
    pE->destroy() to clean up the heap-allocated exception.
    Typical usage inside a service method:

    \code
        catch (CRainmanException *pE)
        {
            return ResultFromException(pE);
        }
    \endcode
*/
inline Result<void> ResultFromException(CRainmanException *pE)
{
    wxString msg;
    const CRainmanException *pCur = pE;
    while (pCur)
    {
        if (!msg.empty())
            msg += wxT("\n  caused by: ");
        msg += wxString::FromUTF8(pCur->getMessage());
        pCur = pCur->getPrecursor();
    }
    pE->destroy();
    return Result<void>::Err(std::move(msg));
}

//! Typed variant of ResultFromException for Result<T>.
template <typename T> Result<T> ResultFromExceptionT(CRainmanException *pE)
{
    wxString msg;
    const CRainmanException *pCur = pE;
    while (pCur)
    {
        if (!msg.empty())
            msg += wxT("\n  caused by: ");
        msg += wxString::FromUTF8(pCur->getMessage());
        pCur = pCur->getPrecursor();
    }
    pE->destroy();
    return Result<T>::Err(std::move(msg));
}
