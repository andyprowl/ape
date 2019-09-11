#include "manipulator.hpp"

#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

namespace
{

auto clamp(float const value, float const min, float const max)
    -> float
{
    return std::max(min, std::min(value, max));
}

auto getInitialYaw(Camera const & camera)
    -> float
{
    const auto currentDirection = camera.getDirection();

    const auto yawDirection = glm::normalize(glm::vec3{
        cos(glm::radians(0.0)) * cos(glm::radians(0.0)),
        sin(glm::radians(0.0)),
        cos(glm::radians(0.0)) * sin(glm::radians(0.0))});

    const auto angle = glm::acos(glm::dot(currentDirection, yawDirection));

    return glm::degrees(-angle);
}

} // unnamed namespace

CameraManipulator::CameraManipulator(GLFWwindow & window, Camera & camera, float const sensitivity)
    : mouseTracker{window}
    , camera{&camera}
    , pitch{0.0}
    , yaw{getInitialYaw(camera)}
    , sensitivity{sensitivity}
{
}

auto CameraManipulator::update(double const lastFrameDuration)
    -> void
{
    mouseTracker.update();

    const auto movement = mouseTracker.getLastMovement();

    const auto offset = Movement{movement.deltaX * sensitivity, -movement.deltaY * sensitivity};

    yaw += static_cast<float>(offset.deltaX);

    pitch = clamp(pitch + static_cast<float>(offset.deltaY), -89.0f, 89.0f);

    const auto newDirection = glm::vec3{
        cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
        sin(glm::radians(pitch)),
        cos(glm::radians(pitch)) * sin(glm::radians(yaw))};

    camera->setDirection(newDirection);
}
