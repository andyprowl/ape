#include <Ape/CameraManipulator.hpp>

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

CameraManipulator::CameraManipulator(Scene & scene, Window & window, float const sensitivity)
    : scene{&scene}
    , window{&window}
    , sightDriver{window, *scene.cameraSystem.activeCamera, sensitivity}
{
}

auto CameraManipulator::update(double lastFrameDuration)
    -> void
{
    processMouseMovement(lastFrameDuration);

    processStraightMovement(lastFrameDuration);

    processStrafeMovement(lastFrameDuration);
}

auto CameraManipulator::processMouseMovement(double const lastFrameDuration)
    -> void
{
    sightDriver.update(lastFrameDuration);
}

auto CameraManipulator::processStraightMovement(double lastFrameDuration) const
    -> void
{
    auto const translationDelta = static_cast<float>(lastFrameDuration * 5.0f);

    if (window->isKeyPressed(Key::keyUp))
    {
        moveCameraAlongDirection(*scene->cameraSystem.activeCamera, +translationDelta);
    }
    else if (window->isKeyPressed(Key::keyDown))
    {
        moveCameraAlongDirection(*scene->cameraSystem.activeCamera, -translationDelta);
    }
}

auto CameraManipulator::processStrafeMovement(double const lastFrameDuration) const
    -> void
{
    auto const translationDelta = static_cast<float>(lastFrameDuration * 5.0f);

    if (window->isKeyPressed(Key::keyLeft))
    {
        moveCameraSideways(*scene->cameraSystem.activeCamera, -translationDelta);
    }
    else if (window->isKeyPressed(Key::keyRight))
    {
        moveCameraSideways(*scene->cameraSystem.activeCamera, +translationDelta);
    }
}

} // namespace ape
