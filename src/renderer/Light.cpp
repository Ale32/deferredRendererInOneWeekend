#include "Light.h"

#include <glm/gtc/matrix_transform.hpp>

Light::Light()
    : _influence(5.f),
    _position(glm::vec3{ 0.f }),
    _color(glm::vec3{ 1.f }),
    _castShadow(false)
{
}

Light::~Light()
{
}

PointLight::PointLight()
    : Light()
{
    _castShadow = false;
}

PointLight::~PointLight()
{
}

PointLight::PointLight(float influence, glm::vec3 position, glm::vec3 color)
    : Light()
{
    _influence = influence;
    _position = position;
    _color = color;

    _castShadow = false;
}

SpotLight::SpotLight()
    : Light()
{
    _direction = glm::vec3(-1.f, -1.f, -1.f);
    _coneAngle = 30.f;

    _castShadow = true;
}

SpotLight::~SpotLight()
{
}

SpotLight::SpotLight(float influence, glm::vec3 position, glm::vec3 color, glm::vec3 direction, float coneAngle)
    : Light()
{
    _influence = influence;
    _position = position;
    _color = color;
    _direction = direction;
    _coneAngle = coneAngle;
    _cutOffValue = glm::cos(glm::radians(coneAngle));

    _castShadow = true;
}

glm::mat4 SpotLight::GetViewMatrix() const
{
    // TODO: This needs attention. The up vector could cause issue if parallel to the lookAt vector
    return glm::lookAt(_position, _position + _direction, glm::vec3(0.f, 1.f, 0.f));
}

glm::mat4 SpotLight::GetProjectionMatrix(int width, int height) const
{
    return glm::perspective(glm::radians(2 * _coneAngle), (float)width / (float)height, _NEAR, _influence * 2.f);
}
