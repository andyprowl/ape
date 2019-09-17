#pragma once

#include "Signal.hpp"

#include "GLFW.hpp"

#include <exception>
#include <functional>
#include <unordered_map>

class DuplicateKeyboardPublisher : public std::exception
{

public:

    // virtual (from std::exception)
    auto what() const noexcept
        -> char const * override
    {
        return "There cannot be two instances of KeyboardPublisher";
    }

};

class KeyboardPublisher
{

public:

    using KeyboardEventSignature = auto (int key, int scancode, int action, int mods) -> void;

public:

    explicit KeyboardPublisher(GLFWwindow & window);

public:

    Signal<KeyboardEventSignature> onKeyboardEvent;

private:

    auto ensureUniqueInstance() const
        -> void;

};