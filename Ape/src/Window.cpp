#include <Ape/Window.hpp>

#include "GLFW.hpp"

#include <iostream>

namespace
{

auto theWindow = static_cast<Window *>(nullptr);

auto onResize(GLFWwindow * const /*window*/, int const width, int const height)
    -> void
{
    theWindow->onResize.fire(Size<int>{width, height});
}

auto onMouseWheel(GLFWwindow * const /*window*/, double const /*xOffset*/, double const yOffset)
    -> void
{
    theWindow->onMouseWheel.fire(yOffset);
}

auto onKeyboard(
    GLFWwindow * const /*window*/,
    int const key,
    int const /*scancode*/,
    int const action,
    int const modifiers)
    -> void
{
    theWindow->onKeyboard.fire(
        static_cast<Key>(key),
        static_cast<KeyAction>(action),
        static_cast<KeyModifier>(modifiers));
}

auto makeRegularGlfwWindow(std::string const & title)
    -> GLFWwindow *
{
    return glfwCreateWindow(1024, 768, title.c_str(), nullptr, nullptr);
}

auto makeFullScreenGlfwWindow(std::string const & title)
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

auto makeGlfwWindow(std::string const & title, bool const fullscreen)
    -> GLFWwindow &
{
    auto const window = fullscreen 
        ? makeFullScreenGlfwWindow(title)
        : makeRegularGlfwWindow(title);

    if (window == nullptr)
    {
        throw CouldNotCreateWindow{};
    }

    return *window;
}

} // unnamed namespace

Window::Window(std::string const & title, bool const createAsFullScreen)
    : handle{&makeGlfwWindow(title, createAsFullScreen)}
    , isFullScreenModeOn{createAsFullScreen}
    , lastWindowedArea{getPosition(), getSize()}
{
    theWindow = this;

    registerEventHandlers();
}

Window::Window(Window && rhs) noexcept
    : handle{rhs.handle}
    , isFullScreenModeOn{rhs.isFullScreenModeOn}
    , lastWindowedArea{rhs.lastWindowedArea}
{
    theWindow = this;

    rhs.handle = nullptr;
}

auto Window::operator = (Window && rhs) noexcept
    -> Window &
{
    theWindow = this;

    handle = rhs.handle;
    
    isFullScreenModeOn = rhs.isFullScreenModeOn;
    
    lastWindowedArea = rhs.lastWindowedArea;

    rhs.handle = nullptr;

    return *this;
}

Window::~Window() = default;

auto Window::getAspectRatio() const
    -> float
{
    auto width = int{};

    auto height = int{};

    glfwGetWindowSize(handle, &width, &height);

    return (static_cast<float>(width) / height);
}

auto Window::getCenter() const
    -> Position<double>
{
    auto width = int{};

    auto height = int{};

    glfwGetWindowSize(handle, &width, &height);

    return {width / 2.0, height / 2.0};
}

auto Window::getSize() const
    -> Size<int>
{
    auto width = int{};

    auto height = int{};

    glfwGetWindowSize(handle, &width, &height);

    return {width, height};
}

auto Window::getPosition() const
    -> Position<int>
{
    auto x = int{};

    auto y = int{};

    glfwGetWindowPos(handle, &x, &y);

    return {x, y};
}

auto Window::getMousePosition() const
    -> Position<double>
{
    auto x = double{};

    auto y = double{};

    glfwGetCursorPos(handle, &x, &y);

    return {x, y};
}

auto Window::isKeyPressed(Key const key) const
    -> bool
{
    return (glfwGetKey(handle, static_cast<int>(key)) == GLFW_PRESS);
}

auto Window::shouldClose() const
    -> bool
{
    return glfwWindowShouldClose(handle);
}

auto Window::requestClosure()
    -> void
{
    glfwSetWindowShouldClose(handle, true);
}

auto Window::swapBuffers()
    -> void
{
    glfwSwapBuffers(handle);
}

auto Window::captureMouse()
    -> void
{
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

auto Window::releaseMouse()
    -> void
{
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

auto Window::isFullScreen() const
    -> bool
{
    return isFullScreenModeOn;
}

auto Window::setFullScreen()
    -> void
{
    if (isFullScreen())
    {
        return;
    }

    lastWindowedArea = WindowArea{getPosition(), getSize()};

    auto const monitor = glfwGetPrimaryMonitor();

    auto const mode = glfwGetVideoMode(monitor);

    glfwSetWindowMonitor(handle, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

    isFullScreenModeOn = true;

    onResize.fire(lastWindowedArea.size);
}

auto Window::exitFullScreen()
    -> void
{
    if (!isFullScreen())
    {
        return;
    }

    glfwSetWindowMonitor(
        handle,
        nullptr,
        lastWindowedArea.position.x,
        lastWindowedArea.position.y,
        lastWindowedArea.size.width,
        lastWindowedArea.size.height, GLFW_DONT_CARE);

    isFullScreenModeOn = false;

    onResize.fire(lastWindowedArea.size);
}

auto Window::makeCurrent()
    -> void
{
    glfwMakeContextCurrent(handle);
}

auto Window::registerEventHandlers()
    -> void
{
    glfwSetScrollCallback(handle, ::onMouseWheel);

    glfwSetFramebufferSizeCallback(handle, ::onResize);

    glfwSetKeyCallback(handle, ::onKeyboard);
}