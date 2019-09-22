#pragma once

#include <Ape/CameraManipulator.hpp>
#include <Ape/InputHandler.hpp>
#include <Ape/ScopedSignalConnection.hpp>

namespace ape
{

enum class Key;
enum class KeyModifier;

class StandardInputHandler : public InputHandler
{

public:

    StandardInputHandler(Window & window, Scene & scene, float manipulatorSensitivity = 0.1f);

    // virtual (from InputHandler)
    auto processInput(double lastFrameDuration)
        -> void override;

    auto getScene() const
        -> Scene &;

    auto getWindow() const
        -> Window &;

private:

    virtual auto onProcessInput(double lastFrameDuration)
        -> void;

    virtual auto onKeyPressed(Key key, KeyModifier modifier)
        -> bool;

    auto registerKeyboardHandler()
        -> ScopedSignalConnection;

    auto handleKeyPress(Key key, KeyModifier modifier)
        -> void;

    auto processFullScreenToggling(ape::Key const key, KeyModifier modifier) const
        -> void;

    auto processLightToggling(ape::Key key, ape::KeyModifier modifier) const
        -> void;

    auto processCameraSwitching(ape::Key key, ape::KeyModifier modifier) const
        -> void;

    auto togglePointLight(int index) const
        -> void;

    auto toggleSpotLight(int index) const
        -> void;

    auto switchToCamera(int index) const
        -> void;

private:

    Window * handledWindow;

    Scene * handledScene;

    CameraManipulator cameraManipulator;

    ScopedSignalConnection keyboardHandlerConnection;

};

} // namespace ape
