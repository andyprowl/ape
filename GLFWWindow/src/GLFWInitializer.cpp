#include "GLFWInitializer.hpp"

#include "GLFW.hpp"

namespace ape
{

namespace
{

auto initGLFW()
    -> void
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // MacOS: glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

} // unnamed namespace

GLFWInitializer::GLFWInitializer()
{
    initGLFW();
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
