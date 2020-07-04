#pragma once

#include <memory>

#include "glm/glm.hpp"

class Shader;
class Texture;


class Decal
{
private:
    glm::vec3 _position;
    glm::vec3 _lookAtPoint;

    float _xExtent;
    float _yExtent;
    float _minProjDistance;
    float _maxProjDistance;

    glm::mat4 _viewMatrix;
    glm::mat4 _projectionMatrix;

    std::unique_ptr<Shader> _shader;
    std::shared_ptr<Texture> _texture;

public:
    Decal(const glm::vec3& position, const glm::vec3& direction, float xExtent, float yExtent, float minProjectionDistance = 0.1f, float maxProjectionDistance = 100.f);
    ~Decal();

    inline glm::mat4 GetViewMatrix() const { return _viewMatrix; }
    inline glm::mat4 GetProjectionMatrix() const { return _projectionMatrix; }
    inline const Shader& GetShader() const { return *_shader; }

    void SetupShader();
    void UseShader(std::uint32_t colorBuffer, std::uint32_t positionBufferId, std::uint32_t depthBufferId);
};
