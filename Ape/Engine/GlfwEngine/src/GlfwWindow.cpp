#include <Ape/Engine/GlfwEngine/GlfwWindow.hpp>

#include "Glfw.hpp"

#include <unordered_map>

namespace ape
{

namespace
{

auto windowMap = std::unordered_map<GLFWwindow *, GlfwWindow *>{};

auto onResize(GLFWwindow * const handle, int const width, int const height)
    -> void
{
    auto const it = windowMap.find(handle);

    if (it == std::cend(windowMap))
    {
        return;
    }

    auto const window = it->second;

    window->onResize.fire(basix::Size2d<int>{width, height});
}

auto onMouseWheel(GLFWwindow * const handle, double const xOffset, double const yOffset)
    -> void
{
    auto const it = windowMap.find(handle);

    if (it == std::cend(windowMap))
    {
        return;
    }

    auto const window = it->second;

    auto const offset = basix::Offset2d<int>{static_cast<int>(xOffset), static_cast<int>(yOffset)};

    window->onMouseWheel.fire(offset);
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

auto onFocusChange(GLFWwindow * const handle, int const status)
{
    auto const it = windowMap.find(handle);

    if (it == std::cend(windowMap))
    {
        return;
    }

    auto const window = it->second;

    if (status == GL_TRUE)
    {
        window->onFocusAcquired.fire();
    }
    else
    {
        window->onFocusLost.fire();
    }
}

auto makeRegularGLFWWindow(std::string_view title, basix::Size2d<int> const & size)
    -> GLFWwindow &
{
    auto const w = glfwCreateWindow(size.width, size.height, title.data(), nullptr, nullptr);

    if (w == nullptr)
    {
        throw CouldNotCreateWindow{};
    }

    return *w;
}

auto makeFullScreenGLFWWindow(std::string_view title)
    -> GLFWwindow &
{
    auto const monitor = glfwGetPrimaryMonitor();

    auto const mode = glfwGetVideoMode(monitor);
    
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);

    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);

    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);

    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    auto const w = glfwCreateWindow(mode->width, mode->height, title.data(), monitor, nullptr);

    if (w == nullptr)
    {
        throw CouldNotCreateWindow{};
    }

    return *w;
}

} // unnamed namespace

GlfwWindow::GlfwWindow(std::string_view title, CreateAsFullscreen const)
    : GlfwWindow{makeFullScreenGLFWWindow(title), true}
{
}

GlfwWindow::GlfwWindow(std::string_view title, basix::Size2d<int> const & size)
    : GlfwWindow{makeRegularGLFWWindow(title, size), false}
{
}

GlfwWindow::GlfwWindow(GlfwWindow && rhs) noexcept
    : handle{rhs.handle}
    , isFullScreenModeOn{rhs.isFullScreenModeOn}
    , lastGLFWWindowedArea{rhs.lastGLFWWindowedArea}
{
    windowMap[handle] = this;

    rhs.handle = nullptr;
}

auto GlfwWindow::operator = (GlfwWindow && rhs) noexcept
    -> GlfwWindow &
{
    windowMap[handle] = this;

    handle = rhs.handle;
    
    isFullScreenModeOn = rhs.isFullScreenModeOn;
    
    lastGLFWWindowedArea = rhs.lastGLFWWindowedArea;

    rhs.handle = nullptr;

    return *this;
}

GlfwWindow::~GlfwWindow() = default;

auto GlfwWindow::getGlfwHandle() const
    -> GLFWwindow *
{
    return handle;
}

// virtual (from Window)
auto GlfwWindow::getAspectRatio() const
    -> float
{
    auto width = int{};

    auto height = int{};

    glfwGetWindowSize(handle, &width, &height);

    return (static_cast<float>(width) / height);
}

// virtual (from Window)
auto GlfwWindow::getSize() const
    -> basix::Size2d<int>
{
    auto width = int{};

    auto height = int{};

    glfwGetWindowSize(handle, &width, &height);

    return {width, height};
}

// virtual (from Window)
auto GlfwWindow::getPosition() const
    -> basix::Position2d<int>
{
    auto x = int{};

    auto y = int{};

    glfwGetWindowPos(handle, &x, &y);

    return {x, y};
}

// virtual (from Window)
auto GlfwWindow::getMousePosition() const
    -> basix::Position2d<int>
{
    auto x = double{};

    auto y = double{};

    glfwGetCursorPos(handle, &x, &y);

    return {static_cast<int>(x), static_cast<int>(y)};
}

auto GlfwWindow::isFullScreen() const
    -> bool
{
    return isFullScreenModeOn;
}

auto GlfwWindow::setFullScreen()
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

    onResize.fire(basix::Size2d<int>{mode->width, mode->height});
}

auto GlfwWindow::exitFullScreen()
    -> void
{
    if (not isFullScreen())
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

auto GlfwWindow::isKeyPressed(Key const key) const
    -> bool
{
    return (glfwGetKey(handle, static_cast<int>(key)) == GLFW_PRESS);
}

auto GlfwWindow::swapBuffers()
    -> void
{
    glfwSwapBuffers(handle);
}

auto GlfwWindow::isMouseCaptured() const
    -> bool
{
    return (glfwGetInputMode(handle, GLFW_CURSOR) == GLFW_CURSOR_DISABLED);
}

auto GlfwWindow::captureMouse()
    -> void
{
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

auto GlfwWindow::releaseMouse()
    -> void
{
    glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

auto GlfwWindow::makeCurrent()
    -> void
{
    glfwMakeContextCurrent(handle);
}

auto GlfwWindow::close()
    -> void
{
    glfwSetWindowShouldClose(handle, true);
}

auto GlfwWindow::isClosing()
    -> bool
{
    return glfwWindowShouldClose(handle);
}

GlfwWindow::GlfwWindow(GLFWwindow & handle, bool const isFullScreen)
    : handle{&handle}
    , isFullScreenModeOn{isFullScreen}
    , lastGLFWWindowedArea{getPosition(), getSize()}
{
    auto const size = getSize();

    glfwSetCursorPos(&handle, size.width / 2, size.height / 2);

    windowMap.emplace(&handle, this);

    registerEventHandlers();
}

auto GlfwWindow::registerEventHandlers()
    -> void
{
    glfwSetScrollCallback(handle, ape::onMouseWheel);

    glfwSetFramebufferSizeCallback(handle, ape::onResize);

    glfwSetKeyCallback(handle, ape::onKeyboard);

    glfwSetWindowFocusCallback(handle, ape::onFocusChange);
}

} // namespace ape
