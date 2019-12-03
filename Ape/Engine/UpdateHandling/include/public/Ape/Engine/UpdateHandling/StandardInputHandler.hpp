#pragma once

#include <Ape/Engine/UpdateHandling/CameraManipulator.hpp>
#include <Ape/Engine/UpdateHandling/InputHandler.hpp>

#include <Basix/Signal/ScopedSignalConnection.hpp>

#include <memory>

namespace ape
{

enum class Key;
enum class KeyModifier;

class CameraSelector;
class EffectSelector;
class Scene;
class SceneRenderer;
class SkyboxSelector;
class Window;

class StandardInputHandler : public InputHandler
{

public:

    StandardInputHandler(
        Window & window,
        SceneRenderer & renderer,
        CameraSelector & cameraSelector,
        SkyboxSelector & skyboxSelector,
        EffectSelector & effectSelector,
        float manipulatorSensitivity = 0.1f);

    auto getWindow() const
        -> Window &;

    auto getCameraManipulator()
        -> CameraManipulator &;

    auto getCameraManipulator() const
        -> CameraManipulator const &;

    // virtual (from InputHandler)
    auto onFrame(std::chrono::nanoseconds frameDuration)
        -> void override;

protected:

    virtual auto onKeyPress(Key key, KeyModifier modifier)
        -> void;

    virtual auto onKeyRelease(Key key, KeyModifier modifier)
        -> void;

    virtual auto onMouseWheel(basix::Offset<int> position)
        -> void;

    virtual auto onFocusAcquired()
        -> void;

    virtual auto onFocusLost()
        -> void;

private:

    auto registerKeyboardHandlerConnection()
        -> basix::ScopedSignalConnection;

    auto registerMouseWheelHandlerConnection()
        -> basix::ScopedSignalConnection;

    auto registerFocusAcquiredHandlerConnection()
        -> basix::ScopedSignalConnection;

    auto registerFocusLostHandlerConnection()
        -> basix::ScopedSignalConnection;

    auto processInputCaptureToggling(ape::Key const key, KeyModifier const modifier) const
        -> void;

    auto processFullScreenToggling(ape::Key const key, KeyModifier modifier) const
        -> void;

    auto processFrustumCullingToggling(ape::Key key, ape::KeyModifier modifier) const
        -> void;

    auto processOmniShadowModeToggling(ape::Key key, ape::KeyModifier modifier) const
        -> void;

    auto processLightToggling(ape::Key key, ape::KeyModifier modifier) const
        -> void;

    auto processCameraSwitching(ape::Key key, ape::KeyModifier modifier)
        -> void;

    auto processSkyboxSwitching(ape::Key key, ape::KeyModifier modifier)
        -> void;

    auto processEffectSwitching(ape::Key key, ape::KeyModifier modifier)
        -> void;

    auto togglePointLight(int index) const
        -> void;

    auto toggleSpotLight(int index) const
        -> void;

    auto toggleDirectionalLight(int index) const
        -> void;

    auto switchToCamera(int index)
        -> void;

private:

    Window * handledWindow;

    SceneRenderer * renderer;

    CameraManipulator cameraManipulator;
        
    SkyboxSelector * skyboxSelector;

    EffectSelector * effectSelector;

    basix::ScopedSignalConnection keyPressHandlerConnection;

    basix::ScopedSignalConnection mouseWheelHandlerConnection;

    basix::ScopedSignalConnection focusAcquiredHandlerConnection;

    basix::ScopedSignalConnection focusLostHandlerConnection;

};

auto getCameraSelector(StandardInputHandler & handler)
    -> CameraSelector &;

auto getScene(StandardInputHandler const & handler)
    -> Scene &;

} // namespace ape
