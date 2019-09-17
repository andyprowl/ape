#pragma once

#include "Signal.hpp"

#include "GLFW.hpp"

#include <exception>
#include <functional>
#include <unordered_map>

class DuplicateMouseWheelPublisher : public std::exception
{

public:

    // virtual (from std::exception)
    auto what() const noexcept
        -> char const * override
    {
        return "There cannot be two instances of MouseWheelPublisher";
    }

};

class MouseWheelPublisher
{

public:

    using WheelEventSignature = auto (double offset) -> void;

public:

    explicit MouseWheelPublisher(GLFWwindow & window);

public:

    Signal<WheelEventSignature> onWheelEvent;

private:

    auto ensureUniqueInstance() const
        -> void;

};