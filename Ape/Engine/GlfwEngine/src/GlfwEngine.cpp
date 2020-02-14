#include <Ape/Engine/GlfwEngine/GlfwEngine.hpp>

#include "Glfw.hpp"

#include <Ape/Engine/GlfwEngine/GlfwWindow.hpp>

#include <Ape/Engine/Engine/InputHandler.hpp>
#include <Ape/Engine/Engine/Renderer.hpp>

namespace ape
{

GlfwEngine::GlfwEngine(
    GlfwWindow & window,
    std::unique_ptr<Renderer> renderer,
    std::unique_ptr<InputHandler> inputHandler)
    : GlfwEngine{
        window,
        std::move(renderer),
        std::move(inputHandler),
        std::make_unique<EngineObjects>()}
{
}

GlfwEngine::GlfwEngine(
    GlfwWindow & window,
    std::unique_ptr<Renderer> renderer,
    std::unique_ptr<InputHandler> inputHandler,
    std::unique_ptr<EngineObjects> engineObjects)
    : Engine{
        window,
        engineObjects->eventSystem,
        std::move(renderer),
        std::move(inputHandler),
        engineObjects->imGuiDispatcher,
        engineObjects->frameLoop}
    , engineObjects{std::move(engineObjects)}
{
}

} // namespace ape
