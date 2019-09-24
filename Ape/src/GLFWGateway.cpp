#include <Ape/GLFWGateway.hpp>

#include "GLFWInitializer.hpp"

#include "GLFW.hpp"

namespace ape
{

class GLFWGateway::Impl
{

public:

    Impl()
        : areOpenGLFunctionsLoaded{false}
    {
    }

    auto createWindow(std::string const & title, bool createAsFullscreen)
        -> GLFWWindow
    {
        auto window = GLFWWindow{title, createAsFullscreen};

        window.makeCurrent();

        if (!areOpenGLFunctionsLoaded)
        {
            initializeOpenGL();
        }

        return window;
    }

    auto initializeOpenGL()
        -> void
    {
        if (areOpenGLFunctionsLoaded)
        {
            return;
        }

        if (!gladLoadGL())
        {
            throw CouldNotLoadOpenGLFunctions{};
        }

        areOpenGLFunctionsLoaded = true;
    }

private:

    GLFWInitializer glfwInitializer;

    bool areOpenGLFunctionsLoaded;

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

auto GLFWGateway::initializeOpenGL()
    -> void
{
    return impl->initializeOpenGL();
}

} // namespace ape
