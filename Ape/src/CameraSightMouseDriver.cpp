#include <Ape/CameraSightMouseDriver.hpp>

#include <Ape/Camera.hpp>
#include <Ape/CameraSelector.hpp>
#include <Ape/Math.hpp>
#include <Ape/Window.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

namespace ape
{

namespace
{

auto computeDirection(TaitBryanAngles const & angles)
    -> glm::vec3
{
    return glm::vec3{
        cos(glm::radians(angles.pitch)) * cos(glm::radians(angles.yaw)),
        sin(glm::radians(angles.pitch)),
        cos(glm::radians(angles.pitch)) * sin(glm::radians(angles.yaw))};
}

auto getInitialAngles(CameraSelector const & selector)
    -> TaitBryanAngles
{
    auto const activeCamera = selector.getActiveCamera();

    if (activeCamera == nullptr)
    {
        return {0.0, 0.0, 0.0};
    }

    auto const direction = activeCamera->getDirection();

    const auto pitch = asin(direction.y);

    auto const yaw = acos(direction.x / cos(pitch)) * ((direction.z < 0) ? -1.0f : 1.0f);

    const auto roll = 0.0f;

    return {glm::degrees(yaw), glm::degrees(pitch), glm::degrees(roll)};
}

} // unnamed namespace

CameraSightMouseDriver::CameraSightMouseDriver(
    Window & window,
    CameraSelector & cameraSelector,
    float const sensitivity)
    : window{&window}
    , cameraSelector{&cameraSelector}
    , mouseTracker{window}
    , angles{getInitialAngles(cameraSelector)}
    , sensitivity{sensitivity}
    , wheelHandlerConnection{registerForWheelNotifications()}
    , cameraChangeHandlerConnection{registerForActiveCameraChangeNotifications()}
{
}

auto CameraSightMouseDriver::update()
    -> void
{
    mouseTracker.update();

    auto const activeCamera = cameraSelector->getActiveCamera();

    if (activeCamera == nullptr)
    {
        return;
    }

    auto const offset = mouseTracker.getLastMovement();

    if ((offset.deltaX == 0.0) || (offset.deltaY == 0.0))
    {
        return;
    }

    auto const angularOffset = Offset{offset.deltaX * sensitivity, -offset.deltaY * sensitivity};

    moveBy(*activeCamera, angularOffset);
}

auto CameraSightMouseDriver::registerForWheelNotifications()
    -> ScopedSignalConnection
{
    return window->onMouseWheel.registerHandler([this] (double const offset)
    {
        auto const activeCamera = cameraSelector->getActiveCamera();

        if (activeCamera == nullptr)
        {
            return;
        }

        auto const factor = offset * 2.0;

        zoomBy(*activeCamera, factor);
    });
}

auto CameraSightMouseDriver::registerForActiveCameraChangeNotifications()
    -> ScopedSignalConnection
{
    return cameraSelector->onActiveCameraChanged.registerHandler(
        [this] (Camera const * const /*activeCamera*/)
    {
        angles = getInitialAngles(*cameraSelector);
    });
}

auto CameraSightMouseDriver::moveBy(Camera & camera, Offset const & angularOffset)
    -> void
{
    angles.pitch = clamp(angles.pitch + static_cast<float>(angularOffset.deltaY), -89.0f, 89.0f);

    angles.yaw += static_cast<float>(angularOffset.deltaX);

    auto const newDirection = computeDirection(angles);

    camera.setDirection(newDirection);
}

auto CameraSightMouseDriver::zoomBy(Camera & camera, double const factor) const
    -> void
{
    auto const currentFieldOfView = glm::degrees(camera.getFieldOfView());

    auto const newFieldOfView = currentFieldOfView - static_cast<float>(factor);

    auto const clampedFieldOfView = clamp(newFieldOfView, 1.0f, 45.0f);

    camera.setFieldOfView(glm::radians(clampedFieldOfView));
}

} // namespace ape
