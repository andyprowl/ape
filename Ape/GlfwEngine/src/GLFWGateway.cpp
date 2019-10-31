#include <Ape/GlfwEngine/GLFWGateway.hpp>

#include "GLFW.hpp"
#include "GLFWInitializer.hpp"

#include <Ape/Initialization/OpenGLLoader.hpp>

namespace ape
{

class GLFWGateway::Impl
{

public:

    Impl(int const majorVersion, int const minorVersion, bool const enableDebugOutput)
        : glfwInitializer{majorVersion, minorVersion}
        , loader{false, enableDebugOutput}
    {
    }

    auto createWindow(std::string_view title, CreateAsFullscreen)
        -> GLFWWindow
    {
        auto window = GLFWWindow{title, GLFWWindow::CreateAsFullscreen{}};

        window.makeCurrent();

        loader.loadFunctions();

        return window;
    }

    auto createWindow(std::string_view title, Size<int> const & size)
        -> GLFWWindow
    {
        auto window = GLFWWindow{title, size};

        window.makeCurrent();

        loader.loadFunctions();

        return window;
    }

private:

    GLFWInitializer glfwInitializer;

    OpenGLLoader loader;

};

GLFWGateway::GLFWGateway(
    int const majorVersion,
    int const minorVersion,
    bool const enableDebugOutput)
    : impl{std::make_unique<Impl>(majorVersion, minorVersion, enableDebugOutput)}
{
}

GLFWGateway::GLFWGateway(GLFWGateway && rhs) noexcept = default;

auto GLFWGateway::operator = (GLFWGateway && rhs) noexcept
    -> GLFWGateway & = default;

GLFWGateway::~GLFWGateway() = default;

auto GLFWGateway::createWindow(std::string_view title, CreateAsFullscreen)
    -> GLFWWindow
{
    return impl->createWindow(title, CreateAsFullscreen{});
}

auto GLFWGateway::createWindow(std::string_view title, Size<int> const & size)
    -> GLFWWindow
{
    return impl->createWindow(title, size);
}

} // namespace ape
