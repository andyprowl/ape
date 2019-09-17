#pragma once

#include "CameraSightMouseDriver.hpp"

#include "GLFW.hpp"

class Scene;

class CameraManipulator
{

public:

    CameraManipulator(
        Scene & scene,
        GLFWwindow & window,
        MouseWheelPublisher & wheelPublisher,
        float sensitivity);

    auto update(double lastFrameDuration)
        -> void;

private:

    auto processMouseMovement(double lastFrameDuration)
        -> void;

    auto processKeyboardBasedRotation(double lastFrameDuration) const
        -> void;

    auto processStraightMovement(double lastFrameDuration) const
        -> void;

    auto processStrafeMovement(double lastFrameDuration) const
        -> void;

private:

    Scene * scene;

    GLFWwindow * window;

    CameraSightMouseDriver sightDriver;

};