#pragma once

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

    using Cookie = int;

    using Handler = std::function<void(double offset)>;

public:

    explicit MouseWheelPublisher(GLFWwindow & window);

    auto registerHandler(Handler handler)
        -> Cookie;

    auto unregisterHandler(Cookie cookie)
        -> void;

    auto onMouseWheel(double offset) const
        -> void;

private:

    auto ensureUniqueInstance() const
        -> void;

private:

    std::unordered_map<Cookie, Handler> registrations;

};