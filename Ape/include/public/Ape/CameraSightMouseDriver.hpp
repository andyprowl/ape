#pragma once

#include <Ape/ScopedSignalConnection.hpp>
#include <Ape/TaitBryanAngles.hpp>

namespace ape
{

class Camera;
class CameraSelector;
class MouseWheelPublisher;
class MouseTracker;
class Offset;

class CameraSightMouseDriver
{

public:

    CameraSightMouseDriver(
        MouseTracker & mouseTracker,
        CameraSelector & cameraSelector,
        float sensitivity);

    auto update()
        -> void;

private:

    auto registerForWheelNotifications()
        -> ScopedSignalConnection;

    auto registerForActiveCameraChangeNotifications()
        -> ScopedSignalConnection;

    auto moveBy(Camera & camera, Offset const & offset)
        -> void;

    auto zoomBy(Camera & camera, double factor) const
        -> void;

private:

    CameraSelector * cameraSelector;

    MouseTracker * mouseTracker;

    TaitBryanAngles angles;

    float sensitivity;

    ScopedSignalConnection wheelHandlerConnection;

    ScopedSignalConnection cameraChangeHandlerConnection;

};

} // namespace ape
