#include <Ape/GLFWWindow.hpp>

#include "GLFW.hpp"

#include <iostream>
#include <unordered_map>

namespace ape
{

namespace
{

auto windowMap = std::unordered_map<GLFWwindow *, GLFWWindow *>{};

auto onResize(GLFWwindow * const handle, int const width, int const height)
    -> void
{
    auto const it = windowMap.find(handle);

    if (it == std::cend(windowMap))
    {
        return;
    }

    auto const window = it->second;

    window->onResize.fire(Size<int>{width, height});
}

auto onMouseWheel(GLFWwindow * const handle, double const /*xOffset*/, double const yOffset)
    -> void
{
    auto const it = windowMap.find(handle);

    if (it == std::cend(windowMap))
    {
        return;
    }

    auto const window = it->second;

    window->onMouseWheel.fire(yOffset);
}

auto onKeyboard(
    GLFWwindow * const handle,
    int const key,
    int const /*scancode*/,
    int const action,
    int const modifiers)
    -> void
{
    auto const it = windowMap.find(handle);

    if (it == std::cend(windowMap))
    {
        return;
    }

    auto const window = it->second;

    window->onKeyboard.fire(
        static_cast<Key>(key),
        static_cast<KeyAction>(action),
        static_cast<KeyModifier>(modifiers));
}

auto makeRegularGLFWWindow(std::string const & title)
    -> GLFWwindow *
{
    return glfwCreateWindow(1024, 768, title.c_str(), nullptr, nullptr);
}

auto makeFullScreenGLFWWindow(std::string const & title)
    -> GLFWwindow *
{
    auto const monitor = glfwGetPrimaryMonitor();

    auto const mode = glfwGetVideoMode(monitor);
    
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);

    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);

    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);

    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    return glfwCreateWindow(mode->width, mode->height, title.c_str(), monitor, nullptr);
}

auto makeGLFWWindow(std::string const & title, bool const fullscreen)
    -> GLFWwindow &
{
    auto const window = fullscreen 
        ? makeFullScreenGLFWWindow(title)
        : makeRegularGLFWWindow(title);

    if (window == nullptr)
    {
        throw CouldNotCreateWindow{};
    }

    return *window;
}

} // unnamed namespace

GLFWWindow::GLFWWindow(std::string const & title, bool const createAsFullScreen)
    : handle{&makeGLFWWindow(title, createAsFullScreen)}
    , isFullScreenModeOn{createAsFullScreen}
    , lastGLFWWindowedArea{getPosition(), getSize()}
{
    windowMap.emplace(handle, this);

    registerEventHandlers();
}

GLFWWindow::GLFWWindow(GLFWWindow && rhs) noexcept
    : handle{rhs.handle}
    , isFullScreenModeOn{rhs.isFullScreenModeOn}
    , lastGLFWWindowedArea{rhs.lastGLFWWindowedArea}
{
    windowMap[handle] = this;

    rhs.handle = nullptr;
}

auto GLFWWindow::operator = (GLFWWindow && rhs) noexcept
    -> GLFWWindow &
{
    windowMap[handle] = this;

    handle = rhs.handle;
    
    isFullScreenModeOn = rhs.isFullScreenModeOn;
    
    lastGLFWWindowedArea = rhs.lastGLFWWindowedArea;

    rhs.handle = nullptr;

    return *this;
}

GLFWWindow::~GLFWWindow() = default;

auto GLFWWindow::getAspectRatio() const
    -> float
{
    auto width = int{};

    auto height = int{};

    glfwGetWindowSize(handle, &width, &height);

    return (static_cast<float>(width) / height);
}

auto GLFWWindow::getCenter() const
    -> Position<double>
{
    auto width = int{};

    auto height = int{};

    glfwGetWindowSize(handle, &width, &height);

    return {width / 2.0, height / 2.0};
}

auto GLFWWindow::getSize() const
    -> Size<int>
{
    auto width = int{};

    auto height = int{};

    glfwGetWindowSize(handle, &width, &height);

    return {width, height};
}

auto GLFWWindow::getPosition() const
    -> Position<int>
{
    auto x = int{};

    auto y = int{};

    glfwGetWindowPos(handle, &x, &y);

    return {x, y};
}

auto GLFWWindow::getMousePosition() const
    -> Position<double>
{
    auto x = double{};

    auto y = double{};

    glfwGetCursorPos(handle, &x, &y);

    return {x, y};
}

auto GLFWWindow::isKeyPressed(Key const key) const
    -> bool
{
    return (glfwGetKey(handle, static_cast<int>(key)) == GLFW_PRESS);
}

auto GLFWWindow::shouldClose() const
    -> bool
{
    return glfwWindowShouldClose(handle);
}

auto GLFWWindow::requestClosure()
    -> void
{
    glfwSetWindowShouldClose(handle, true);
}

auto GLFWWindow::swapBuffers()
    -> void
{
    glfwSwapBuffers(handle);
}

auto GLFWWindow::isMouseCaptured() const
    -> bool
{
    return (glfwGetInputMode(handle, GLFW_CURSOR) == GLFW_CURSOR_DISABLED);
}

auto GLFWWindow::captureMouse()
    -> void
{
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

auto GLFWWindow::releaseMouse()
    -> void
{
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

auto GLFWWindow::isFullScreen() const
    -> bool
{
    return isFullScreenModeOn;
}

auto GLFWWindow::setFullScreen()
    -> void
{
    if (isFullScreen())
    {
        return;
    }

    lastGLFWWindowedArea = WindowArea{getPosition(), getSize()};

    auto const monitor = glfwGetPrimaryMonitor();

    auto const mode = glfwGetVideoMode(monitor);

    glfwSetWindowMonitor(handle, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

    isFullScreenModeOn = true;

    onResize.fire(lastGLFWWindowedArea.size);
}

auto GLFWWindow::exitFullScreen()
    -> void
{
    if (!isFullScreen())
    {
        return;
    }

    glfwSetWindowMonitor(
        handle,
        nullptr,
        lastGLFWWindowedArea.position.x,
        lastGLFWWindowedArea.position.y,
        lastGLFWWindowedArea.size.width,
        lastGLFWWindowedArea.size.height, GLFW_DONT_CARE);

    isFullScreenModeOn = false;

    onResize.fire(lastGLFWWindowedArea.size);
}

auto GLFWWindow::makeCurrent()
    -> void
{
    glfwMakeContextCurrent(handle);
}

auto GLFWWindow::pollEvents()
    -> void
{
    glfwPollEvents();
}

auto GLFWWindow::registerEventHandlers()
    -> void
{
    glfwSetScrollCallback(handle, ape::onMouseWheel);

    glfwSetFramebufferSizeCallback(handle, ape::onResize);

    glfwSetKeyCallback(handle, ape::onKeyboard);
}

} // namespace ape
