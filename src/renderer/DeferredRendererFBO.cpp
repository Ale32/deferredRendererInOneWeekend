#include "DeferredRendererFBO.h"

#include "glad/glad.h"

#include <iostream>

DeferredRenderingFBO::DeferredRenderingFBO() : _fboID(0) {}

DeferredRenderingFBO::~DeferredRenderingFBO() { _clear(); }

void DeferredRenderingFBO::Init(std::uint32_t width, std::uint32_t height)
{
	_width = width;
	_height = height;

	_diffuseBuffer = std::make_shared<Texture>(width, height, GL_RGBA, GL_RGBA16F, GL_UNSIGNED_BYTE);
	_positionBuffer = std::make_shared<Texture>(width, height, GL_RGB, GL_RGB16F, GL_UNSIGNED_BYTE);
	_normalBuffer = std::make_shared<Texture>(width, height, GL_RGB, GL_RGB16F, GL_UNSIGNED_BYTE);

	glGenFramebuffers(1, &_fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, _fboID);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _diffuseBuffer->GetID(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _positionBuffer->GetID(), 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, _normalBuffer->GetID(), 0);

	// RBO for depth
	glGenRenderbuffers(1, &_rboID);
	glBindRenderbuffer(GL_RENDERBUFFER, _rboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rboID);

	unsigned int attachments[]{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Deferred rendering buffer is incomplete\n";
}

void DeferredRenderingFBO::Resize(std::uint32_t width, std::uint32_t height)
{
	if (width != _width || height != _height) {
		_clear();

		Init(width, height);
	}
}

void DeferredRenderingFBO::_clear()
{
	if (_fboID != 0) {
		glDeleteFramebuffers(1, &_fboID);
		glDeleteRenderbuffers(1, &_rboID);
	}

	_diffuseBuffer.reset();
	_positionBuffer.reset();
	_normalBuffer.reset();
}