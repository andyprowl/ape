#include <Ape/GLFWGateway.hpp>

#include "GLFWInitializer.hpp"

#include "GLFW.hpp"

namespace ape
{

class GLFWGateway::Impl
{

public:

    auto createWindow(std::string const & title, bool createAsFullscreen)
        -> GLFWWindow
    {
        auto window = GLFWWindow{title, createAsFullscreen};

        window.makeCurrent();

        return window;
    }

private:

    GLFWInitializer glfwInitializer;

};

GLFWGateway::GLFWGateway()
    : impl{std::make_unique<Impl>()}
{
}

GLFWGateway::GLFWGateway(GLFWGateway && rhs) noexcept = default;

auto GLFWGateway::operator = (GLFWGateway && rhs) noexcept
    -> GLFWGateway & = default;

GLFWGateway::~GLFWGateway() = default;

auto GLFWGateway::createWindow(std::string const & title, bool const createAsFullscreen)
    -> GLFWWindow
{
    return impl->createWindow(title, createAsFullscreen);
}

} // namespace ape
