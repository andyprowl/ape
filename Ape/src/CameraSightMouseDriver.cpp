#include <Ape/CameraSightMouseDriver.hpp>

#include <Ape/Camera.hpp>
#include <Ape/Math.hpp>
#include <Ape/Window.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

namespace
{

auto getInitialYaw(Camera const & camera)
    -> float
{
    auto const currentDirection = camera.getDirection();

    auto const yawDirection = glm::normalize(glm::vec3{
        cos(glm::radians(0.0)) * cos(glm::radians(0.0)),
        sin(glm::radians(0.0)),
        cos(glm::radians(0.0)) * sin(glm::radians(0.0))});

    auto const angle = glm::acos(glm::dot(currentDirection, yawDirection));

    return glm::degrees(-angle);
}

} // unnamed namespace

CameraSightMouseDriver::CameraSightMouseDriver(
    Window & window,
    Camera & camera,
    float const sensitivity)
    : window{&window}
    , mouseTracker{window}
    , camera{&camera}
    , pitch{0.0}
    , yaw{getInitialYaw(camera)}
    , sensitivity{sensitivity}
    , wheelHandlerConnection{registerForWheelNotifications()}
{
}

auto CameraSightMouseDriver::update(double const /*lastFrameDuration*/)
    -> void
{
    mouseTracker.update();

    auto const movement = mouseTracker.getLastMovement();

    auto const offset = Movement{movement.deltaX * sensitivity, -movement.deltaY * sensitivity};

    yaw += static_cast<float>(offset.deltaX);

    pitch = clamp(pitch + static_cast<float>(offset.deltaY), -89.0f, 89.0f);

    auto const newDirection = glm::vec3{
        cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
        sin(glm::radians(pitch)),
        cos(glm::radians(pitch)) * sin(glm::radians(yaw))};

    camera->setDirection(newDirection);
}

auto CameraSightMouseDriver::registerForWheelNotifications()
    -> ScopedSignalConnection
{
    return window->onMouseWheel.registerHandler([this] (double const offset)
    {
        auto const currentFieldOfView = glm::degrees(camera->getFieldOfView());

        auto const newFieldOfView = currentFieldOfView - static_cast<float>(offset * 2.0);

        auto const clampedFieldOfView = clamp(newFieldOfView, 1.0f, 45.0f);

        camera->setFieldOfView(glm::radians(clampedFieldOfView));
    });
}
