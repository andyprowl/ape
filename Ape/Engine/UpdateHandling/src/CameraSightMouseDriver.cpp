#include <Ape/Engine/UpdateHandling/CameraSightMouseDriver.hpp>

#include <Ape/Engine/UpdateHandling/MovementTracker.hpp>

#include <Ape/Engine/Windowing/Window.hpp>

#include <Ape/World/Scene/Camera.hpp>
#include <Ape/World/Scene/CameraSelector.hpp>

#include <Basix/Mathematics/Math.hpp>
#include <Basix/Mathematics/Offset.hpp>

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

    auto const direction = activeCamera->getView().getDirection();

    auto const pitch = asin(direction.y);

    auto const yaw = acos(direction.x / cos(pitch)) * ((direction.z < 0) ? -1.0f : 1.0f);

    auto const roll = 0.0f;

    return {glm::degrees(yaw), glm::degrees(pitch), glm::degrees(roll)};
}

} // unnamed namespace

CameraSightMouseDriver::CameraSightMouseDriver(
    CameraSelector & cameraSelector,
    Window & window,
    float const sensitivity)
    : cameraSelector{&cameraSelector}
    , window{&window}
    , mouseMovementTracker{window.getMousePosition()}
    , angles{getInitialAngles(cameraSelector)}
    , sensitivity{sensitivity}
    , isEngaged{false}
    , cameraChangeHandlerConnection{registerForActiveCameraChangeNotifications()}
{
}

auto CameraSightMouseDriver::isActive() const
    -> bool
{
    return isEngaged;
}

auto CameraSightMouseDriver::activate()
    -> void
{
    mouseMovementTracker.updatePosition(window->getMousePosition());

    isEngaged = true;
}

auto CameraSightMouseDriver::deactivate()
    -> void
{
    isEngaged = false;
}

auto CameraSightMouseDriver::onFrame()
    -> void
{
    auto const mouseOffset = mouseMovementTracker.updatePosition(window->getMousePosition());

    if (not isActive())
    {
        return;
    }

    auto const activeCamera = cameraSelector->getActiveCamera();

    if (activeCamera == nullptr)
    {
        return;
    }

    if ((mouseOffset.deltaX == 0) || (mouseOffset.deltaY == 0))
    {
        return;
    }

    auto const angularOffset = basix::Offset2d{
        mouseOffset.deltaX * sensitivity,
        -mouseOffset.deltaY * sensitivity};

    moveBy(*activeCamera, angularOffset);
}

auto CameraSightMouseDriver::onMouseWheel(basix::Offset2d<int> const offset)
    -> void
{
    if (not isActive())
    {
        return;
    }

    auto const activeCamera = cameraSelector->getActiveCamera();

    if (activeCamera == nullptr)
    {
        return;
    }

    auto const factor = offset.deltaY * 2.0f;

    zoomBy(*activeCamera, factor);
}

auto CameraSightMouseDriver::registerForActiveCameraChangeNotifications()
    -> basix::ScopedSignalConnection
{
    return cameraSelector->onActiveCameraChanged.registerHandler(
        [this] (Camera const * const /*activeCamera*/)
    {
        angles = getInitialAngles(*cameraSelector);
    });
}

auto CameraSightMouseDriver::moveBy(Camera & camera, basix::Offset2d<float> const & angularOffset)
    -> void
{
    angles.pitch = basix::clamp((angles.pitch + angularOffset.deltaY), -89.0f, 89.0f);

    angles.yaw += angularOffset.deltaX;

    auto const newDirection = computeDirection(angles);

    camera.getView().setDirection(newDirection);
}

auto CameraSightMouseDriver::zoomBy(Camera & camera, float const factor) const
    -> void
{
    auto perspective = camera.getProjection().tryAs<PerspectiveProjection>();

    if (perspective == nullptr)
    {
        return;
    }

    auto const currentFieldOfView = glm::degrees(perspective->getFieldOfView());

    auto const newFieldOfView = currentFieldOfView - factor;

    auto const clampedFieldOfView = basix::clamp(newFieldOfView, 1.0f, 45.0f);

    perspective->setFieldOfView(glm::radians(clampedFieldOfView));
}

} // namespace ape
