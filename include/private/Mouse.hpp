#pragma once

#include "Position.hpp"

#include "GLFW.hpp"

class MouseTracker
{

public:

    explicit MouseTracker(GLFWwindow & window);

    auto update()
        -> void;

    auto getLastMovement() const
        -> Movement;

private:

    GLFWwindow * window;

    Position lastPosition;

    Movement lastMovement;

};

auto getCurrentMousePosition(GLFWwindow & window)
    -> Position;
