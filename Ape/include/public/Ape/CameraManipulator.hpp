#pragma once

#include <Ape/CameraSightMouseDriver.hpp>

namespace ape
{

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

    auto processStraightMovement(double lastFrameDuration) const
        -> void;

    auto processStrafeMovement(double lastFrameDuration) const
        -> void;

private:

    Scene * scene;

    Window * window;

    CameraSightMouseDriver sightDriver;

};

} // namespace ape
