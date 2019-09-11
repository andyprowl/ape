#pragma once

#include "mouse.hpp"
#include "wheel.hpp"

#include "glfw.hpp"

class Camera;

class CameraManipulator
{

public:

    CameraManipulator(GLFWwindow & window, Camera & camera, float sensitivity);

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
