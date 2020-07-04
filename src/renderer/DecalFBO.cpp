#include "DecalFBO.h"

#include "glad/glad.h"

#include <iostream>


DecalFBO::DecalFBO() : _fboID(0) { }

DecalFBO::~DecalFBO() { _clear(); }

void DecalFBO::Init(std::uint32_t width, std::uint32_t height, std::uint32_t albedoTextureID)
{
    // Decal Depth
    _width = width;
    _height = height;

    _depthBufferTexture = std::make_shared<Texture>(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT16, GL_FLOAT);

    glGenFramebuffers(1, &_depthFBOID);
    glBindFramebuffer(GL_FRAMEBUFFER, _depthFBOID);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depthBufferTexture->GetID(), 0);

    // No need to draw anything in the color buffer
    glDrawBuffer(GL_NONE);

    // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Decal depth buffer is incomplete\n";

    // Decal projection
    _albedoTextureID = albedoTextureID;

    glGenFramebuffers(1, &_fboID);
    glBindFramebuffer(GL_FRAMEBUFFER, _fboID);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, albedoTextureID, 0);

    unsigned int attachments[]{ GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, attachments);

    // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Decal projection buffer is incomplete\n";
}

void DecalFBO::Resize(std::uint32_t width, std::uint32_t height, std::uint32_t albedoTextureID)
{
    if (width != _width || height != _height) {
        _clear();

        Init(width, height, albedoTextureID);
    }
}

void DecalFBO::_clear()
{
    if (_fboID != 0) {
        glDeleteFramebuffers(1, &_fboID);
        glDeleteFramebuffers(1, &_depthFBOID);
    }
}