#include <Core/StandardInputHandler.hpp>

#include <Core/CameraSelector.hpp>
#include <Core/Position.hpp>
#include <Core/Scene.hpp>
#include <Core/Window.hpp>

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
    Window & handledWindow,
    CameraSelector & cameraSelector,
    float const manipulatorSensitivity)
    : handledWindow{&handledWindow}
    , cameraManipulator{cameraSelector, handledWindow, manipulatorSensitivity}
    , keyPressHandlerConnection{handledWindow.onKeyboard.registerHandler(
        [this] (Key key, KeyAction action, KeyModifier modifier)
    {
        if (action == KeyAction::press)
        {
            onKeyPress(key, modifier);
        }
    })}
{
}

auto StandardInputHandler::getWindow() const
    -> Window &
{
    return *handledWindow;
}

auto StandardInputHandler::getCameraManipulator()
    -> CameraManipulator &
{
    return cameraManipulator;
}

auto StandardInputHandler::getCameraManipulator() const
    -> CameraManipulator const &
{
    return cameraManipulator;
}

// virtual (from InputHandler)
auto StandardInputHandler::onFrame(std::chrono::nanoseconds frameDuration)
    -> void
{
    cameraManipulator.onFrame(frameDuration);
}

// virtual (from InputHandler)
auto StandardInputHandler::onKeyPress(Key const key, KeyModifier const modifier)
    -> void
{
    processFullScreenToggling(key, modifier);

    processLightToggling(key, modifier);

    processCameraSwitching(key, modifier);
}

// virtual (from InputHandler)
auto StandardInputHandler::onKeyRelease(Key const /*key*/, KeyModifier const /*modifier*/)
    -> void
{
}

// virtual (from InputHandler)
auto StandardInputHandler::onMouseMove(Position<int> const /*position*/)
    -> void
{
}

// virtual (from InputHandler)
auto StandardInputHandler::onMouseWheel(Offset<int> const position)
    -> void
{
    cameraManipulator.onMouseWheel(position);
}

// virtual (from InputHandler)
auto StandardInputHandler::onFocusAcquired()
    -> void
{
    cameraManipulator.activate();
}

// virtual (from InputHandler)
auto StandardInputHandler::onFocusLost()
    -> void
{
    cameraManipulator.deactivate();
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

    if (!handledWindow->isFullScreen())
    {
        handledWindow->setFullScreen();
    }
    else
    {
        handledWindow->exitFullScreen();
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
    auto & cameraSelector = cameraManipulator.getCameraSelector();

    cameraSelector.activateCamera(index);
}

auto getCameraSelector(StandardInputHandler & handler)
    -> CameraSelector &
{
    auto & cameraManipulator = handler.getCameraManipulator();

    return cameraManipulator.getCameraSelector();
}

auto getScene(StandardInputHandler const & handler)
    -> Scene &
{
    auto const & cameraManipulator = handler.getCameraManipulator();

    auto const & cameraSelector = cameraManipulator.getCameraSelector();

    return cameraSelector.getScene();
}

} // namespace ape
