#pragma once

#include <Ape/UpdateHandling/CameraSightMouseDriver.hpp>

#include <chrono>
#include <optional>

namespace ape
{

class Camera;
class CameraSelector;
class Window;

class CameraManipulator
{

public:

    CameraManipulator(CameraSelector & cameraSelector, Window & window, float sensitivity);

    auto isActive() const
        -> bool;

    auto activate()
        -> void;

    auto deactivate()
        -> void;

    auto onFrame(std::chrono::nanoseconds frameDuration)
        -> void;

    auto onMouseWheel(basix::Offset<int> offset)
        -> void;

    auto getCameraSelector() const
        -> CameraSelector &;

private:

    auto processStraightMovement(Camera & camera, double lastFrameDuration) const
        -> void;

    auto processStrafeMovement(Camera & camera, double lastFrameDuration) const
        -> void;

private:

    CameraSelector * cameraSelector;

    Window * window;

    CameraSightMouseDriver sightDriver;

    bool isEngaged;

};

} // namespace ape
