#include <Ape/Engine/UpdateHandling/StandardInputHandler.hpp>

#include <Ape/Engine/Engine/Engine.hpp>
#include <Ape/Engine/Windowing/Window.hpp>

#include <Ape/Rendering/Effect/EffectSelector.hpp>
#include <Ape/Rendering/Rendering/SceneRenderer.hpp>
#include <Ape/Rendering/Skybox/SkyboxSelector.hpp>
#include <Ape/World/Scene/CameraSelector.hpp>
#include <Ape/World/Scene/Scene.hpp>

#include <Basix/Mathematics/Position.hpp>

#include <cassert>

namespace ape
{

StandardInputHandler::StandardInputHandler(
    Window & handledWindow,
    SceneRenderer & renderer,
    CameraSelector & cameraSelector,
    SkyboxSelector & skyboxSelector,
    EffectSelector & effectSelector,
    float const manipulatorSensitivity)
    : handledWindow{&handledWindow}
    , renderer{&renderer}
    , cameraManipulator{cameraSelector, handledWindow, manipulatorSensitivity}
    , skyboxSelector{&skyboxSelector}
    , effectSelector{&effectSelector}
    , engine{nullptr}
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

// virtual (from InputHandler)
auto StandardInputHandler::getEngine() const
    -> Engine &
{
    assert(engine != nullptr);

    return *engine;
}

// virtual (from InputHandler)
auto StandardInputHandler::setEngine(Engine & newEngine)
    -> void
{
    engine = &newEngine;
}

// virtual
auto StandardInputHandler::onKeyPress(Key const key, KeyModifier const modifier)
    -> void
{
    processInputCaptureToggling(key, modifier);

    processInspectionOverlayToggling(key, modifier);

    processFullScreenToggling(key, modifier);

    processFrustumCullingToggling(key, modifier);

    processOmniShadowModeToggling(key, modifier);

    processLightToggling(key, modifier);

    processCameraSwitching(key, modifier);

    processSkyboxSwitching(key, modifier);

    processEffectSwitching(key, modifier);
}

// virtual
auto StandardInputHandler::onKeyRelease(Key const /*key*/, KeyModifier const /*modifier*/)
    -> void
{
}

// virtual
auto StandardInputHandler::onMouseWheel(basix::Offset<int> const offset)
    -> void
{
    cameraManipulator.onMouseWheel(offset);
}

// virtual
auto StandardInputHandler::onFocusAcquired()
    -> void
{
    cameraManipulator.activate();

    getWindow().captureMouse();
}

// virtual
auto StandardInputHandler::onFocusLost()
    -> void
{
    cameraManipulator.deactivate();
}

auto StandardInputHandler::registerKeyboardHandlerConnection()
    -> basix::ScopedSignalConnection
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
    -> basix::ScopedSignalConnection
{
    return handledWindow->onMouseWheel.registerHandler([this] (basix::Offset<int> const & offset)
    {
        onMouseWheel(offset);
    });
}

auto StandardInputHandler::registerFocusAcquiredHandlerConnection()
    -> basix::ScopedSignalConnection
{
    return handledWindow->onFocusAcquired.registerHandler([this]
    {
        onFocusAcquired();
    });
}

auto StandardInputHandler::registerFocusLostHandlerConnection()
    -> basix::ScopedSignalConnection
{
    return handledWindow->onFocusLost.registerHandler([this]
    {
        onFocusLost();
    });
}

auto StandardInputHandler::processInputCaptureToggling(
    ape::Key const key, 
    KeyModifier const modifier)
    -> void
{
    if ((key != ape::Key::keyI) || (modifier != KeyModifier::none))
    {
        return;
    }

    if (handledWindow->isMouseCaptured())
    {
        handledWindow->releaseMouse();

        cameraManipulator.deactivate();
    }
    else
    {
        handledWindow->captureMouse();

        cameraManipulator.activate();
    }
}

auto StandardInputHandler::processInspectionOverlayToggling(
    ape::Key const key, 
    KeyModifier const modifier) const
    -> void
{
    if ((key != ape::Key::keyI) || (modifier != KeyModifier::control))
    {
        return;
    }

    if (engine->isInspectionOverlayVisible())
    {
        engine->hideInspectionOverlay();
    }
    else
    {
        engine->showInspectionOverlay();
    }
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

    if (not handledWindow->isFullScreen())
    {
        handledWindow->setFullScreen();
    }
    else
    {
        handledWindow->exitFullScreen();
    }
}

auto StandardInputHandler::processFrustumCullingToggling(
    ape::Key const key,
    ape::KeyModifier const modifier) const
    -> void
{
    if ((key != ape::Key::keyF) || (modifier != KeyModifier::none))
    {
        return;
    }

    auto const isFrustumCullingEnabled = renderer->isFrustumCullingEnabled();

    renderer->enableFrustumCulling(!isFrustumCullingEnabled);
}

auto StandardInputHandler::processOmniShadowModeToggling(
    ape::Key const key,
    ape::KeyModifier const modifier) const
    -> void
{
    if ((key != ape::Key::keyM) || (modifier != KeyModifier::none))
    {
        return;
    }

    auto & depthRenderer = renderer->getRenderers().depthBodyRenderer;

    auto const isOmniFlatMappingEnabled = depthRenderer.isOmniFlatShadowMappingEnabled();

    depthRenderer.enableOmniFlatShadowMapping(!isOmniFlatMappingEnabled);
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

auto StandardInputHandler::processSkyboxSwitching(
    ape::Key const key,
    ape::KeyModifier const modifier)
    -> void
{
    if (key != ape::Key::keyS)
    {
        return;
    }

    if (modifier == ape::KeyModifier::shift)
    {
        skyboxSelector->activateNextSkybox();
    }
    else
    {
        skyboxSelector->activatePreviousSkybox();
    }
}

auto StandardInputHandler::processEffectSwitching(
    ape::Key const key,
    ape::KeyModifier const modifier)
    -> void
{
    if (key != ape::Key::keyE)
    {
        return;
    }

    if (modifier == ape::KeyModifier::shift)
    {
        effectSelector->activateNextEffect();
    }
    else
    {
        effectSelector->activatePreviousEffect();
    }
}

auto StandardInputHandler::togglePointLight(int const index) const
    -> void
{
    auto & scene = getScene(*this);

    auto & lightSystem = scene.getLightSystem();

    if (index >= static_cast<int>(lightSystem.point.size()))
    {
        return;
    }

    lightSystem.point[index].toggle();
}

auto StandardInputHandler::toggleSpotLight(int const index) const
    -> void
{
    auto & scene = getScene(*this);

    auto & lightSystem = scene.getLightSystem();

    if (index >= static_cast<int>(lightSystem.spot.size()))
    {
        return;
    }

    lightSystem.spot[index].toggle();
}

auto StandardInputHandler::toggleDirectionalLight(int const index) const
    -> void
{
    auto & scene = getScene(*this);

    auto & lightSystem = scene.getLightSystem();

    if (index >= static_cast<int>(lightSystem.directional.size()))
    {
        return;
    }

    lightSystem.directional[index].toggle();
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
