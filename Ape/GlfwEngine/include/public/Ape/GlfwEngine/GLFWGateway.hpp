#pragma once

#include <Ape/GlfwEngine/GLFWWindow.hpp>

#include <memory>
#include <string_view>

namespace ape
{

class Ape;

class GLFWGateway
{

public:

    class CreateAsFullscreen
    {

    public:

        explicit CreateAsFullscreen() = default;

    };

public:

    GLFWGateway(int majorVersion, int minorVersion, bool enableDebugOutput);

    GLFWGateway(GLFWGateway && rhs) noexcept;

    auto operator = (GLFWGateway && rhs) noexcept
        -> GLFWGateway &;

    ~GLFWGateway();

    auto createWindow(std::string_view title, CreateAsFullscreen)
        -> GLFWWindow;

    auto createWindow(std::string_view title, Size<int> const & size)
        -> GLFWWindow;

private:

    class Impl;

private:

    std::unique_ptr<Impl> impl;

};

} // namespace ape
