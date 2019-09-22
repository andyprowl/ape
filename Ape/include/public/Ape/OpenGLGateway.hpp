#pragma once

#include <Ape/Window.hpp>

#include <memory>
#include <stdexcept>

class CouldNotLoadOpenGLFunctions : public std::logic_error
{

public:

    CouldNotLoadOpenGLFunctions()
        : logic_error{"Failed to load OpenGL functions"}
    {
    }

};

class OpenGLGateway
{

public:

    OpenGLGateway();

    OpenGLGateway(OpenGLGateway const & rhs) = delete;

    OpenGLGateway(OpenGLGateway && rhs) noexcept;

    auto operator = (OpenGLGateway const & rhs)
        -> OpenGLGateway & = delete;

    auto operator = (OpenGLGateway && rhs) noexcept
        -> OpenGLGateway &;

    ~OpenGLGateway();

    auto createWindow(std::string const & title, bool createAsFullscreen)
        -> Window;

private:

    class Impl;

private:

    std::unique_ptr<Impl> impl;

};
