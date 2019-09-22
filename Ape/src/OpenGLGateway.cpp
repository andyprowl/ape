#include <Ape/OpenGLGateway.hpp>

#include "GLFWInitializer.hpp"

#include "GLFW.hpp"

namespace ape
{

class OpenGLGateway::Impl
{

public:

    Impl()
        : areOpenGLFunctionsLoaded{false}
    {
    }

    auto createWindow(std::string const & title, bool createAsFullscreen)
        -> Window
    {
        auto window = Window{title, createAsFullscreen};

        window.makeCurrent();

        if (!areOpenGLFunctionsLoaded)
        {
            loadOpenGLFunctions();
        }

        return window;
    }

private:

    auto loadOpenGLFunctions()
        -> void
    {
        //if (!gladLoadGL())
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            throw CouldNotLoadOpenGLFunctions{};
        }
    }

private:

    GLFWInitializer glfwInitializer;

    bool areOpenGLFunctionsLoaded;

};

OpenGLGateway::OpenGLGateway()
    : impl{std::make_unique<Impl>()}
{
}

OpenGLGateway::OpenGLGateway(OpenGLGateway && rhs) noexcept = default;

auto OpenGLGateway::operator = (OpenGLGateway && rhs) noexcept
    -> OpenGLGateway & = default;

OpenGLGateway::~OpenGLGateway() = default;

auto OpenGLGateway::createWindow(std::string const & title, bool const createAsFullscreen)
    -> Window
{
    return impl->createWindow(title, createAsFullscreen);
}

} // namespace ape
