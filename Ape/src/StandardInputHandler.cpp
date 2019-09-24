#include <Ape/StandardInputHandler.hpp>

#include <Ape/CameraSelector.hpp>
#include <Ape/Scene.hpp>
#include <Ape/Window.hpp>

namespace ape
{

namespace
{

auto toggle(bool & b)
    -> void
{
    b = !b;
}

} // unnamed namespace

StandardInputHandler::StandardInputHandler(
    Window & window,
    CameraSelector & cameraSelector,
    float const manipulatorSensitivity)
    : handledWindow{&window}
    , cameraManipulator{std::make_unique<CameraManipulator>(
        cameraSelector,
        window,
        manipulatorSensitivity)}
    , keyboardHandlerConnection{registerKeyboardHandler()}
{
}

// virtual (from InputHandler)
auto StandardInputHandler::processInput(double lastFrameDuration)
    -> void
{
    cameraManipulator->update(lastFrameDuration);

    onProcessInput(lastFrameDuration);
}

auto StandardInputHandler::getWindow() const
    -> Window &
{
    return *handledWindow;
}

auto StandardInputHandler::getCameraManipulator() const
    -> CameraManipulator &
{
    return *cameraManipulator;
}

auto StandardInputHandler::onProcessInput(double const /*lastFrameDuration*/)
    -> void
{
    // Derived classes may override this.
}

auto StandardInputHandler::onKeyPressed(Key const /*key*/, KeyModifier const /*modifier*/)
    -> bool
{
    // Derived classes may override this.

    auto const wasHandled = false;

    return wasHandled;
}

auto StandardInputHandler::registerKeyboardHandler()
    -> ScopedSignalConnection
{
    return handledWindow->onKeyboard.registerHandler(
        [this] (Key const key, KeyAction const action, KeyModifier const modifier)
    {
        if (action == KeyAction::press)
        {
            auto const wasHandled = onKeyPressed(key, modifier);

            if (wasHandled)
            {
                return;
            }

            handleKeyPress(key, modifier);
        }
    });
}

auto StandardInputHandler::handleKeyPress(Key const key, const KeyModifier modifier)
    -> void
{
    processFullScreenToggling(key, modifier);

    processLightToggling(key, modifier);

    processCameraSwitching(key, modifier);
}

auto StandardInputHandler::processFullScreenToggling(
    ape::Key const key, 
    KeyModifier const modifier) const
    -> void
{
    if ((key != ape::Key::keyF11) || (modifier != KeyModifier::none))
    {
        return;
    }

    auto & window = getWindow();

    if (!window.isFullScreen())
    {
        window.setFullScreen();
    }
    else
    {
        window.exitFullScreen();
    }
}

auto StandardInputHandler::processLightToggling(
    ape::Key const key,
    ape::KeyModifier const modifier) const
    -> void
{
    if ((key < ape::Key::key1) || (key > ape::Key::key9))
    {
        return;
    }

    auto const index = static_cast<int>(key) - static_cast<int>(ape::Key::key1);

    if (modifier == ape::KeyModifier::shift)
    {
        togglePointLight(index);
    }
    else if (modifier == ape::KeyModifier::none)
    {
        toggleSpotLight(index);
    }
}

auto StandardInputHandler::processCameraSwitching(
    ape::Key const key,
    ape::KeyModifier const modifier)
    -> void
{
    if ((key < ape::Key::key1) || (key > ape::Key::key9))
    {
        return;
    }

    auto const index = static_cast<int>(key) - static_cast<int>(ape::Key::key1);

    if (modifier == ape::KeyModifier::control)
    {
        switchToCamera(index);
    }
}

auto StandardInputHandler::togglePointLight(int const index) const
    -> void
{
    auto & scene = getScene(*this);

    if (index >= static_cast<int>(scene.lighting.point.size()))
    {
        return;
    }

    toggle(scene.lighting.point[index].isTurnedOn);
}

auto StandardInputHandler::toggleSpotLight(int const index) const
    -> void
{
    auto & scene = getScene(*this);

    if (index >= static_cast<int>(scene.lighting.spot.size()))
    {
        return;
    }

    toggle(scene.lighting.spot[index].isTurnedOn);
}

auto StandardInputHandler::switchToCamera(int const index)
    -> void
{
    auto & cameraSelector = cameraManipulator->getCameraSelector();

    cameraSelector.activateCamera(index);
}

auto getCameraSelector(StandardInputHandler const & handler)
    -> CameraSelector &
{
    return handler.getCameraManipulator().getCameraSelector();
}

auto getScene(StandardInputHandler const & handler)
    -> Scene &
{
    return getCameraSelector(handler).getScene();
}

} // namespace ape
