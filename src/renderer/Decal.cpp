#include "Decal.h"

#include "glad/glad.h"
#include "glm/gtc/matrix_transform.hpp"

#include "Shader.h"
#include "Texture.h"


Decal::Decal(const glm::vec3& position, const glm::vec3& lookAtPoint, float xExtent, float yExtent, float minProjectionDistance, float maxProjectionDistance)
    : _position(position),
    _lookAtPoint(lookAtPoint),
    _xExtent(xExtent),
    _yExtent(yExtent),
    _minProjDistance(minProjectionDistance),
    _maxProjDistance(maxProjectionDistance)
{
    _viewMatrix = glm::lookAt(_position, _lookAtPoint, glm::vec3(0.f, 1.f, 0.f));
    _projectionMatrix = glm::ortho(-_xExtent / 2.f, _xExtent / 2.f, _yExtent / 2.f, -_yExtent / 2.f, _minProjDistance, _maxProjDistance);
}

Decal::~Decal() { _texture.reset(); }

void Decal::SetupShader()
{
    _shader = std::make_unique<Shader>("shaders/screenPass.vs", "shaders/decal.fs");

    _shader->Bind();
    _shader->SetUniformMat4f("uDecalViewMatrix", GetViewMatrix());
    _shader->SetUniformMat4f("uDecalOrthoMatrix", GetProjectionMatrix());
    _shader->SetUniform1f("uDecalSpec", 15.f);

    _texture = std::make_shared<Texture>("res/decalTexture.jpg");
    _shader->SetUniform1f("uDecalMap", _texture->GetID());
}

void Decal::UseShader(std::uint32_t colorBuffer, std::uint32_t positionBufferId, std::uint32_t depthBufferId)
{
    // Binding the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, colorBuffer);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, positionBufferId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthBufferId);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _texture->GetID());

    _shader->Bind();
    _shader->SetUniform1i("gPosition", 0);
    _shader->SetUniform1i("uDecalDepthMap", 1);
    _shader->SetUniform1i("uDecalMap", 2);
}