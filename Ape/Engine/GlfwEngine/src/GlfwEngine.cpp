#include <Ape/Engine/GlfwEngine/GlfwEngine.hpp>

#include "Glfw.hpp"

#include <Ape/Engine/GlfwEngine/GlfwWindow.hpp>

namespace ape
{

GlfwEngine::GlfwEngine(GlfwWindow & window, Renderer & renderer, InputHandler & inputHandler)
    : GlfwEngine{window, renderer, inputHandler, std::make_unique<EngineObjects>()}
{
}

GlfwEngine::GlfwEngine(
    GlfwWindow & window,
    Renderer & renderer,
    InputHandler & inputHandler,
    std::unique_ptr<EngineObjects> engineObjects)
    : Engine{
        window,
        engineObjects->eventSystem,
        renderer,
        inputHandler,
        engineObjects->imGuiDispatcher,
        engineObjects->frameLoop}
    , engineObjects{std::move(engineObjects)}
{
}

} // namespace ape
