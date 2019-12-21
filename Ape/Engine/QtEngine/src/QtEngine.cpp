#include <Ape/Engine/QtEngine/QtEngine.hpp>

#include <Ape/Engine/QtEngine/QtWindow.hpp>

namespace ape::qt
{

QtEngine::QtEngine(QtWindow & window, Renderer & renderer, InputHandler & inputHandler)
    : QtEngine{window, renderer, inputHandler, std::make_unique<EngineObjects>()}
{
}

QtEngine::QtEngine(
    QtWindow & window,
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

} // namespace ape::qt
