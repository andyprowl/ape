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

    auto processCameraManipulation(double lastFrameDuration)
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

    CameraManipulator cameraManipulator;

};
