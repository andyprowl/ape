#pragma once

#include <Ape/CameraSightMouseDriver.hpp>
#include <Ape/MouseTracker.hpp>

namespace ape
{

class Camera;
class CameraSelector;
class Window;

class CameraManipulator
{

public:

    CameraManipulator(CameraSelector & cameraSelector, Window & window, float sensitivity);

    auto update(double lastFrameDuration)
        -> void;

    auto getCameraSelector() const
        -> CameraSelector &;

    auto isActive() const
        -> bool;

    auto activate()
        -> void;

    auto deactivate()
        -> void;

private:

    auto processMouseMovement()
        -> void;

    auto processStraightMovement(Camera & camera, double lastFrameDuration) const
        -> void;

    auto processStrafeMovement(Camera & camera, double lastFrameDuration) const
        -> void;

private:

    CameraSelector * cameraSelector;

    Window * window;

    MouseTracker mouseTracker;

    CameraSightMouseDriver sightDriver;

    bool isEngaged;

};

} // namespace ape
