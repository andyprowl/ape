#include "CameraManipulator.hpp"

#include "Scene.hpp"
#include "Window.hpp"

#include "GLFW.hpp"

#include <glm/trigonometric.hpp>

namespace
{

auto rotateCameraAroundUpVector(Camera & camera, float const radians)
    -> void
{
    auto const rotation = glm::rotate(glm::mat4{1.0f}, radians, camera.getUp());

    auto const position = camera.getPosition();

    auto const direction = camera.getDirection();

    auto const newDirection = rotation * glm::vec4{direction, 1.0f};

    camera.setDirection(newDirection);
}

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

    processKeyboardBasedRotation(lastFrameDuration);

    processStraightMovement(lastFrameDuration);

    processStrafeMovement(lastFrameDuration);
}

auto CameraManipulator::processMouseMovement(double const lastFrameDuration)
    -> void
{
    sightDriver.update(lastFrameDuration);
}

auto CameraManipulator::processKeyboardBasedRotation(double const lastFrameDuration) const
    -> void
{
    auto const rotationDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    if (window->getKeyStatus(GLFW_KEY_J) == GLFW_PRESS)
    {
        rotateCameraAroundUpVector(*scene->cameraSystem.activeCamera, +rotationDelta);
    }
    else if (window->getKeyStatus(GLFW_KEY_L) == GLFW_PRESS)
    {
        rotateCameraAroundUpVector(*scene->cameraSystem.activeCamera, -rotationDelta);
    }
}

auto CameraManipulator::processStraightMovement(double lastFrameDuration) const
    -> void
{
    auto const translationDelta = static_cast<float>(lastFrameDuration * 5.0f);

    if ((window->getKeyStatus(GLFW_KEY_I) == GLFW_PRESS) ||
        (window->getKeyStatus(GLFW_KEY_UP) == GLFW_PRESS))
    {
        moveCameraAlongDirection(*scene->cameraSystem.activeCamera, +translationDelta);
    }
    else if ((window->getKeyStatus(GLFW_KEY_K) == GLFW_PRESS) ||
             (window->getKeyStatus(GLFW_KEY_DOWN) == GLFW_PRESS))
    {
        moveCameraAlongDirection(*scene->cameraSystem.activeCamera, -translationDelta);
    }
}

auto CameraManipulator::processStrafeMovement(double const lastFrameDuration) const
    -> void
{
    auto const translationDelta = static_cast<float>(lastFrameDuration * 5.0f);

    if (window->getKeyStatus(GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        moveCameraSideways(*scene->cameraSystem.activeCamera, -translationDelta);
    }
    else if (window->getKeyStatus(GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        moveCameraSideways(*scene->cameraSystem.activeCamera, +translationDelta);
    }
}
