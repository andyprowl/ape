#include <Ape/Engine/GlfwEngine/GlfwGateway.hpp>

#include "Glfw.hpp"
#include "GlfwInitializer.hpp"

#include <Ape/Engine/Initialization/OpenGLLoader.hpp>

namespace ape
{

class GlfwGateway::Impl
{

public:

    Impl(int const majorVersion, int const minorVersion, bool const enableDebugOutput)
        : glfwInitializer{majorVersion, minorVersion}
        , loader{false, enableDebugOutput}
    {
    }

    auto createWindow(std::string_view title, CreateAsFullscreen)
        -> GlfwWindow
    {
        auto window = GlfwWindow{title, GlfwWindow::CreateAsFullscreen{}};

        window.makeCurrent();

        loader.loadFunctions();

        return window;
    }

    auto createWindow(std::string_view title, basix::Size2d<int> const & size)
        -> GlfwWindow
    {
        auto window = GlfwWindow{title, size};

        window.makeCurrent();

        loader.loadFunctions();

        return window;
    }

private:

    GlfwInitializer glfwInitializer;

    OpenGLLoader loader;

};

GlfwGateway::GlfwGateway(
    int const majorVersion,
    int const minorVersion,
    bool const enableDebugOutput)
    : impl{std::make_unique<Impl>(majorVersion, minorVersion, enableDebugOutput)}
{
}

GlfwGateway::GlfwGateway(GlfwGateway && rhs) noexcept = default;

auto GlfwGateway::operator = (GlfwGateway && rhs) noexcept
    -> GlfwGateway & = default;

GlfwGateway::~GlfwGateway() = default;

auto GlfwGateway::createWindow(std::string_view title, CreateAsFullscreen)
    -> GlfwWindow
{
    return impl->createWindow(title, CreateAsFullscreen{});
}

auto GlfwGateway::createWindow(std::string_view title, basix::Size2d<int> const & size)
    -> GlfwWindow
{
    return impl->createWindow(title, size);
}

} // namespace ape
