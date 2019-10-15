#include <InputHandling/StandardInputHandler.hpp>

#include <InputHandling/Window.hpp>

#include <Scene/CameraSelector.hpp>
#include <Scene/Scene.hpp>

#include <Mathematics/Position.hpp>

namespace ape
{

StandardInputHandler::StandardInputHandler(
    Window & handledWindow,
    CameraSelector & cameraSelector,
    float const manipulatorSensitivity)
    : handledWindow{&handledWindow}
    , cameraManipulator{cameraSelector, handledWindow, manipulatorSensitivity}
    , keyPressHandlerConnection{registerKeyboardHandlerConnection()}
    , mouseWheelHandlerConnection{registerMouseWheelHandlerConnection()}
    , focusAcquiredHandlerConnection{registerFocusAcquiredHandlerConnection()}
    , focusLostHandlerConnection{registerFocusLostHandlerConnection()}
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

// virtual
auto StandardInputHandler::onKeyPress(Key const key, KeyModifier const modifier)
    -> void
{
    processFullScreenToggling(key, modifier);

    processLightToggling(key, modifier);

    processCameraSwitching(key, modifier);
}

// virtual
auto StandardInputHandler::onKeyRelease(Key const /*key*/, KeyModifier const /*modifier*/)
    -> void
{
}

// virtual
auto StandardInputHandler::onMouseWheel(Offset<int> const offset)
    -> void
{
    cameraManipulator.onMouseWheel(offset);
}

// virtual
auto StandardInputHandler::onFocusAcquired()
    -> void
{
    cameraManipulator.activate();
}

// virtual
auto StandardInputHandler::onFocusLost()
    -> void
{
    cameraManipulator.deactivate();
}

auto StandardInputHandler::registerKeyboardHandlerConnection()
    -> ScopedSignalConnection
{
    return handledWindow->onKeyboard.registerHandler(
        [this] (Key const key, KeyAction const action, KeyModifier const modifier)
    {
        if (action == KeyAction::press)
        {
            onKeyPress(key, modifier);
        }
        else
        {
            onKeyRelease(key, modifier);
        }
    });
}

auto StandardInputHandler::registerMouseWheelHandlerConnection()
    -> ScopedSignalConnection
{
    return handledWindow->onMouseWheel.registerHandler([this] (Offset<int> const & offset)
    {
        onMouseWheel(offset);
    });
}

auto StandardInputHandler::registerFocusAcquiredHandlerConnection()
    -> ScopedSignalConnection
{
    return handledWindow->onFocusAcquired.registerHandler([this]
    {
        onFocusAcquired();
    });
}

auto StandardInputHandler::registerFocusLostHandlerConnection()
    -> ScopedSignalConnection
{
    return handledWindow->onFocusLost.registerHandler([this]
    {
        onFocusLost();
    });
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
    else if (modifier == ape::KeyModifier::alt)
    {
        toggleDirectionalLight(index);
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

    auto & lighting = scene.getLighting();

    if (index >= static_cast<int>(lighting.point.size()))
    {
        return;
    }

    lighting.point[index].toggle();
}

auto StandardInputHandler::toggleSpotLight(int const index) const
    -> void
{
    auto & scene = getScene(*this);

    auto & lighting = scene.getLighting();

    if (index >= static_cast<int>(lighting.spot.size()))
    {
        return;
    }

    lighting.spot[index].toggle();
}

auto StandardInputHandler::toggleDirectionalLight(int const index) const
    -> void
{
    auto & scene = getScene(*this);

    auto & lighting = scene.getLighting();

    if (index >= static_cast<int>(lighting.directional.size()))
    {
        return;
    }

    lighting.directional[index].toggle();
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
