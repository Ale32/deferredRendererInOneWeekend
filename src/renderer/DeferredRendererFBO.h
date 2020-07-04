#pragma once

#include <memory>

#include "Texture.h"

/**
 *	Deferred rendering class to manage the frame buffer and the render targets
 */
class DeferredRenderingFBO
{
private:
	std::uint32_t _fboID;
	std::uint32_t _rboID;

	std::shared_ptr<Texture> _diffuseBuffer;
	std::shared_ptr<Texture> _positionBuffer;
	std::shared_ptr<Texture> _normalBuffer;
	std::shared_ptr<Texture> _depthBuffer;

	std::uint32_t _width = 0;
	std::uint32_t _height = 0;

	void _clear();

public:
	DeferredRenderingFBO();
	~DeferredRenderingFBO();

	/**
	 *	Creates the FBO and its textures with the given width and height
	 *
	 *	\param	width	The width of the FBO
	 *	\param	height	The height of the FBO
	 */
	void Init(std::uint32_t width, std::uint32_t height);

	/**
	 *	Returns the FBO id
	 *
	 *	\return The id for this FBO
	 */
	inline std::uint32_t GetID() const { return _fboID; };

	/**
	 *	Resizes the FBO
	 *
	 *	\param	width	The new width of the FBO
	 *	\param	height	The new height of the FBO
	 */
	void Resize(std::uint32_t width, std::uint32_t height);

	// Getters
	const Texture& GetDiffuseBuffer() const { return *_diffuseBuffer; };
	const Texture& GetPositionBuffer() const { return *_positionBuffer; };
	const Texture& GetNormalBuffer() const { return *_normalBuffer; };
	const Texture& GetDepthBuffer() const { return *_depthBuffer; };

	std::uint32_t GetWidth() const { return _width; }
	std::uint32_t GetHeight() const { return _height; }
};