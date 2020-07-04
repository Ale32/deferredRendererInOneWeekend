#pragma once

#include <string>


class Texture
{
private:
    std::uint32_t _rendererID;
    std::string _filePath;

    unsigned char* _localBuffer;
    int _width;
    int _height;
    int _BPP;

public:
    /**
     *  Creates a texture loading it from file
     */
    Texture(const std::string& path);

    /**
     *  Creates a texture to use as a buffer
     *
     *  \param  width           Width of the texture
     *  \param  height          Height of the texture
     *  \param  format          Pixel data format (GL_RGB, GL_RGBA, etc)
     *  \param  internalFormat  The format of the data stored in the texture
     *  \param  mipmap          Wether or not to use the mipmap
     */
    Texture(int width, int height, int format, int internalFormat, int type, bool mipmap = false);

    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline std::uint32_t GetID() const { return _rendererID; }
    inline int GetWidth() const { return _width; }
    inline int GetHeight() const { return _height; }
};