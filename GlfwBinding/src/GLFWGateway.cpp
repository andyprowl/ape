#include <GlfwBinding/GLFWGateway.hpp>

#include "GLFWInitializer.hpp"

#include "GLFW.hpp"

namespace ape
{

class GLFWGateway::Impl
{

public:

    auto createWindow(std::string const & title, CreateAsFullscreen)
        -> GLFWWindow
    {
        auto window = GLFWWindow{title, GLFWWindow::CreateAsFullscreen{}};

        window.makeCurrent();

        return window;
    }

    auto createWindow(std::string const & title, Size<int> const & size)
        -> GLFWWindow
    {
        auto window = GLFWWindow{title, size};

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

auto GLFWGateway::createWindow(std::string const & title, CreateAsFullscreen)
    -> GLFWWindow
{
    return impl->createWindow(title, CreateAsFullscreen{});
}

auto GLFWGateway::createWindow(std::string const & title, Size<int> const & size)
    -> GLFWWindow
{
    return impl->createWindow(title, size);
}

} // namespace ape
