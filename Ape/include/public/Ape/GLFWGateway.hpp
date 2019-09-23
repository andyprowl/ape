#pragma once

#include <Ape/GLFWWindow.hpp>

#include <memory>
#include <stdexcept>
 
namespace ape
{

class CouldNotLoadOpenGLFunctions : public std::logic_error
{

public:

    CouldNotLoadOpenGLFunctions()
        : logic_error{"Failed to load OpenGL functions"}
    {
    }

};

class GLFWGateway
{

public:

    GLFWGateway();

    GLFWGateway(GLFWGateway const & rhs) = delete;

    GLFWGateway(GLFWGateway && rhs) noexcept;

    auto operator = (GLFWGateway const & rhs)
        -> GLFWGateway & = delete;

    auto operator = (GLFWGateway && rhs) noexcept
        -> GLFWGateway &;

    ~GLFWGateway();

    auto createWindow(std::string const & title, bool createAsFullscreen)
        -> GLFWWindow;

    auto initializeOpenGL()
        -> void;

private:

    class Impl;

private:

    std::unique_ptr<Impl> impl;

};

} // namespace ape
