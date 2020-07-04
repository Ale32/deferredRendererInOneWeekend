#pragma once

#include "glm/glm.hpp"

#include <memory>


class Light
{
protected:
    float _influence;

    glm::vec3 _position;
    glm::vec3 _color;

    bool _castShadow;

public:
    Light();
    virtual ~Light();

    inline float GetInfluence() const { return _influence; }
    inline glm::vec3 GetPosition() const { return _position; }
    inline glm::vec3 GetColor() const { return _color; }
    inline bool CastShadow() const { return _castShadow; }
};


class PointLight : public Light
{
public:
    PointLight();
    ~PointLight();

    PointLight(float influence, glm::vec3 position, glm::vec3 color);
};


class SpotLight : public Light
{
private:
    glm::vec3 _direction;

    // Degree angle
    float _coneAngle;

    // The cosine of the angle in radians
    float _cutOffValue;

    const float _NEAR = 0.1f;

public:
    SpotLight();
    ~SpotLight();

    SpotLight(float influence, glm::vec3 position, glm::vec3 color, glm::vec3 direction, float coneAngle);

    inline glm::vec3 GetDirection() const { return _direction; }
    inline float GetConeAngle() const { return _coneAngle; }
    inline float GetCutOffValue() const { return _cutOffValue; }

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(int width, int height) const;
};