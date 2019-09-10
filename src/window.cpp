#include "window.hpp"

#include <iostream>

namespace
{

auto initGLFW()
    -> void
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // MacOS: glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

auto printCapabilities()
    -> void
{
    auto numOfAttributes = int{};

    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numOfAttributes);

    std::cout << "Maximum number of vertex attributes: " << numOfAttributes << "\n";
}

auto makeGlfwWindow()
    -> GLFWwindow &
{
    auto window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    
    if (window == nullptr)
    {
        throw CouldNotCreateWindow{};
    }

    return *window;
}

auto onResize(GLFWwindow * const window, int const width, int const height)
    -> void
{
    glViewport(0, 0, width, height);
}

} // unnamed namespace

auto createWindow()
    -> GLFWwindow &
{
    initGLFW();

    auto & window = makeGlfwWindow();

    glfwMakeContextCurrent(&window);
    
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        throw CouldNotInitializeGLAD{};
    }

    printCapabilities();

    glViewport(0, 0, 800, 600);

    glEnable(GL_DEPTH_TEST);

    glfwSetFramebufferSizeCallback(&window, onResize);

    return window;
}

auto getWindowRatio(GLFWwindow & window)
    -> float
{
    auto width = int{};

    auto height = int{};

    glfwGetWindowSize(&window, &width, &height);

    return (static_cast<float>(width) / height);
}
