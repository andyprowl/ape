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
    instance->onKeyboardEvent.fire(key, scancode, action, mods);
}

} // unnamed namespace

KeyboardPublisher::KeyboardPublisher(GLFWwindow & window)
{
    ensureUniqueInstance();

    glfwSetKeyCallback(&window, ::onKeyboard);
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
