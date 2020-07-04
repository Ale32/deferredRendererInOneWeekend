#include "Texture.h"

#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


Texture::Texture(const std::string& path)
    : _rendererID(0), _filePath(path), _localBuffer(nullptr), _width(0), _height(0), _BPP(0)
{
    stbi_set_flip_vertically_on_load(1);
    _localBuffer = stbi_load(path.c_str(), &_width, &_height, &_BPP, 4);

    glGenTextures(1, &_rendererID);
    glBindTexture(GL_TEXTURE_2D, _rendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _localBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (_localBuffer)
        stbi_image_free(_localBuffer);
}

Texture::Texture(int width, int height, int format, int internalFormat, int type, bool mipmap)
    : _localBuffer(nullptr),
    _width(width),
    _height(height),
    _BPP(0)
{
    glGenTextures(1, &_rendererID);
    glBindTexture(GL_TEXTURE_2D, _rendererID);

    if (internalFormat == GL_REPEAT)
        internalFormat = format;

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, format, type, _localBuffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    if (mipmap) {
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &_rendererID);
}

void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, _rendererID);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
