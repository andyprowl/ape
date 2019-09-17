#pragma once

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

    using Cookie = int;

    using Handler = std::function<auto (int key, int scancode, int action, int mods) -> void>;

public:

    explicit KeyboardPublisher(GLFWwindow & window);

    auto registerHandler(Handler handler)
        -> Cookie;

    auto unregisterHandler(Cookie cookie)
        -> void;

    auto onKeyboardEvent(int key, int scancode, int action, int mods) const
        -> void;

private:

    auto ensureUniqueInstance() const
        -> void;

private:

    std::unordered_map<Cookie, Handler> registrations;

};