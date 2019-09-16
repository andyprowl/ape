#pragma once

#include "CameraManipulator.hpp"
#include "MouseTracker.hpp"

#include <vector>

class Camera;
class Mesh;
class ShaderProgram;
class Scene;

class InputHandler
{

public:

    InputHandler(Scene & scene, GLFWwindow & window, ShaderProgram const & program);

    auto processInput(double lastFrameDuration)
        -> void;

private:

    auto processTerminationRequest() const
        -> void;

    auto processMouseCapture() const
        -> void;

    auto processFullScreenToggle() const
        -> void;

    auto processMouseMovement(double lastFrameDuration)
        -> void;

    auto processRotationalMovement(double lastFrameDuration) const
        -> void;

    auto processLateralMovement(double lastFrameDuration) const
        -> void;

    auto processShapeModification(double lastFrameDuration) const
        -> void;

    auto processShapeRotation(double lastFrameDuration) const
        -> void;

    auto processShapeScaling(double lastFrameDuration) const
        -> void;

    auto processLightRevolution(double lastFrameDuration) const
        -> void;

private:

    Scene * scene;

    GLFWwindow * window;
    
    ShaderProgram const * program;

    CameraCameraManipulator cameraManipulator;

};
