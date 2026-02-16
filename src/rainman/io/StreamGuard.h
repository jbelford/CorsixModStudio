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

#ifndef _STREAM_GUARD_H_
#define _STREAM_GUARD_H_

#include "rainman/io/IFileStore.h"

namespace rainman
{

//! RAII wrapper for IFileStore::IStream pointers.
/*!
    Owns an IStream* and deletes it on destruction.
    Callers can opt in to this wrapper gradually alongside existing manual code.
*/
class StreamGuard
{
  public:
	explicit StreamGuard(IFileStore::IStream *p = nullptr) noexcept : m_p(p) {}
	~StreamGuard() { delete m_p; }

	StreamGuard(StreamGuard &&other) noexcept : m_p(other.m_p) { other.m_p = nullptr; }
	StreamGuard &operator=(StreamGuard &&other) noexcept
	{
		if (this != &other)
		{
			delete m_p;
			m_p = other.m_p;
			other.m_p = nullptr;
		}
		return *this;
	}

	StreamGuard(const StreamGuard &) = delete;
	StreamGuard &operator=(const StreamGuard &) = delete;

	IFileStore::IStream *get() const noexcept { return m_p; }
	IFileStore::IStream *operator->() const noexcept { return m_p; }
	IFileStore::IStream *release() noexcept
	{
		auto *p = m_p;
		m_p = nullptr;
		return p;
	}
	explicit operator bool() const noexcept { return m_p != nullptr; }

  private:
	IFileStore::IStream *m_p;
};

//! RAII wrapper for IFileStore::IOutputStream pointers.
/*!
    Owns an IOutputStream* and deletes it on destruction.
*/
class OutputStreamGuard
{
  public:
	explicit OutputStreamGuard(IFileStore::IOutputStream *p = nullptr) noexcept : m_p(p) {}
	~OutputStreamGuard() { delete m_p; }

	OutputStreamGuard(OutputStreamGuard &&other) noexcept : m_p(other.m_p) { other.m_p = nullptr; }
	OutputStreamGuard &operator=(OutputStreamGuard &&other) noexcept
	{
		if (this != &other)
		{
			delete m_p;
			m_p = other.m_p;
			other.m_p = nullptr;
		}
		return *this;
	}

	OutputStreamGuard(const OutputStreamGuard &) = delete;
	OutputStreamGuard &operator=(const OutputStreamGuard &) = delete;

	IFileStore::IOutputStream *get() const noexcept { return m_p; }
	IFileStore::IOutputStream *operator->() const noexcept { return m_p; }
	IFileStore::IOutputStream *release() noexcept
	{
		auto *p = m_p;
		m_p = nullptr;
		return p;
	}
	explicit operator bool() const noexcept { return m_p != nullptr; }

  private:
	IFileStore::IOutputStream *m_p;
};

} // namespace rainman

#endif
