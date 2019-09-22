#pragma once

#include <Ape/CameraSightMouseDriver.hpp>

class Scene;
class Window;

class CameraManipulator
{

public:

    CameraManipulator(Scene & scene, Window & window, float sensitivity);

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

    Window * window;

    CameraSightMouseDriver sightDriver;

};
