#include "MouseWheelPublisher.hpp"

#include <cassert>

namespace
{

auto instance = static_cast<MouseWheelPublisher const *>(nullptr);

auto onMouseWheel(GLFWwindow * const /*window*/, double const /*xOffset*/, double const yOffset)
    -> void
{
    instance->onWheelEvent.fire(yOffset);
}

} // unnamed namespace

MouseWheelPublisher::MouseWheelPublisher(GLFWwindow & window)
{
    ensureUniqueInstance();

    glfwSetScrollCallback(&window, ::onMouseWheel);
}

auto MouseWheelPublisher::ensureUniqueInstance() const
    -> void
{
    if (instance != nullptr)
    {
        assert(false);

        throw DuplicateMouseWheelPublisher{};
    }
    else
    {
        instance = this;
    }
}
