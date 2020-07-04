#include "ShadowMapFBO.h"

#include "glad/glad.h"

#include <iostream>


ShadowMapFBO::ShadowMapFBO() : _fboID(0) {}

ShadowMapFBO::~ShadowMapFBO() { _clear(); }

void ShadowMapFBO::_clear()
{
    if (_fboID != 0) {
        glDeleteFramebuffers(1, &_fboID);
    }

    _depthBufferTexture.reset();
}

void ShadowMapFBO::Init(std::uint32_t width, std::uint32_t height)
{
    _width = width;
    _height = height;

    _depthBufferTexture = std::make_shared<Texture>(width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT16, GL_FLOAT);

    glGenFramebuffers(1, &_fboID);
    glBindFramebuffer(GL_FRAMEBUFFER, _fboID);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depthBufferTexture->GetID(), 0);

    // No need to draw anything in the color buffer
    glDrawBuffer(GL_NONE);

    // Always check that our framebuffer is ok
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Shadow map buffer is incomplete\n";
}

void ShadowMapFBO::Resize(std::uint32_t width, std::uint32_t height)
{
    if (width != _width || height != _height) {
        _clear();

        Init(width, height);
    }
}