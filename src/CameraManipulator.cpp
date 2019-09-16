#include "CameraManipulator.hpp"

#include "Camera.hpp"
#include "Math.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

namespace
{

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

CameraCameraManipulator::CameraCameraManipulator(
    GLFWwindow & window,
    Camera & camera,
    float const sensitivity)
    : mouseTracker{window}
    , wheelPublisher{window}
    , camera{&camera}
    , pitch{0.0}
    , yaw{getInitialYaw(camera)}
    , sensitivity{sensitivity}
{
    registerForWheelNotifications();
}

auto CameraCameraManipulator::update(double const /*lastFrameDuration*/)
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

auto CameraCameraManipulator::registerForWheelNotifications()
    -> void
{
    wheelPublisher.registerHandler([this] (double const offset)
    {
        auto const currentFieldOfView = glm::degrees(camera->getFieldOfView());

        auto const newFieldOfView = currentFieldOfView - static_cast<float>(offset * 2.0);

        auto const clampedFieldOfView = clamp(newFieldOfView, 1.0f, 60.0f);

        camera->setFieldOfView(glm::radians(clampedFieldOfView));
    });
}
