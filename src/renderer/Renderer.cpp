#include "Renderer.h"

#include <iostream>
#include "glad/glad.h"

#include "primitives/Sphere.h"
#include "primitives/ScreenQuad.h"
#include "primitives/Cube.h"
#include "primitives/Plane.h"

#include "Light.h"


Renderer::Renderer() {}

Renderer::~Renderer() {}

void Renderer::init()
{
    gladLoadGL();
    std::cout << "OpenGL version supported: " << glGetString(GL_VERSION) << std::endl;

    _clear();

    // Initial settings that we share between all passes - mostly
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Create camera
    _camera = std::make_unique<Camera>(glm::vec3(0.f, 1.f, 5.f));

    // Plane as a ground
    std::unique_ptr<Primitive> planePrim = std::make_unique<Plane>(5.f, 5.f);

    std::shared_ptr<Shader> planeShader = std::make_shared<Shader>("shaders/deferredBasePass.vs", "shaders/deferredBasePass.fs");
    planeShader->Bind();
    glm::mat4 planeModelMatrix = glm::rotate(glm::mat4(1.f), glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
    planePrim->SetModelMatrix(planeModelMatrix);
    planeShader->SetUniformMat4f("uModel", planeModelMatrix);
    planeShader->SetUniform3f("uColor", glm::vec3(0.9f, 0.9f, 0.9f));
    planeShader->SetUniform1f("uSpecular", 1.f);

    planePrim->SetShader(planeShader);
    _primitives.push_back(std::move(planePrim));

    // Sphere
    std::unique_ptr<Primitive> spherePrim = std::make_unique<Sphere>();

    std::shared_ptr<Shader> sphereShader = std::make_shared<Shader>("shaders/deferredBasePass.vs", "shaders/deferredBasePass.fs");
    sphereShader->Bind();
    glm::mat4 sphereModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.f, 1.f, 0.f));
    spherePrim->SetModelMatrix(sphereModelMatrix);
    sphereShader->SetUniformMat4f("uModel", sphereModelMatrix);
    sphereShader->SetUniform3f("uColor", glm::vec3(1.f, 0.f, 0.f));
    sphereShader->SetUniform1f("uSpecular", 30.0f);

    spherePrim->SetShader(sphereShader);
    _primitives.push_back(std::move(spherePrim));

    // Cube
    std::unique_ptr<Primitive> cubePrim = std::make_unique<Cube>();

    std::shared_ptr<Shader> cubeshader = std::make_shared<Shader>("shaders/deferredBasePass.vs", "shaders/deferredBasePass.fs");
    cubeshader->Bind();
    glm::mat4 cubeModelMatrix = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(1.f, 0.75f, 0.f)), glm::vec3(1.5f));
    cubePrim->SetModelMatrix(cubeModelMatrix);
    cubeshader->SetUniformMat4f("uModel", cubeModelMatrix);
    cubeshader->SetUniform3f("uColor", glm::vec3(0.f, 1.f, 0.f));
    sphereShader->SetUniform1f("uSpecular", 5.0f);

    cubePrim->SetShader(cubeshader);
    _primitives.push_back(std::move(cubePrim));

    // Screen quad primitive creation
    _screenQuad = std::make_unique<ScreenQuad>();

    // Lighting setup
    _setupPointLights();
    _setupSpotLight();

    // Setup shadow map shader
    _shadowDepthShader = std::make_unique<Shader>("shaders/shadowDepth.vs", "shaders/shadowDepth.fs");

    // Decal shader setup
    glm::vec3 decalPos = _camera->GetPosition();
    glm::vec3 decalLookAt = glm::vec3(1.3f, 0.7f, 0.f);
    float decalExtent = 1.f;

    _decal = std::make_unique<Decal>(decalPos, decalLookAt, decalExtent, decalExtent);
    _decal->SetupShader();
}

void Renderer::update(int w, int h)
{
    _viewportX = w;
    _viewportY = h;

    glViewport(0, 0, w, h);

    _deferredFBO.Resize(_viewportX, _viewportY);
    _shadowMapFBO.Resize(_viewportX, _viewportY);
    _decalFBO.Resize(_viewportX, _viewportY, _deferredFBO.GetDiffuseBuffer().GetID());
}

void Renderer::_setupPointLights()
{
    _pointLightsShader = std::make_unique<Shader>("shaders/screenPass.vs", "shaders/pointLights.fs");

    const int numOfLights = 3;

    float influence = 5.f;
    glm::vec3 positions[numOfLights];
    glm::vec3 colors[numOfLights];

    positions[0] = glm::vec3(-3, 5,  0);
    positions[1] = glm::vec3( 0, 5, -3);
    positions[2] = glm::vec3( 0, 5,  0);

    colors[0] = glm::vec3(0.3f, 0.25f, 0.25f);
    colors[1] = glm::vec3(0.1f, 0.1f, 0.05f);
    colors[2] = glm::vec3(0.1f, 0.1f, 0.05f);

    _pointLightsShader->Bind();
    _pointLightsShader->SetUniform1f("pointLights[0].influence", influence);
    _pointLightsShader->SetUniform3f("pointLights[0].position", positions[0]);
    _pointLightsShader->SetUniform3f("pointLights[0].color", colors[0]);

    _pointLightsShader->SetUniform1f("pointLights[1].influence", influence);
    _pointLightsShader->SetUniform3f("pointLights[1].position", positions[1]);
    _pointLightsShader->SetUniform3f("pointLights[1].color", colors[1]);

    _pointLightsShader->SetUniform1f("pointLights[2].influence", influence);
    _pointLightsShader->SetUniform3f("pointLights[2].position", positions[2]);
    _pointLightsShader->SetUniform3f("pointLights[2].color", colors[2]);
}

void Renderer::_setupSpotLight()
{
    _spotLightShader = std::make_unique<Shader>("shaders/screenPass.vs", "shaders/spotLight.fs");
    _spotLight = std::make_unique<SpotLight>(
        10.f, glm::vec3(3.f, 5.f, 3.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(-3.f, -5.f, -3.f), 30.f);

    _spotLightShader->Bind();
    _spotLightShader->SetUniform1f("spotLight.influence", _spotLight->GetInfluence());
    _spotLightShader->SetUniform3f("spotLight.position", _spotLight->GetPosition());
    _spotLightShader->SetUniform3f("spotLight.color", _spotLight->GetColor());
    _spotLightShader->SetUniform3f("spotLight.direction", _spotLight->GetDirection());
    _spotLightShader->SetUniform1f("spotLight.cutOffValue", _spotLight->GetCutOffValue());
}

void Renderer::render()
{
    // Get the QtOpenGLWidget frame buffer id that we need to draw on the widget directly
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &_qtOpenGLWidgetFBO);

    _clear();

    // Render primitives into the GBuffer
    _basePass();

    // Render decals onto the gbuffer
    _renderDecal();

    // Compute point lighting from the GBuffer
    _pointLightPass();

    // Compute spot lighting and shadow
    _spotLightPass();
}

void Renderer::_basePass()
{
    // Bind the deferred rendering targets
    glBindFramebuffer(GL_FRAMEBUFFER, _deferredFBO.GetID());

    glEnable(GL_DEPTH_TEST);

    _clear();

    for (auto& prim : _primitives) {
        prim->GetShader()->Bind();
        prim->GetShader()->SetUniformMat4f("uView", _camera->GetViewMatrix());
        prim->GetShader()->SetUniformMat4f("uProjection", _camera->GetProjectionMatrix(_viewportX, _viewportY));

        prim->Draw();
    }

    // Restore the QtOpenGL widget buffer
    glBindFramebuffer(GL_FRAMEBUFFER, _qtOpenGLWidgetFBO);
}

void Renderer::_pointLightPass()
{
    // Set the widget area as a target
    glBindFramebuffer(GL_FRAMEBUFFER, _qtOpenGLWidgetFBO);

    _clear();

    // Rendering to a quad on the screen
    glDisable(GL_DEPTH_TEST);

    // Binding the FBO
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _deferredFBO.GetPositionBuffer().GetID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _deferredFBO.GetNormalBuffer().GetID());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _deferredFBO.GetDiffuseBuffer().GetID());

    // Bind the shader
    _pointLightsShader->Bind();
    _pointLightsShader->SetUniform1i("gPosition", 0);
    _pointLightsShader->SetUniform1i("gNormal", 1);
    _pointLightsShader->SetUniform1i("gAlbedo", 2);
    _pointLightsShader->SetUniform3f("uViewPosition", _camera->GetPosition());

    // Draw the quad
    _screenQuad->Draw();
    
    // Unbind the textures
    for (int i = 2; i >= 0; --i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Re enable the depth test
    glEnable(GL_DEPTH_TEST);
}

void Renderer::_spotLightPass()
{
    // Render shadows first
    _spotLightShadowPass();

    // Set the widget area as a target
    glBindFramebuffer(GL_FRAMEBUFFER, _qtOpenGLWidgetFBO);

    // Disable clear and enable blend to add this contribute to the one from the point lights
    //_clear();
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    // Rendering to a quad on the screen
    glDisable(GL_DEPTH_TEST);

    // Binding the FBO
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _deferredFBO.GetPositionBuffer().GetID());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _deferredFBO.GetNormalBuffer().GetID());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _deferredFBO.GetDiffuseBuffer().GetID());
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, _shadowMapFBO.GetDepthBuffer().GetID());

    // Bind the shader
    _spotLightShader->Bind();
    _spotLightShader->SetUniform1i("gPosition", 0);
    _spotLightShader->SetUniform1i("gNormal", 1);
    _spotLightShader->SetUniform1i("gAlbedo", 2);
    _spotLightShader->SetUniform3f("uViewPosition", _camera->GetPosition());
    _spotLightShader->SetUniform1i("uShadowMap", 3);
    _spotLightShader->SetUniformMat4f("uLigthMVP", _spotLight->GetProjectionMatrix(_viewportX, _viewportY) * _spotLight->GetViewMatrix());

    // Draw the quad
    _screenQuad->Draw();

    // Unbind the textures
    for (int i = 3; i >= 0; --i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Re enable the depth test
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void Renderer::_spotLightShadowPass()
{
    _shadowDepthShader->Bind();
    _shadowDepthShader->SetUniformMat4f("uViewProjection", _spotLight->GetProjectionMatrix(_viewportX, _viewportY) * _spotLight->GetViewMatrix());

    // Bind the shadow map target
    glBindFramebuffer(GL_FRAMEBUFFER, _shadowMapFBO.GetID());

    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);

    _clear();

    // Drawing the primitives with the shadow map shader
    for (auto& prim : _primitives) {
        _shadowDepthShader->SetUniformMat4f("uModel", prim->GetModelMatrix());
        prim->Draw();
    }

    // Put back the back face culling
    glCullFace(GL_BACK);

    // Restore the QtOpenGL widget buffer
    glBindFramebuffer(GL_FRAMEBUFFER, _qtOpenGLWidgetFBO);
}

void Renderer::_renderDecal()
{
    // Depth pass from the decal point of view
    _shadowDepthShader->Bind();
    _shadowDepthShader->SetUniformMat4f("uViewProjection", _decal->GetProjectionMatrix() * _decal->GetViewMatrix());

    glBindFramebuffer(GL_FRAMEBUFFER, _decalFBO.GetDepthID());

    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);

    _clear();

    for (auto& prim : _primitives) {
        _shadowDepthShader->SetUniformMat4f("uModel", prim->GetModelMatrix());
        prim->Draw();
    }

    // Put back the back face culling
    glCullFace(GL_BACK);

    // Then use the depth for the decal projection

    // No need to clear because we are writing on top of the GBuffer albedo
    //_clear();

    _decal->UseShader(_decalFBO.GetProjID(), _deferredFBO.GetPositionBuffer().GetID(), _decalFBO.GetDepthBuffer().GetID());

    // Draw the quad
    _screenQuad->Draw();

    // Unbind the textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Restore the QtOpenGL widget buffer
    glBindFramebuffer(GL_FRAMEBUFFER, _qtOpenGLWidgetFBO);
}

void Renderer::_clear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::MoveCamera(CameraMovement movement, float deltaTime)
{
    _camera->Move(movement, deltaTime);
}

void Renderer::RotateCamera(float hMovement, float vMovement)
{
    _camera->LookAround(hMovement, vMovement);
}

void Renderer::ZoomCamera(float zoom)
{
    _camera->Zoom(zoom);
}
