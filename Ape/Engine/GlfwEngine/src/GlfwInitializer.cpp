#include "GlfwInitializer.hpp"

#include "Glfw.hpp"

namespace ape
{

namespace
{

auto initGLFW(int const majorVersion, int const minorVersion)
    -> void
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // MacOS: glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

} // unnamed namespace

GlfwInitializer::GlfwInitializer(int const majorVersion, int const minorVersion)
    : isInitialized{true}
{
    initGLFW(majorVersion, minorVersion);
}

GlfwInitializer::GlfwInitializer(GlfwInitializer && rhs) noexcept
    : isInitialized{rhs.isInitialized}
{
    rhs.isInitialized = false;
}

auto GlfwInitializer::operator = (GlfwInitializer && rhs) noexcept
    -> GlfwInitializer &
{
    isInitialized = rhs.isInitialized;

    rhs.isInitialized = false;

    return *this;
}

GlfwInitializer::~GlfwInitializer()
{
    if (isInitialized)
    {
        glfwTerminate();
    }
}

} // namespace ape
