#pragma once

#include "MouseTracker.hpp"
#include "ScopedSignalConnection.hpp"

class Camera;
class MouseWheelPublisher;
class Window;

class CameraSightMouseDriver
{

public:

    CameraSightMouseDriver(
        Window & window,
        Camera & camera,
        float sensitivity);

    auto update(double lastFrameDuration)
        -> void;

private:

    auto registerForWheelNotifications()
        -> ScopedSignalConnection;

private:

    Window * window;

    MouseTracker mouseTracker;

    Camera * camera;

    float pitch;

    float yaw;

    float sensitivity;

    ScopedSignalConnection wheelHandlerConnection;

};
