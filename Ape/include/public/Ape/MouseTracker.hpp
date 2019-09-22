#pragma once

#include <Ape/Movement.hpp>
#include <Ape/Position.hpp>

class Window;

class MouseTracker
{

public:

    explicit MouseTracker(Window & window);

    auto update()
        -> void;

    auto getLastMovement() const
        -> Movement;

private:

    Window * window;

    Position<double> lastPosition;

    Movement lastMovement;

};
