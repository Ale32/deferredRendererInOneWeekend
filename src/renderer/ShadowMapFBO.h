#pragma once

#include <memory>

#include "Texture.h"


/**
 *	Shadow FBO class to manage the frame buffer for the spot light's shadow map
 */
class ShadowMapFBO
{
private:
	std::uint32_t _fboID;

	std::shared_ptr<Texture> _depthBufferTexture;

	std::uint32_t _width = 0;
	std::uint32_t _height = 0;

	void _clear();

public:
	ShadowMapFBO();
	~ShadowMapFBO();

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

	const Texture& GetDepthBuffer() const { return *_depthBufferTexture; };

	std::uint32_t GetWidth() const { return _width; }
	std::uint32_t GetHeight() const { return _height; }
};