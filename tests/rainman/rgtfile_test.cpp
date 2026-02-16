#include <gtest/gtest.h>
#include "rainman/formats/CRgtFile.h"
#include "rainman/io/CMemoryStore.h"
#include <memory>
#include "rainman/core/Exception.h"
#include <cstring>
#include <string>
#include <vector>

class RgtFileTest : public ::testing::Test {
protected:
	CRgtFile rgt;
	CMemoryStore store;

	void SetUp() override { store.VInit(); }

	// Build a minimal uncompressed 32-bit RGBA TGA
	static std::vector<unsigned char> makeTga(unsigned short width, unsigned short height,
	                                          const std::vector<unsigned char>& pixels)
	{
		std::vector<unsigned char> tga;
		// TGA header (18 bytes)
		tga.push_back(0);                                         // ID length
		tga.push_back(0);                                         // Colormap type (none)
		tga.push_back(2);                                         // Data type (uncompressed true-color)
		tga.push_back(0); tga.push_back(0);                      // Colormap origin
		tga.push_back(0); tga.push_back(0);                      // Colormap length
		tga.push_back(0);                                         // Colormap depth
		tga.push_back(0); tga.push_back(0);                      // X origin
		tga.push_back(0); tga.push_back(0);                      // Y origin
		tga.push_back(static_cast<unsigned char>(width & 0xFF));  // Width low
		tga.push_back(static_cast<unsigned char>(width >> 8));    // Width high
		tga.push_back(static_cast<unsigned char>(height & 0xFF)); // Height low
		tga.push_back(static_cast<unsigned char>(height >> 8));   // Height high
		tga.push_back(32);                                        // Bits per pixel
		tga.push_back(0x28);                                      // Flags: top-left origin + 8 alpha bits
		// Pixel data (BGRA)
		tga.insert(tga.end(), pixels.begin(), pixels.end());
		return tga;
	}

	// Build a 2x2 red TGA (all pixels = red with full alpha)
	static std::vector<unsigned char> makeRedTga()
	{
		// 4 pixels * 4 bytes (BGRA) = 16 bytes
		std::vector<unsigned char> pixels(16);
		for (int i = 0; i < 4; ++i)
		{
			pixels[i * 4 + 0] = 0;   // B
			pixels[i * 4 + 1] = 0;   // G
			pixels[i * 4 + 2] = 255; // R
			pixels[i * 4 + 3] = 255; // A
		}
		return makeTga(2, 2, pixels);
	}
};

TEST(RgtFileEmpty, DefaultConstructor)
{
	CRgtFile rgt;
	EXPECT_EQ(rgt.GetImageFormat(), CRgtFile::IF_None);
	// DXT codec stubs are set by default
	EXPECT_TRUE(rgt.isDXTCodecPresent());
}

TEST_F(RgtFileTest, LoadMinimalTga)
{
	auto tga = makeRedTga();
	std::unique_ptr<IFileStore::IStream> stream(CMemoryStore::OpenStreamExt(
	    reinterpret_cast<char*>(tga.data()), static_cast<unsigned long>(tga.size()), false));

	EXPECT_NO_THROW(rgt.LoadTGA(stream.get()));

	EXPECT_EQ(rgt.GetImageFormat(), CRgtFile::IF_Tga);
	EXPECT_EQ(rgt.GetProperty(CRgtFile::IP_Width), 2);
	EXPECT_EQ(rgt.GetProperty(CRgtFile::IP_Height), 2);
}

TEST_F(RgtFileTest, LoadTgaImageProperties)
{
	auto tga = makeRedTga();
	std::unique_ptr<IFileStore::IStream> stream(CMemoryStore::OpenStreamExt(
	    reinterpret_cast<char*>(tga.data()), static_cast<unsigned long>(tga.size()), false));

	rgt.LoadTGA(stream.get());

	EXPECT_EQ(rgt.GetProperty(CRgtFile::IP_Width), 2);
	EXPECT_EQ(rgt.GetProperty(CRgtFile::IP_Height), 2);
}

TEST_F(RgtFileTest, LoadTga24Bit)
{
	// 24-bit TGA (no alpha)
	std::vector<unsigned char> tga;
	tga.push_back(0); tga.push_back(0); tga.push_back(2); // header
	tga.push_back(0); tga.push_back(0);                    // colormap origin
	tga.push_back(0); tga.push_back(0);                    // colormap length
	tga.push_back(0);                                       // colormap depth
	tga.push_back(0); tga.push_back(0);                    // X origin
	tga.push_back(0); tga.push_back(0);                    // Y origin
	tga.push_back(1); tga.push_back(0);                    // Width = 1
	tga.push_back(1); tga.push_back(0);                    // Height = 1
	tga.push_back(24);                                      // BPP
	tga.push_back(0x20);                                    // Flags: top-left
	// 1 pixel BGR
	tga.push_back(0);   // B
	tga.push_back(255); // G
	tga.push_back(0);   // R

	bool is32Bit = true;
	std::unique_ptr<IFileStore::IStream> stream(CMemoryStore::OpenStreamExt(
	    reinterpret_cast<char*>(tga.data()), static_cast<unsigned long>(tga.size()), false));

	rgt.LoadTGA(stream.get(), false, &is32Bit);

	EXPECT_EQ(rgt.GetImageFormat(), CRgtFile::IF_Tga);
	EXPECT_EQ(rgt.GetProperty(CRgtFile::IP_Width), 1);
	EXPECT_EQ(rgt.GetProperty(CRgtFile::IP_Height), 1);
	EXPECT_FALSE(is32Bit);
}

TEST_F(RgtFileTest, SaveTgaRoundTrip)
{
	// Load a TGA
	auto tga = makeRedTga();
	std::unique_ptr<IFileStore::IStream> stream(CMemoryStore::OpenStreamExt(
	    reinterpret_cast<char*>(tga.data()), static_cast<unsigned long>(tga.size()), false));
	rgt.LoadTGA(stream.get());

	// Save as TGA
	auto* pOut = CMemoryStore::OpenOutputStreamExt();
	rgt.SaveTGA(pOut);
	EXPECT_GT(pOut->GetDataLength(), 0u);

	// Reload the saved TGA
	CRgtFile rgt2;
	std::unique_ptr<IFileStore::IStream> stream2(CMemoryStore::OpenStreamExt(
	    const_cast<char*>(pOut->GetData()), pOut->GetDataLength(), false));
	rgt2.LoadTGA(stream2.get());

	EXPECT_EQ(rgt2.GetProperty(CRgtFile::IP_Width), 2);
	EXPECT_EQ(rgt2.GetProperty(CRgtFile::IP_Height), 2);

	delete pOut;
}

TEST_F(RgtFileTest, SaveAndLoadRgtChunky)
{
	// Load a TGA, save as RGT chunky, reload
	auto tga = makeRedTga();
	std::unique_ptr<IFileStore::IStream> stream(CMemoryStore::OpenStreamExt(
	    reinterpret_cast<char*>(tga.data()), static_cast<unsigned long>(tga.size()), false));
	rgt.LoadTGA(stream.get());

	// Save as RGT
	auto* pOut = CMemoryStore::OpenOutputStreamExt();
	rgt.Save(pOut);
	EXPECT_GT(pOut->GetDataLength(), 0u);

	// Reload the RGT
	CRgtFile rgt2;
	std::unique_ptr<IFileStore::IStream> stream2(CMemoryStore::OpenStreamExt(
	    const_cast<char*>(pOut->GetData()), pOut->GetDataLength(), false));
	rgt2.Load(stream2.get());

	EXPECT_EQ(rgt2.GetImageFormat(), CRgtFile::IF_Tga);
	EXPECT_EQ(rgt2.GetProperty(CRgtFile::IP_Width), 2);
	EXPECT_EQ(rgt2.GetProperty(CRgtFile::IP_Height), 2);

	delete pOut;
}

TEST_F(RgtFileTest, MipLevelInfo)
{
	auto tga = makeRedTga();
	std::unique_ptr<IFileStore::IStream> stream(CMemoryStore::OpenStreamExt(
	    reinterpret_cast<char*>(tga.data()), static_cast<unsigned long>(tga.size()), false));
	rgt.LoadTGA(stream.get());

	EXPECT_GE(rgt.GetProperty(CRgtFile::IP_MipLevelCount), 1);
	EXPECT_EQ(rgt.GetProperty(CRgtFile::IP_MipLevel), 0);
}

TEST_F(RgtFileTest, DxtCodecPresentByDefault)
{
	// Constructor sets squish stub functions
	EXPECT_TRUE(rgt.isDXTCodecPresent());
}
