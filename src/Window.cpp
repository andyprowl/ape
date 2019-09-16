#include "Window.hpp"

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

auto makeRegularGlfwWindow()
    -> GLFWwindow *
{
    return glfwCreateWindow(1024, 768, "LearnOpenGL", nullptr, nullptr);
}

auto makeFullScreenGlfwWindow()
    -> GLFWwindow *
{
    auto const monitor = glfwGetPrimaryMonitor();

    const auto mode = glfwGetVideoMode(monitor);
    
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);

    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);

    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);

    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    return glfwCreateWindow(
        mode->width,
        mode->height,
        "LearnOpenGL",
        glfwGetPrimaryMonitor(),
        nullptr);
}

auto makeGlfwWindow(bool const fullscreen)
    -> GLFWwindow &
{
    auto const window = fullscreen ? makeFullScreenGlfwWindow() : makeRegularGlfwWindow();

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

auto fitViewport(GLFWwindow & window)
    -> void
{
    auto const size = getWindowSize(window);

    glViewport(0, 0, size.width, size.height);
}

} // unnamed namespace

auto createWindow()
    -> GLFWwindow &
{
    initGLFW();

    auto & window = makeGlfwWindow(false);

    glfwMakeContextCurrent(&window);
    
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        throw CouldNotInitializeGLAD{};
    }

    printCapabilities();

    fitViewport(window);

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

auto getWindowCenter(GLFWwindow & window)
    -> Position
{
    auto width = int{};

    auto height = int{};

    glfwGetWindowSize(&window, &width, &height);

    return {width / 2.0, height / 2.0};
}

auto getWindowSize(GLFWwindow & window)
    -> Size<int>
{
    auto width = int{};

    auto height = int{};

    glfwGetWindowSize(&window, &width, &height);

    return {width, height};
}
