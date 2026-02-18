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

#include <gtest/gtest.h>
#include "services/Result.h"
#include <rainman/core/Exception.h>

// --- Result<int> ---

TEST(ResultTest, OkInt)
{
	auto r = Result<int>::Ok(42);
	EXPECT_TRUE(r.ok());
	EXPECT_TRUE(static_cast<bool>(r));
	EXPECT_EQ(r.value(), 42);
}

TEST(ResultTest, ErrInt)
{
	auto r = Result<int>::Err(wxT("something broke"));
	EXPECT_FALSE(r.ok());
	EXPECT_FALSE(static_cast<bool>(r));
	EXPECT_EQ(r.error(), wxT("something broke"));
}

// --- Result<void> ---

TEST(ResultTest, OkVoid)
{
	auto r = Result<void>::Ok();
	EXPECT_TRUE(r.ok());
}

TEST(ResultTest, ErrVoid)
{
	auto r = Result<void>::Err(wxT("failed"));
	EXPECT_FALSE(r.ok());
	EXPECT_EQ(r.error(), wxT("failed"));
}

// --- Result<wxString> ---

TEST(ResultTest, OkString)
{
	auto r = Result<wxString>::Ok(wxT("hello"));
	EXPECT_TRUE(r.ok());
	EXPECT_EQ(r.value(), wxT("hello"));
}

// --- ResultFromException ---

TEST(ResultTest, FromExceptionSimple)
{
	CRainmanException ex(__FILE__, __LINE__, "test error");
	auto r = ResultFromException(ex);
	EXPECT_FALSE(r.ok());
	EXPECT_TRUE(r.error().Contains(wxT("test error")));
}

TEST(ResultTest, FromExceptionChain)
{
	CRainmanException inner(__FILE__, __LINE__, "inner");
	CRainmanException outer(inner, __FILE__, __LINE__, "outer");
	auto r = ResultFromException(outer);
	EXPECT_FALSE(r.ok());
	EXPECT_TRUE(r.error().Contains(wxT("outer")));
	EXPECT_TRUE(r.error().Contains(wxT("inner")));
}

TEST(ResultTest, FromExceptionTyped)
{
	CRainmanException ex(__FILE__, __LINE__, "typed error");
	auto r = ResultFromExceptionT<int>(ex);
	EXPECT_FALSE(r.ok());
	EXPECT_TRUE(r.error().Contains(wxT("typed error")));
}

// --- Move semantics ---

TEST(ResultTest, MoveOk)
{
	auto r1 = Result<wxString>::Ok(wxT("hello"));
	auto r2 = std::move(r1);
	EXPECT_TRUE(r2.ok());
	EXPECT_EQ(r2.value(), wxT("hello"));
}

TEST(ResultTest, MoveErr)
{
	auto r1 = Result<int>::Err(wxT("error"));
	auto r2 = std::move(r1);
	EXPECT_FALSE(r2.ok());
	EXPECT_EQ(r2.error(), wxT("error"));
}
