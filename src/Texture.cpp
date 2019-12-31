#include "Texture.hpp"

#include <Log.hpp>
#include <Utils.hpp>

#include <iostream>

#include <stb/stb_image.h>

Texture::Texture(const std::string& filename)
{
    Load(filename);
}

Texture::Texture(unsigned char* data, glm::ivec2 size, int comp)
{
    Load(data, size, comp);
}

Texture::Texture(GLuint&& id, glm::ivec2 size)
{
    std::swap(_mglID, id);
    _mSize = size;
    if (_mglID >= 0)
    {
        _mLoaded = true;
        LogLoad("Loaded from existing ID %u", _mglID);
    }
}

Texture::Texture(Texture&& rhs)
{
    std::swap(_mLoaded, rhs._mLoaded);
    std::swap(_mglID, rhs._mglID);
}

Texture::~Texture()
{
    if (_mglID > 0)
    {
        glDeleteTextures(1, &_mglID);
    }
}

bool Texture::Load(const std::string& filename)
{
    // Load img
    int            bpp;
    unsigned char* image = nullptr;
    const auto& paths = Utils::GetResourcePaths();
    FILE * file = NULL;

    LogInfo("Loading: [%s]\n", filename);

    for (const std::string& p : paths) {
        std::string fullFilename = p + "/" + filename;
        file = fopen(fullFilename.c_str(), "rb");

        if (!file) continue;

        LogLoad("Loaded:  [%s]\n", fullFilename);

        stbi_set_flip_vertically_on_load(true);

        image = stbi_load_from_file(file, &_mSize.x, &_mSize.y, &bpp, STBI_rgb_alpha);
        fclose(file);
    }

    if (!image) {
        LogError("Failed to load [%s]\n", filename);
        return false;
    }

    // Load from buffer
    Load(image, _mSize, bpp);

    // Free the Image
    stbi_image_free(image);

    return _mLoaded;
}

bool Texture::Load(unsigned char* buffer, glm::ivec2 size, int comp)
{
    _mLoaded = false;

    if (_mglID > 0)
    {
        glDeleteTextures(1, &_mglID);
        _mglID = 0;
    }

    glGenTextures(1, &_mglID);

    if (0 == _mglID)
    {
        LogError("Failed to create GL Texture\n");
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, _mglID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    LogVerbose("Binding texture to id %u\n", _mglID);

    stbi_set_flip_vertically_on_load(true);

    stbi_uc* image_data = nullptr;
    if (size.y == 0)
    {
        image_data = stbi_load_from_memory(buffer, size.x, &size.x, &size.y, &comp, STBI_rgb_alpha);
    }
    else
    {
        image_data = stbi_load_from_memory(buffer, size.x * size.y, &size.x, &size.y, &comp, STBI_rgb_alpha);
    }

    if (!image_data)
    {
        LogError("Image not loaded. %s\n", stbi_failure_reason());
    }

    // texture wrapping params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // texture filtering params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Create the image
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Bind texture
    glBindTexture(GL_TEXTURE_2D, 0);
    _mLoaded = true;

    return _mLoaded;
}

void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, _mglID);
}