#pragma once

#include <memory>

#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Decal.h"

#include "DeferredRendererFBO.h"
#include "ShadowMapFBO.h"
#include "DecalFBO.h"


class Renderer {
private:
    int _viewportX;
    int _viewportY;

	int _qtOpenGLWidgetFBO;

    std::unique_ptr<Camera> _camera;
	    
	// Primitives to render in base pass
	std::vector<std::unique_ptr<class Primitive>> _primitives;

	// Scene's point lights
	std::vector<std::unique_ptr<class PointLight>> _pointLights;
	std::unique_ptr<Shader> _pointLightsShader;

	// Scene's spot light
	std::unique_ptr<class SpotLight> _spotLight;
	std::unique_ptr<Shader> _spotLightShader;
	std::unique_ptr<Shader> _shadowDepthShader;

	// The screen quad primitive
	std::unique_ptr<class Primitive> _screenQuad;

	// Decal shader
	std::unique_ptr<Decal> _decal;
	std::unique_ptr<Shader> _decalShader;

	DeferredRenderingFBO _deferredFBO;
	ShadowMapFBO _shadowMapFBO;
	DecalFBO _decalFBO;

    void _clear();

	/** Creates the scene's point lights and initialize some constant uniforms */
	void _setupPointLights();

	/** Creates the scene's spot light and initialize some constant uniforms */
	void _setupSpotLight();

	/** Renders primitive to the GBuffer */
    void _basePass();

	/** Renders the point lighting pass */
    void _pointLightPass();

	/** Renders the spot lighting pass */
	void _spotLightPass();

	/** Renders the spot light shadow */
	void _spotLightShadowPass();

	/** Render decal */
	void _renderDecal();

public:
    Renderer();
    ~Renderer();

    void init();
    void update(int w, int h);
    void render();

    // Camera events
    void MoveCamera(CameraMovement movement, float deltaTime);
    void RotateCamera(float hMovement, float vMovement);
    void ZoomCamera(float zoom);
};