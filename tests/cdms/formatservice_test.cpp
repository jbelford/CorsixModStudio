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
#include "services/FormatService.h"
#include <rainman/formats/CRgdFile.h>
#include <rainman/formats/CRgdHashTable.h>
#include <rainman/formats/CChunkyFile.h>
#include <rainman/io/CMemoryStore.h>

class FormatServiceTest : public ::testing::Test
{
  protected:
	CMemoryStore m_store;

	void SetUp() override { m_store.VInit(); }
};

TEST_F(FormatServiceTest, LoadRgdFromInvalidStreamReturnsError)
{
	// Create a stream with invalid RGD data
	const char badData[] = "this is not valid RGD data at all";
	char *pRange = m_store.MemoryRange((void *)badData, sizeof(badData) - 1);
	auto *pIn = m_store.VOpenStream(pRange);
	ASSERT_NE(pIn, nullptr);

	CRgdHashTable hashTable;
	CRgdFile rgd;
	rgd.SetHashTable(&hashTable);
	auto result = FormatService::LoadRgd(&rgd, pIn);
	delete pIn;
	// Should return an error (invalid data), not crash
	EXPECT_FALSE(result.ok());
}

TEST_F(FormatServiceTest, SaveChunkyReturnsOk)
{
	CChunkyFile chunky;
	// Save an empty chunky file
	auto *pOut = m_store.VOpenOutputStream("test.chunky", true);
	ASSERT_NE(pOut, nullptr);
	auto result = FormatService::SaveChunky(&chunky, pOut);
	delete pOut;
	ASSERT_TRUE(result.ok()) << result.error().utf8_str().data();
}
