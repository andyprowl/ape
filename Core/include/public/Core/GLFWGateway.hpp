#pragma once

#include <Core/GLFWWindow.hpp>

#include <memory>
 
namespace ape
{

class Ape;

class GLFWGateway
{

public:

    GLFWGateway();

    GLFWGateway(GLFWGateway && rhs) noexcept;

    auto operator = (GLFWGateway && rhs) noexcept
        -> GLFWGateway &;

    ~GLFWGateway();

    auto createWindow(std::string const & title, bool createAsFullscreen)
        -> GLFWWindow;

private:

    class Impl;

private:

    std::unique_ptr<Impl> impl;

};

} // namespace ape
