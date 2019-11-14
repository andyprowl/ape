#pragma once

#include <Ape/Engine/UpdateHandling/MovementTracker.hpp>
#include <Ape/Engine/UpdateHandling/TaitBryanAngles.hpp>

#include <Basix/Mathematics/Offset.hpp>
#include <Basix/Mathematics/Position.hpp>
#include <Basix/Signal/ScopedSignalConnection.hpp>

namespace ape
{

class Camera;
class CameraSelector;
class MouseWheelPublisher;
class Window;

class CameraSightMouseDriver
{

public:

    CameraSightMouseDriver(CameraSelector & cameraSelector, Window & window, float sensitivity);

    auto isActive() const
        -> bool;

    auto activate()
        -> void;

    auto deactivate()
        -> void;

    auto onFrame()
        -> void;

    auto onMouseWheel(basix::Offset<int> offset)
        -> void;

private:

    auto registerForActiveCameraChangeNotifications()
        -> basix::ScopedSignalConnection;

    auto moveBy(Camera & camera, basix::Offset<float> const & angularOffset)
        -> void;

    auto zoomBy(Camera & camera, float factor) const
        -> void;

private:

    CameraSelector * cameraSelector;

    Window * window;

    MovementTracker mouseMovementTracker;

    TaitBryanAngles angles;

    float sensitivity;

    bool isEngaged;

    basix::ScopedSignalConnection cameraChangeHandlerConnection;

};

} // namespace ape
