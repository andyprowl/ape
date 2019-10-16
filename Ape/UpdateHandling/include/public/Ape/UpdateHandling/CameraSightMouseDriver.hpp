#pragma once

#include <UpdateHandling/MovementTracker.hpp>
#include <UpdateHandling/TaitBryanAngles.hpp>

#include <Mathematics/Offset.hpp>
#include <Mathematics/Position.hpp>

#include <Signal/ScopedSignalConnection.hpp>

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

    auto onMouseWheel(Offset<int> offset)
        -> void;

private:

    auto registerForActiveCameraChangeNotifications()
        -> ScopedSignalConnection;

    auto moveBy(Camera & camera, Offset<float> const & angularOffset)
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

    ScopedSignalConnection cameraChangeHandlerConnection;

};

} // namespace ape
