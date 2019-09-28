#pragma once

#include <Core/MovementTracker.hpp>
#include <Core/Offset.hpp>
#include <Core/Position.hpp>
#include <Core/ScopedSignalConnection.hpp>
#include <Core/TaitBryanAngles.hpp>

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
