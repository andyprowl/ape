#include <Ape/GlfwEngine/GLFWGateway.hpp>

#include "GLFW.hpp"
#include "GLFWInitializer.hpp"

#include <Ape/Initialization/Initialization.hpp>

namespace ape
{

class GLFWGateway::Impl
{

public:

    Impl(int const majorVersion, int const minorVersion)
        : glfwInitializer{majorVersion, minorVersion}
    {
    }

    auto createWindow(std::string const & title, CreateAsFullscreen)
        -> GLFWWindow
    {
        auto window = GLFWWindow{title, GLFWWindow::CreateAsFullscreen{}};

        window.makeCurrent();

        ape::initialize();

        return window;
    }

    auto createWindow(std::string const & title, Size<int> const & size)
        -> GLFWWindow
    {
        auto window = GLFWWindow{title, size};

        window.makeCurrent();

        ape::initialize();

        return window;
    }

private:

    GLFWInitializer glfwInitializer;

};

GLFWGateway::GLFWGateway(int const majorVersion, int const minorVersion)
    : impl{std::make_unique<Impl>(majorVersion, minorVersion)}
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
