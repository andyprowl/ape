#pragma once

#include <Ape/MouseTracker.hpp>
#include <Ape/ScopedSignalConnection.hpp>
#include <Ape/TaitBryanAngles.hpp>

namespace ape
{

class CameraSelector;
class MouseWheelPublisher;
class Window;

class CameraSightMouseDriver
{

public:

    CameraSightMouseDriver(Window & window, CameraSelector & cameraSelector, float sensitivity);

    auto update()
        -> void;

private:

    auto registerForWheelNotifications()
        -> ScopedSignalConnection;

    auto registerForActiveCameraChangeNotifications()
        -> ScopedSignalConnection;

    auto moveBy(Offset const & offset)
        -> void;

    auto zoomBy(double factor) const
        -> void;

private:

    Window * window;

    CameraSelector * cameraSelector;

    MouseTracker mouseTracker;

    TaitBryanAngles angles;

    float sensitivity;

    ScopedSignalConnection wheelHandlerConnection;

    ScopedSignalConnection cameraChangeHandlerConnection;

};

} // namespace ape
