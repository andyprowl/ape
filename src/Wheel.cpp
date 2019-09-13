#include "Wheel.hpp"

#include <cassert>

namespace
{

auto instance = static_cast<MouseWheelPublisher const *>(nullptr);

auto onMouseWheel(GLFWwindow * const /*window*/, double const /*xOffset*/, double const yOffset)
    -> void
{
    instance->onMouseWheel(yOffset);
}

} // unnamed namespace

MouseWheelPublisher::MouseWheelPublisher(GLFWwindow & window)
{
    ensureUniqueInstance();

    glfwSetScrollCallback(&window, ::onMouseWheel);
}

auto MouseWheelPublisher::registerHandler(Handler handler)
    -> Cookie
{
    auto const cookie = static_cast<int>(registrations.size());

    registrations.emplace(cookie, std::move(handler));

    return cookie;
}

auto MouseWheelPublisher::unregisterHandler(Cookie cookie)
    -> void
{
    registrations.erase(cookie);
}

auto MouseWheelPublisher::onMouseWheel(double const offset) const
    -> void
{
    for (const auto & registration : registrations)
    {
        registration.second(offset);
    }
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
