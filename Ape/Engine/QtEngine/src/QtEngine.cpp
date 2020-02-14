#include <Ape/Engine/QtEngine/QtEngine.hpp>

#include <Ape/Engine/QtEngine/QtWindow.hpp>

#include <Ape/Engine/Engine/InputHandler.hpp>
#include <Ape/Engine/Engine/Renderer.hpp>

namespace ape::qt
{

QtEngine::QtEngine(
    QtWindow & window,
    std::unique_ptr<Renderer> renderer,
    std::unique_ptr<InputHandler> inputHandler)
    : QtEngine{
        window,
        std::move(renderer),
        std::move(inputHandler),
        std::make_unique<EngineObjects>()}
{
}

QtEngine::QtEngine(
    QtWindow & window,
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

} // namespace ape::qt
