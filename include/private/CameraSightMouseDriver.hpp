#pragma once

#include "MouseTracker.hpp"
#include "MouseWheelPublisher.hpp"

#include "GLFW.hpp"

class Camera;
class MouseWheelPublisher;

class CameraSightMouseDriver
{

public:

    CameraSightMouseDriver(
        GLFWwindow & window,
        Camera & camera,
        MouseWheelPublisher & wheelPublisher,
        float sensitivity);

    auto update(double lastFrameDuration)
        -> void;

private:

    auto registerForWheelNotifications()
        -> void;

private:

    MouseTracker mouseTracker;

    Camera * camera;

    MouseWheelPublisher * wheelPublisher;

    float pitch;

    float yaw;

    float sensitivity;

};
