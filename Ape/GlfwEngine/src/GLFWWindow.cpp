#include <Ape/GlfwEngine/GLFWWindow.hpp>

#include "GLFW.hpp"

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

auto onMouseWheel(GLFWwindow * const handle, double const xOffset, double const yOffset)
    -> void
{
    auto const it = windowMap.find(handle);

    if (it == std::cend(windowMap))
    {
        return;
    }

    auto const window = it->second;

    window->onMouseWheel.fire(Offset<int>{static_cast<int>(xOffset), static_cast<int>(yOffset)});
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

auto makeRegularGLFWWindow(std::string_view title, Size<int> const & size)
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

GLFWWindow::GLFWWindow(std::string_view title, CreateAsFullscreen const)
    : GLFWWindow{makeFullScreenGLFWWindow(title), true}
{
}

GLFWWindow::GLFWWindow(std::string_view title, Size<int> const & size)
    : GLFWWindow{makeRegularGLFWWindow(title, size), false}
{
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

// virtual (from Window)
auto GLFWWindow::getAspectRatio() const
    -> float
{
    auto width = int{};

    auto height = int{};

    glfwGetWindowSize(handle, &width, &height);

    return (static_cast<float>(width) / height);
}

// virtual (from Window)
auto GLFWWindow::getSize() const
    -> Size<int>
{
    auto width = int{};

    auto height = int{};

    glfwGetWindowSize(handle, &width, &height);

    return {width, height};
}

// virtual (from Window)
auto GLFWWindow::getPosition() const
    -> Position<int>
{
    auto x = int{};

    auto y = int{};

    glfwGetWindowPos(handle, &x, &y);

    return {x, y};
}

// virtual (from Window)
auto GLFWWindow::getMousePosition() const
    -> Position<int>
{
    auto x = double{};

    auto y = double{};

    glfwGetCursorPos(handle, &x, &y);

    return {static_cast<int>(x), static_cast<int>(y)};
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

    onResize.fire(Size<int>{mode->width, mode->height});
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

auto GLFWWindow::isKeyPressed(Key const key) const
    -> bool
{
    return (glfwGetKey(handle, static_cast<int>(key)) == GLFW_PRESS);
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

auto GLFWWindow::makeCurrent()
    -> void
{
    glfwMakeContextCurrent(handle);
}

auto GLFWWindow::close()
    -> void
{
    glfwSetWindowShouldClose(handle, true);
}

auto GLFWWindow::isClosing()
    -> bool
{
    return glfwWindowShouldClose(handle);
}

GLFWWindow::GLFWWindow(GLFWwindow & handle, bool const isFullScreen)
    : handle{&handle}
    , isFullScreenModeOn{isFullScreen}
    , lastGLFWWindowedArea{getPosition(), getSize()}
{
    windowMap.emplace(&handle, this);

    registerEventHandlers();
}

auto GLFWWindow::registerEventHandlers()
    -> void
{
    glfwSetScrollCallback(handle, ape::onMouseWheel);

    glfwSetFramebufferSizeCallback(handle, ape::onResize);

    glfwSetKeyCallback(handle, ape::onKeyboard);

    glfwSetWindowFocusCallback(handle, ape::onFocusChange);
}

} // namespace ape
