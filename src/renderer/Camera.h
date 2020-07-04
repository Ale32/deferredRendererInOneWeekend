#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>


enum class CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};


class Camera
{
private:
    glm::vec3 _position;
    glm::vec3 _up;
    glm::vec3 _right;

    glm::vec3 _front = glm::vec3(0.f, 0.f, -1.f);
    glm::vec3 _worldUp = glm::vec3(0.f, 1.f, 0.f);

    // Euler Angles
    float _yaw;
    float _pitch;

    float _zoom;

    // Camera options
    const float _SPEED = 15.f;
    const float _ROTATION_SENSITIVITY = 0.15f;
    const float _NEAR_PLANE = 0.1f;
    const float _FAR_PLANE = 100.f;
    
    void _updateCameraVectors();

public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float zoom = 45.f, float yaw = -90.f, float pitch = 0.f);

    /** Returns the view position */
    glm::vec3 GetPosition() const;

    /** Returns the view matrix calculated using Euler Angles and the LookAt Matrix */
    glm::mat4 GetViewMatrix() const;

    /** Returns the view matrix calculated using Euler Angles and the LookAt Matrix */
    glm::mat4 GetProjectionMatrix(int width, int height) const;

    /**
     *  Move the camera along the given direction
     *
     *  \param   direction
     *  \param   deltaTime
     */
    void Move(CameraMovement direction, float deltaTime);

    /**
     *  Rotate the camera to look around
     *
     *  \param   xoffset    X angle
     *  \param   yoffset    Y angle
     */
    void LookAround(float horizontalAmount, float verticalAmount);

    /**
     *  Zoom the camera in/out
     *
     *  \param   amount  The intensity of the zoom
     */
    void Zoom(float amount);
};