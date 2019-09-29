#include <Core/CameraManipulator.hpp>

#include <Core/CameraSelector.hpp>
#include <Core/Scene.hpp>
#include <Core/Window.hpp>

#include <glm/trigonometric.hpp>

namespace ape
{

namespace
{

auto moveCameraAlongDirection(Camera & camera, float const magnitude)
    -> void
{
    auto const position = camera.getPosition();

    auto const direction = camera.getDirection();

    auto const newPosition = position + (direction * magnitude);

    camera.setPosition(newPosition);
}

auto moveCameraSideways(Camera & camera, float const magnitude)
    -> void
{
    auto const position = camera.getPosition();

    auto const direction = camera.getDirection();

    auto const up = camera.getUp();

    auto const movementDirection = glm::cross(direction, up);

    auto const newPosition = position + (movementDirection * magnitude);

    camera.setPosition(newPosition);
}

} // unnamed namespace

CameraManipulator::CameraManipulator(
    CameraSelector & cameraSelector,
    Window & window,
    float const sensitivity)
    : cameraSelector{&cameraSelector}
    , window{&window}
    , sightDriver{cameraSelector, window, sensitivity}
    , isEngaged{false}
{
}

auto CameraManipulator::isActive() const
    -> bool
{
    return isEngaged;
}

auto CameraManipulator::activate()
    -> void
{
    isEngaged = true;

    sightDriver.activate();
}

auto CameraManipulator::deactivate()
    -> void
{
    isEngaged = false;

    sightDriver.deactivate();
}

auto CameraManipulator::onFrame(std::chrono::nanoseconds const frameDuration)
    -> void
{
    sightDriver.onFrame();

    if (!isActive())
    {
        return;
    }

    auto const activeCamera = cameraSelector->getActiveCamera();

    if (activeCamera == nullptr)
    {
        return;
    }
    
    auto const frameDurationInSeconds = frameDuration.count() / 1'000'000'000.0;

    processStraightMovement(*activeCamera, frameDurationInSeconds);

    processStrafeMovement(*activeCamera, frameDurationInSeconds);
}

auto CameraManipulator::onMouseWheel(Offset<int> const offset)
    -> void
{
    sightDriver.onMouseWheel(offset);
}

auto CameraManipulator::getCameraSelector() const
    -> CameraSelector &
{
    return *cameraSelector;
}

auto CameraManipulator::processStraightMovement(
    Camera & camera,
    double const lastFrameDuration) const
    -> void
{
    auto const translationDelta = static_cast<float>(lastFrameDuration * 5.0f);

    if (window->isKeyPressed(Key::keyUp))
    {
        moveCameraAlongDirection(camera, +translationDelta);
    }
    else if (window->isKeyPressed(Key::keyDown))
    {
        moveCameraAlongDirection(camera, -translationDelta);
    }
}

auto CameraManipulator::processStrafeMovement(
    Camera & camera,
    double const lastFrameDuration) const
    -> void
{
    auto const translationDelta = static_cast<float>(lastFrameDuration * 5.0f);

    if (window->isKeyPressed(Key::keyLeft))
    {
        moveCameraSideways(camera, -translationDelta);
    }
    else if (window->isKeyPressed(Key::keyRight))
    {
        moveCameraSideways(camera, +translationDelta);
    }
}

} // namespace ape
