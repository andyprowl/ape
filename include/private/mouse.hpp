#pragma once

#include "position.hpp"

#include "glfw.hpp"

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
