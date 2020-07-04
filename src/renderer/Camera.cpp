#include "Camera.h"

#include <algorithm>


Camera::Camera(glm::vec3 position, float zoom, float yaw, float pitch)
{
    _position = position;
    _yaw = yaw;
    _pitch = pitch;
    _zoom = zoom;

    _updateCameraVectors();
}

void Camera::_updateCameraVectors()
{
    // Calculate the new front vector
    glm::vec3 front;
    front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    front.y = sin(glm::radians(_pitch));
    front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(front);

    // Re-calculate the Right and Up vector
    // Normalize the vectors because their length gets closer to 0 the more you look up or down which results in slower movement
    _right = glm::normalize(glm::cross(_front, _worldUp));
    _up = glm::normalize(glm::cross(_right, _front));
}

glm::vec3 Camera::GetPosition() const
{
    return _position;
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(_position, _position + _front, _up);
}

glm::mat4 Camera::GetProjectionMatrix(int width, int height) const
{
    return glm::perspective(glm::radians(_zoom), (float)width / (float)height, _NEAR_PLANE, _FAR_PLANE);
}

void Camera::Move(CameraMovement direction, float deltaTime)
{
    float velocity = _SPEED * deltaTime;
    if (direction == CameraMovement::FORWARD)
        _position += _front * velocity;
    if (direction == CameraMovement::BACKWARD)
        _position -= _front * velocity;
    if (direction == CameraMovement::LEFT)
        _position -= _right * velocity;
    if (direction == CameraMovement::RIGHT)
        _position += _right * velocity;
    if (direction == CameraMovement::UP)
        _position += _up * velocity;
    if (direction == CameraMovement::DOWN)
        _position -= _up * velocity;
}

void Camera::LookAround(float horizontalAmount, float verticalAmount)
{
    horizontalAmount *= _ROTATION_SENSITIVITY;
    verticalAmount *= _ROTATION_SENSITIVITY;

    _yaw += horizontalAmount;
    _pitch += verticalAmount;

    const float maxPitch = 89.f;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    // Could use std::clamp(n, lower, upper) here but I don't want to add c++17 requirement only for it
    if (_pitch > maxPitch)
        _pitch = maxPitch;
    if (_pitch < -maxPitch)
        _pitch = -maxPitch;

    // Update Front, Right and Up Vectors using the updated Euler angles
    _updateCameraVectors();
}

void Camera::Zoom(float amount)
{
    const float min = 30.f;
    const float max = 90.f;

    // Could use std::clamp(n, lower, upper) here but I don't want to add c++17 requirement only for it
    if (_zoom >= min && _zoom <= max)
        _zoom -= amount;

    if (_zoom < min)
        _zoom = min;
    if (_zoom > max)
        _zoom = max;
}