#pragma once

#include "MouseTracker.hpp"
#include "MouseWheelPublisher.hpp"

#include "GLFW.hpp"

class Camera;

class CameraSightMouseDriver
{

public:

    CameraSightMouseDriver(GLFWwindow & window, Camera & camera, float sensitivity);

    auto update(double lastFrameDuration)
        -> void;

private:

    auto registerForWheelNotifications()
        -> void;

private:

    MouseTracker mouseTracker;

    MouseWheelPublisher wheelPublisher;

    Camera * camera;

    float pitch;

    float yaw;

    float sensitivity;

};
