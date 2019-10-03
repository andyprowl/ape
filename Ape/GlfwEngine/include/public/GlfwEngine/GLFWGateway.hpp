#pragma once

#include <GlfwEngine/GLFWWindow.hpp>

#include <memory>
 
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

    GLFWGateway();

    GLFWGateway(GLFWGateway && rhs) noexcept;

    auto operator = (GLFWGateway && rhs) noexcept
        -> GLFWGateway &;

    ~GLFWGateway();

    auto createWindow(std::string const & title, CreateAsFullscreen)
        -> GLFWWindow;

    auto createWindow(std::string const & title, Size<int> const & size)
        -> GLFWWindow;

private:

    class Impl;

private:

    std::unique_ptr<Impl> impl;

};

} // namespace ape
