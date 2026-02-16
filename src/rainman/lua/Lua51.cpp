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

/*
** Lua 5.1.2 is now statically linked via lua512.lib with renamed symbols.
** This file previously loaded lua512.dll at runtime via LoadLibraryW and
** populated ~100 function pointers via GetProcAddress.  That is no longer
** needed — all lua51_* functions are real symbols in the static library.
**
** Lua51_Load() is kept as a no-op for backward compatibility with any
** callers that still reference it.
*/

#include "rainman/core/Api.h"

RAINMAN_API bool Lua51_Load(const wchar_t *sDll)
{
	(void)sDll;
	return true;
}
