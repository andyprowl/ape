#include "KeyboardPublisher.hpp"

#include <cassert>

namespace
{

auto instance = static_cast<KeyboardPublisher const *>(nullptr);

auto onKeyboard(
    GLFWwindow * const /*window*/,
    int const key,
    int const scancode,
    int const action,
    int const mods)
    -> void
{
    instance->onKeyboardEvent(key, scancode, action, mods);
}

} // unnamed namespace

KeyboardPublisher::KeyboardPublisher(GLFWwindow & window)
{
    ensureUniqueInstance();

    glfwSetKeyCallback(&window, ::onKeyboard);
}

auto KeyboardPublisher::registerHandler(Handler handler)
    -> Cookie
{
    auto const cookie = static_cast<int>(registrations.size());

    registrations.emplace(cookie, std::move(handler));

    return cookie;
}

auto KeyboardPublisher::unregisterHandler(Cookie cookie)
    -> void
{
    registrations.erase(cookie);
}

auto KeyboardPublisher::onKeyboardEvent(
    int const key,
    int const scancode,
    int const action,
    int const mods) const
    -> void
{
    for (const auto & registration : registrations)
    {
        registration.second(key, scancode, action, mods);
    }
}

auto KeyboardPublisher::ensureUniqueInstance() const
    -> void
{
    if (instance != nullptr)
    {
        assert(false);

        throw DuplicateKeyboardPublisher{};
    }
    else
    {
        instance = this;
    }
}
