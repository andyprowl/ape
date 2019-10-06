#include "GLFWInitializer.hpp"

#include "GLFW.hpp"

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

GLFWInitializer::GLFWInitializer(int const majorVersion, int const minorVersion)
    : isInitialized{true}
{
    initGLFW(majorVersion, minorVersion);
}

GLFWInitializer::GLFWInitializer(GLFWInitializer && rhs) noexcept
    : isInitialized{rhs.isInitialized}
{
    rhs.isInitialized = false;
}

auto GLFWInitializer::operator = (GLFWInitializer && rhs) noexcept
    -> GLFWInitializer &
{
    isInitialized = rhs.isInitialized;

    rhs.isInitialized = false;

    return *this;
}

GLFWInitializer::~GLFWInitializer()
{
    if (isInitialized)
    {
        glfwTerminate();
    }
}

} // namespace ape
