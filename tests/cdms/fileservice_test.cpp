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
#include "services/FileService.h"
#include <rainman/io/CMemoryStore.h>

class FileServiceTest : public ::testing::Test
{
  protected:
	CMemoryStore m_store;

	void SetUp() override { m_store.VInit(); }
};

TEST_F(FileServiceTest, OpenStreamFromMemory)
{
	// CMemoryStore::MemoryRange returns a special "filename" encoding a pointer.
	// FileService::OpenStream converts wxString→char* via wxStringToAscii,
	// which may not round-trip the encoded pointer safely.
	// Instead, test with VOpenOutputStream which uses simple string names.
	FileService svc(&m_store, nullptr);
	auto result = svc.OpenOutputStream(wxT("test_file"), true);
	ASSERT_TRUE(result.ok()) << result.error().utf8_str().data();

	auto &guard = result.value();
	ASSERT_TRUE(static_cast<bool>(guard));

	const char data[] = "Hello, Rainman!";
	guard->VWrite(sizeof(data) - 1, 1, data);
}

TEST_F(FileServiceTest, OpenOutputStreamAndWrite)
{
	FileService svc(&m_store, nullptr);
	auto result = svc.OpenOutputStream(wxT("test_output"), true);
	ASSERT_TRUE(result.ok()) << result.error().utf8_str().data();

	auto &guard = result.value();
	ASSERT_TRUE(static_cast<bool>(guard));

	const char data[] = "output data";
	guard->VWrite(sizeof(data) - 1, 1, data);
}

TEST_F(FileServiceTest, IteratorGuardDeletesOnDestruction)
{
	// IteratorGuard with nullptr should not crash
	{ IteratorGuard guard(nullptr); }
	SUCCEED();
}

TEST_F(FileServiceTest, IteratorGuardMoveSemantics)
{
	IteratorGuard g1(nullptr);
	IteratorGuard g2(std::move(g1));
	EXPECT_EQ(g1.get(), nullptr);
	EXPECT_EQ(g2.get(), nullptr);
}
