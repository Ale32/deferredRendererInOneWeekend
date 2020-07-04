#pragma once

#include <memory>

#include "Texture.h"

/**
 *	Decal FBO class to manage the frame buffer for the decal projection onto the GBuffer albedo
 */
class DecalFBO
{
private:
	std::uint32_t _fboID;
	std::uint32_t _depthFBOID;

	std::uint32_t _albedoTextureID;

	std::shared_ptr<Texture> _depthBufferTexture;

	std::uint32_t _width = 0;
	std::uint32_t _height = 0;

	void _clear();

public:
	DecalFBO();
	~DecalFBO();

	/**
	 *	Creates the FBO
	 */
	void Init(std::uint32_t width, std::uint32_t height, std::uint32_t albedoTextureID);

	/**
	 *	Returns the FBO id
	 */
	inline std::uint32_t GetProjID() const { return _fboID; };
	inline std::uint32_t GetDepthID() const { return _depthFBOID; };

	/**
	 *	Resizes the FBO
	 */
	void Resize(std::uint32_t width, std::uint32_t height, std::uint32_t albedoTextureID);

	const std::uint32_t GetAlbedoTextureID() const { return _albedoTextureID; };

	const Texture& GetDepthBuffer() const { return *_depthBufferTexture; };

	std::uint32_t GetWidth() const { return _width; }
	std::uint32_t GetHeight() const { return _height; }
};