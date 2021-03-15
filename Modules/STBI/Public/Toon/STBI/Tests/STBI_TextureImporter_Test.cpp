#include <Toon/STBI/STBITextureImporter.hpp>

#include <gtest/gtest.h>

#include "test.h"

static Toon::STBI::STBITextureImporter _TextureImporter;

TEST(LoadFromFile, PNG_RGB) {
    auto data = _TextureImporter.LoadFromFile("test_rgb.png");
    ASSERT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), Toon::uvec2(32, 32));
}

TEST(LoadFromFile, PNG_RGBA) {
    auto data = _TextureImporter.LoadFromFile("test_rgba.png");
    ASSERT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), Toon::uvec2(32, 32));
}

TEST(LoadFromFile, PNG_Grey) {
    auto data = _TextureImporter.LoadFromFile("test_grey.png");
    ASSERT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), Toon::uvec2(32, 32));
}

TEST(LoadFromFile, JPEG_RGB) {
    auto data = _TextureImporter.LoadFromFile("test_rgb.jpg");
    ASSERT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), Toon::uvec2(32, 32));
}

TEST(LoadFromFile, JPEG_Grey) {
    auto data = _TextureImporter.LoadFromFile("test_grey.jpg");
    ASSERT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), Toon::uvec2(32, 32));
}

TEST(LoadFromFile, BMP_RGB) {
    auto data = _TextureImporter.LoadFromFile("test_rgb.bmp");
    ASSERT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), Toon::uvec2(32, 32));
}

TEST(LoadFromFile, BMP_Grey) {
    auto data = _TextureImporter.LoadFromFile("test_rgb.bmp");
    ASSERT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), Toon::uvec2(32, 32));
}

TEST(LoadFromFile, TGA) {
    auto data = _TextureImporter.LoadFromFile("test_rgb.tga");
    ASSERT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), Toon::uvec2(32, 32));
}
TEST(LoadFromFile, GIF_RGB) {
    auto data = _TextureImporter.LoadFromFile("test_rgb.gif");
    ASSERT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), Toon::uvec2(32, 32));
}

TEST(LoadFromFile, GIF_RGBA) {
    auto data = _TextureImporter.LoadFromFile("test_rgba.gif");
    ASSERT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), Toon::uvec2(32, 32));
}

TEST(LoadFromFile, GIF_Grey) {
    auto data = _TextureImporter.LoadFromFile("test_grey.gif");
    ASSERT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), Toon::uvec2(32, 32));
}

TEST(LoadFromFile, NoFilename) {
    auto data = _TextureImporter.LoadFromFile("");
    EXPECT_TRUE(data == nullptr);
}

TEST(LoadFromFile, FileNotFound) {
    auto data = _TextureImporter.LoadFromFile("asdf.png");
    EXPECT_TRUE(data == nullptr);
}

TEST(LoadFromMemory, PNG_RGB) {
    auto data = _TextureImporter.LoadFromMemory(___Modules_STBI_Tests_test_png, ___Modules_STBI_Tests_test_png_len);
    ASSERT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), Toon::uvec2(32, 32));
}

TEST(LoadFromMemory, JPEG_RGB) {
    auto data = _TextureImporter.LoadFromMemory(___Modules_STBI_Tests_test_jpg, ___Modules_STBI_Tests_test_jpg_len);
    ASSERT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), Toon::uvec2(32, 32));
}

TEST(LoadFromMemory, BMP_RGB) {
    auto data = _TextureImporter.LoadFromMemory(___Modules_STBI_Tests_test_bmp, ___Modules_STBI_Tests_test_bmp_len);
    ASSERT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), Toon::uvec2(32, 32));
}

TEST(LoadFromMemory, TGA_RGB) {
    auto data = _TextureImporter.LoadFromMemory(___Modules_STBI_Tests_test_tga, ___Modules_STBI_Tests_test_tga_len);
    ASSERT_FALSE(data == nullptr);
    EXPECT_EQ(data->GetSize(), Toon::uvec2(32, 32));
}

TEST(LoadFromMemory, NullInput) {
    auto data = _TextureImporter.LoadFromMemory(nullptr, 0);
    EXPECT_TRUE(data == nullptr);
}

int main(int argc, char ** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}