#pragma once

#include "glad/glad.h"

#include <GLFW/glfw3.h>

#include <vector>

class Camera;
class ShaderProgram;
class Widget;
class World;

class InputHandler
{

public:

    InputHandler(
        World & world,
        Camera & camera,
        GLFWwindow & window,
        ShaderProgram const & program);

    auto processInput(double lastFrameDuration) const
        -> void;

private:

    auto processTerminationRequest() const
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

    auto processStyleModification(double lastFrameDuration) const
        -> void;

private:

    World * world;

    Camera * camera;
    
    GLFWwindow * window;
    
    ShaderProgram const * program;

};
