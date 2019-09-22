#include <Ape/StandardInputHandler.hpp>

#include <Ape/Scene.hpp>
#include <Ape/Window.hpp>

namespace ape
{

auto toggle(bool & b)
    -> void
{
    b = !b;
}

StandardInputHandler::StandardInputHandler(
    Window & window,
    Scene & scene,
    float const manipulatorSensitivity)
    : handledWindow{&window}
    , handledScene{&scene}
    , cameraManipulator{scene, window, manipulatorSensitivity}
    , keyboardHandlerConnection{registerKeyboardHandler()}
{
}

// virtual (from InputHandler)
auto StandardInputHandler::processInput(double lastFrameDuration)
    -> void
{
    cameraManipulator.update(lastFrameDuration);

    onProcessInput(lastFrameDuration);
}

auto StandardInputHandler::getScene() const
    -> Scene &
{
    return *handledScene;
}

auto StandardInputHandler::getWindow() const
    -> Window &
{
    return *handledWindow;
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
    ape::KeyModifier const modifier) const
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
    auto & scene = getScene();

    if (index >= static_cast<int>(scene.lighting.point.size()))
    {
        return;
    }

    toggle(scene.lighting.point[index].isTurnedOn);
}

auto StandardInputHandler::toggleSpotLight(int const index) const
    -> void
{
    auto & scene = getScene();

    if (index >= static_cast<int>(scene.lighting.spot.size()))
    {
        return;
    }

    toggle(scene.lighting.spot[index].isTurnedOn);
}

auto StandardInputHandler::switchToCamera(int const index) const
    -> void
{
    auto & scene = getScene();

    auto & cameras = scene.cameraSystem.cameras;

    if (index >= static_cast<int>(cameras.size()))
    {
        return;
    }

    auto const & window = getWindow();

    scene.cameraSystem.activeCamera = &cameras[index];

    scene.cameraSystem.activeCamera->setAspectRatio(window.getAspectRatio());
}

} // namespace ape
