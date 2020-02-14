#pragma once

#include <Ape/Engine/Engine/Engine.hpp>
#include <ape/Engine/QtEngine/QtEventFrameLoop.hpp>
#include <Ape/Engine/QtEngine/QtEventSystem.hpp>
#include <Ape/Engine/QtEngine/QtImGuiEventDispatcher.hpp>

#include <memory>

namespace ape
{

class InputHandler;
class Renderer;

} // namespace ape

namespace ape::qt
{

class QtWindow;

class QtEngine : public Engine
{

public:

    QtEngine(
        QtWindow & window,
        std::unique_ptr<Renderer> renderer,
        std::unique_ptr<InputHandler> inputHandler);

private:

    class EngineObjects
    {

    public:

        QtEventSystem eventSystem;

        QtImGuiEventDispatcher imGuiDispatcher;

        QtEventFrameLoop frameLoop;

    };

private:

    QtEngine(
        QtWindow & window,
        std::unique_ptr<Renderer> renderer,
        std::unique_ptr<InputHandler> inputHandler,
        std::unique_ptr<EngineObjects> engineObjects);

private:

    std::shared_ptr<EngineObjects const> engineObjects;

};

} // namespace ape::qt
