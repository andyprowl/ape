#include <Ape/CameraManipulator.hpp>

#include <Ape/CameraSelector.hpp>
#include <Ape/Scene.hpp>
#include <Ape/Window.hpp>

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
    , sightDriver{window, cameraSelector, sensitivity}
    , isEngaged{true}
{
}

auto CameraManipulator::update(double const lastFrameDuration)
    -> void
{
    if (!isActive())
    {
        return;
    }

    auto const activeCamera = cameraSelector->getActiveCamera();

    if (activeCamera == nullptr)
    {
        return;
    }

    processMouseMovement();

    processStraightMovement(*activeCamera, lastFrameDuration);

    processStrafeMovement(*activeCamera, lastFrameDuration);
}

auto CameraManipulator::getCameraSelector() const
    -> CameraSelector &
{
    return *cameraSelector;
}

auto CameraManipulator::isActive() const
    -> bool
{
    return isEngaged;
}

auto CameraManipulator::activate()
    -> void
{
    isEngaged = true;;
}

auto CameraManipulator::deactivate()
    -> void
{
    isEngaged = false;
}

auto CameraManipulator::processMouseMovement()
    -> void
{
    sightDriver.update();
}

auto CameraManipulator::processStraightMovement(Camera & camera, double lastFrameDuration) const
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

auto CameraManipulator::processStrafeMovement(Camera & camera, double const lastFrameDuration) const
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
