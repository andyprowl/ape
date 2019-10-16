#pragma once

#include <QObject>

#include <memory>

namespace ape
{

class InputHandler;
class SceneRenderer;
class Window;

} // namespace ape

namespace ape::qt
{

class QtEngine
{

public:

    QtEngine(Window & window, SceneRenderer & renderer, InputHandler & inputHandler);

    QtEngine(QtEngine const &) = delete;

    QtEngine(QtEngine &&) noexcept;

    auto operator = (QtEngine const &) 
        -> QtEngine & = delete;

    auto operator = (QtEngine &&) noexcept
        -> QtEngine &;

    ~QtEngine();

    auto start()
        -> void;

    auto stop()
        -> void;

private:

    class Impl;

private:

    std::unique_ptr<Impl> impl;

};

} // namespace ape::qt
