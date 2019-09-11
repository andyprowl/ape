#pragma once

#include "mouse.hpp"

#include "glfw.hpp"

class Camera;

class CameraManipulator
{

public:

    CameraManipulator(GLFWwindow & window, Camera & camera, float sensitivity);

    auto update(double lastFrameDuration)
        -> void;

private:

    MouseTracker mouseTracker;

    Camera * camera;

    float pitch;

    float yaw;

    float sensitivity;

};
