#include <gtest/gtest.h>
#include "module/CModuleFile.h"
#include "core/Exception.h"
#include <cstring>
#include <filesystem>
#include <fstream>
#include <process.h>

class ModuleFileTest : public ::testing::Test {
protected:
    std::filesystem::path tempDir;

    void SetUp() override {
        tempDir = std::filesystem::temp_directory_path() /
            ("mod_test_" + std::to_string(_getpid()) + "_" + std::to_string(reinterpret_cast<uintptr_t>(this)));
        std::filesystem::create_directories(tempDir);
    }

    void TearDown() override {
        std::filesystem::remove_all(tempDir);
    }

    std::string writeModuleFile(const std::string& name, const std::string& content) {
        auto path = (tempDir / name).string();
        std::ofstream ofs(path, std::ios::binary);
        ofs << content;
        ofs.close();
        return path;
    }
};

TEST_F(ModuleFileTest, DefaultConstructor) {
    CModuleFile mod;
    EXPECT_EQ(mod.GetModuleType(), CModuleFile::MT_DawnOfWar);
}

TEST_F(ModuleFileTest, SetAndGetLocale) {
    CModuleFile mod;
    mod.SetLocale("English");
    EXPECT_STREQ(mod.GetLocale(), "English");

    mod.SetLocale("French");
    EXPECT_STREQ(mod.GetLocale(), "French");
}

TEST_F(ModuleFileTest, LoadNonexistentFileThrows) {
    CModuleFile mod;
    CRainmanException* caught = nullptr;
    try {
        mod.LoadModuleFile("C:\\nonexistent\\path\\fake.module");
    } catch (CRainmanException* ex) {
        caught = ex;
    }
    ASSERT_NE(caught, nullptr);
    caught->destroy();
}

TEST_F(ModuleFileTest, LoadEmptyFileClassifiesAsCohEarly) {
    // An empty .module file gets classified based on default heuristics
    // (no data sources and no folders/archives/required = CohEarly wins)
    auto path = writeModuleFile("empty.module", "");

    CModuleFile mod;
    mod.LoadModuleFile(path.c_str());
    EXPECT_EQ(mod.GetModuleType(), CModuleFile::MT_CompanyOfHeroesEarly);
}

TEST_F(ModuleFileTest, LoadDawnOfWarModule) {
    std::string content =
        "[global]\r\n"
        "UIName = Test Mod\r\n"
        "Description = A test mod\r\n"
        "DllName = TestDll\r\n"
        "ModFolder = TestFolder\r\n"
        "TextureFE = texturefe\r\n"
        "TextureIcon = textureicon\r\n"
        "ModVersion = 2.1.3\r\n"
        "DataFolder.1 = DataFolder1\r\n"
        "DataFolder.2 = DataFolder2\r\n"
        "ArchiveFile.1 = archive1\r\n"
        "RequiredMod.1 = RequiredBase\r\n"
        "CompatableMod.1 = CompatMod1\r\n";

    auto path = writeModuleFile("test.module", content);

    CModuleFile mod;
    mod.LoadModuleFile(path.c_str());

    EXPECT_STREQ(mod.GetUiName(), "Test Mod");
    EXPECT_STREQ(mod.GetDescription(), "A test mod");
    EXPECT_STREQ(mod.GetDllName(), "TestDll");
    EXPECT_STREQ(mod.GetModFolder(), "TestFolder");
    EXPECT_STREQ(mod.GetTextureFe(), "texturefe");
    EXPECT_STREQ(mod.GetTextureIcon(), "textureicon");
    EXPECT_EQ(mod.GetVersionMajor(), 2);
    EXPECT_EQ(mod.GetVersionMinor(), 1);
    EXPECT_EQ(mod.GetVersionRevision(), 3);
    EXPECT_EQ(mod.GetFolderCount(), 2u);
    EXPECT_EQ(mod.GetArchiveCount(), 1u);
    EXPECT_EQ(mod.GetRequiredCount(), 1u);
    EXPECT_EQ(mod.GetCompatibleCount(), 1u);
    EXPECT_EQ(mod.GetModuleType(), CModuleFile::MT_DawnOfWar);
}

TEST_F(ModuleFileTest, LoadDawnOfWarFolderDetails) {
    std::string content =
        "[global]\r\n"
        "UIName = FolderTest\r\n"
        "TextureFE = t\r\n"
        "TextureIcon = t\r\n"
        "DataFolder.1 = MyData\r\n"
        "DataFolder.5 = OtherData\r\n";

    auto path = writeModuleFile("folders.module", content);

    CModuleFile mod;
    mod.LoadModuleFile(path.c_str());

    EXPECT_EQ(mod.GetFolderCount(), 2u);
    auto* f0 = mod.GetFolder(0);
    EXPECT_STREQ(f0->GetName(), "MyData");
    EXPECT_EQ(f0->GetNumber(), 1);

    auto* f1 = mod.GetFolder(1);
    EXPECT_STREQ(f1->GetName(), "OtherData");
    EXPECT_EQ(f1->GetNumber(), 5);
}

TEST_F(ModuleFileTest, LoadArchiveAppendsSga) {
    std::string content =
        "[global]\r\n"
        "UIName = ArchTest\r\n"
        "TextureFE = t\r\n"
        "TextureIcon = t\r\n"
        "ArchiveFile.1 = W40k\r\n";

    auto path = writeModuleFile("arch.module", content);

    CModuleFile mod;
    mod.LoadModuleFile(path.c_str());

    EXPECT_EQ(mod.GetArchiveCount(), 1u);
    // ArchiveFile values get ".sga" appended
    EXPECT_STREQ(mod.GetArchive(0)->GetFileName(), "W40k.sga");
}

TEST_F(ModuleFileTest, LoadRequiredMod) {
    std::string content =
        "[global]\r\n"
        "UIName = ReqTest\r\n"
        "TextureFE = t\r\n"
        "TextureIcon = t\r\n"
        "RequiredMod.1 = W40k\r\n"
        "RequiredMod.2 = WXP\r\n";

    auto path = writeModuleFile("req.module", content);

    CModuleFile mod;
    mod.LoadModuleFile(path.c_str());

    EXPECT_EQ(mod.GetRequiredCount(), 2u);
    EXPECT_STREQ(mod.GetRequired(0)->GetFileName(), "W40k");
    EXPECT_STREQ(mod.GetRequired(1)->GetFileName(), "WXP");
}

TEST_F(ModuleFileTest, VersionParsingMajorOnly) {
    std::string content =
        "[global]\r\n"
        "UIName = Test\r\n"
        "TextureFE = t\r\n"
        "TextureIcon = t\r\n"
        "ModVersion = 5\r\n"
        "DataFolder.1 = Data\r\n";

    auto path = writeModuleFile("ver.module", content);

    CModuleFile mod;
    mod.LoadModuleFile(path.c_str());

    EXPECT_EQ(mod.GetVersionMajor(), 5);
    EXPECT_EQ(mod.GetVersionMinor(), 0);
    EXPECT_EQ(mod.GetVersionRevision(), 0);
}

TEST_F(ModuleFileTest, VersionParsingMajorMinor) {
    std::string content =
        "[global]\r\n"
        "UIName = Test\r\n"
        "TextureFE = t\r\n"
        "TextureIcon = t\r\n"
        "ModVersion = 3.7\r\n"
        "DataFolder.1 = Data\r\n";

    auto path = writeModuleFile("ver2.module", content);

    CModuleFile mod;
    mod.LoadModuleFile(path.c_str());

    EXPECT_EQ(mod.GetVersionMajor(), 3);
    EXPECT_EQ(mod.GetVersionMinor(), 7);
    EXPECT_EQ(mod.GetVersionRevision(), 0);
}

TEST_F(ModuleFileTest, SgaOutputVersionDoW) {
    std::string content =
        "[global]\r\n"
        "UIName = Test\r\n"
        "TextureFE = t\r\n"
        "TextureIcon = t\r\n"
        "DataFolder.1 = Data\r\n";

    auto path = writeModuleFile("dow.module", content);

    CModuleFile mod;
    mod.LoadModuleFile(path.c_str());
    EXPECT_EQ(mod.GetModuleType(), CModuleFile::MT_DawnOfWar);
    EXPECT_EQ(mod.GetSgaOutputVersion(), 2);
}

TEST_F(ModuleFileTest, CommentsAreIgnored) {
    std::string content =
        "[global]\r\n"
        "; This is a comment\r\n"
        "UIName = Commented Mod\r\n"
        "TextureFE = t\r\n"
        "TextureIcon = t\r\n"
        "DataFolder.1 = Data\r\n";

    auto path = writeModuleFile("comment.module", content);

    CModuleFile mod;
    mod.LoadModuleFile(path.c_str());

    EXPECT_STREQ(mod.GetUiName(), "Commented Mod");
}

TEST_F(ModuleFileTest, FileMapNameFromFilename) {
    std::string content =
        "[global]\r\n"
        "UIName = MapTest\r\n"
        "TextureFE = t\r\n"
        "TextureIcon = t\r\n"
        "DataFolder.1 = Data\r\n";

    auto path = writeModuleFile("mymod.module", content);

    CModuleFile mod;
    mod.LoadModuleFile(path.c_str());

    // FileMapName is derived from the filename without extension
    EXPECT_STREQ(mod.GetFileMapName(), "mymod");
}

TEST_F(ModuleFileTest, SettersWorkAfterLoad) {
    std::string content =
        "[global]\r\n"
        "UIName = Original\r\n"
        "TextureFE = t\r\n"
        "TextureIcon = t\r\n"
        "DataFolder.1 = Data\r\n";

    auto path = writeModuleFile("set.module", content);

    CModuleFile mod;
    mod.LoadModuleFile(path.c_str());

    mod.SetName("NewName");
    EXPECT_STREQ(mod.GetName(), "NewName");

    mod.SetDescription("New desc");
    EXPECT_STREQ(mod.GetDescription(), "New desc");

    mod.SetVersionMajor(9);
    EXPECT_EQ(mod.GetVersionMajor(), 9);
}
