#pragma once

#include <ape/Engine/Engine/BusyFrameLoop.hpp>
#include <Ape/Engine/Engine/Engine.hpp>
#include <Ape/Engine/GlfwEngine/GlfwEventSystem.hpp>
#include <Ape/Engine/GlfwEngine/GlfwImGuiEventDispatcher.hpp>

#include <memory>

namespace ape
{

class GlfwWindow;
class InputHandler;
class Renderer;

class GlfwEngine : public Engine
{

public:

    GlfwEngine(
        GlfwWindow & window,
        std::unique_ptr<Renderer> renderer,
        std::unique_ptr<InputHandler> inputHandler);

private:

    class EngineObjects
    {

    public:

        GlfwEventSystem eventSystem;

        GlfwImGuiEventDispatcher imGuiDispatcher;

        BusyFrameLoop frameLoop;

    };

private:

    GlfwEngine(
        GlfwWindow & window,
        std::unique_ptr<Renderer> renderer,
        std::unique_ptr<InputHandler> inputHandler,
        std::unique_ptr<EngineObjects> engineObjects);

private:

    std::shared_ptr<EngineObjects const> engineObjects;

};

} // namespace ape
