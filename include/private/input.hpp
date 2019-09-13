#pragma once

#include "manipulator.hpp"
#include "mouse.hpp"

#include <vector>

class Body;
class Camera;
class ShaderProgram;
class World;

class InputHandler
{

public:

    InputHandler(World & world, GLFWwindow & window, ShaderProgram const & program);

    auto processInput(double lastFrameDuration)
        -> void;

private:

    auto processTerminationRequest() const
        -> void;

    auto processMouseCapture() const
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

    World * world;

    GLFWwindow * window;
    
    ShaderProgram const * program;

    CameraManipulator cameraManipulator;

};
