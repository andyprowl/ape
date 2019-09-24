#pragma once

#include <Ape/CameraManipulator.hpp>
#include <Ape/InputHandler.hpp>
#include <Ape/ScopedSignalConnection.hpp>

#include <memory>

namespace ape
{

enum class Key;
enum class KeyModifier;

class CameraSelector;
class Scene;
class Window;

class StandardInputHandler : public InputHandler
{

public:

    StandardInputHandler(
        Window & window, 
        CameraSelector & cameraSelector,
        float manipulatorSensitivity = 0.1f);

    // virtual (from InputHandler)
    auto processInput(double lastFrameDuration)
        -> void override;

    auto getWindow() const
        -> Window &;

    auto getCameraManipulator() const
        -> CameraManipulator &;

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

    auto processCameraSwitching(ape::Key key, ape::KeyModifier modifier)
        -> void;

    auto togglePointLight(int index) const
        -> void;

    auto toggleSpotLight(int index) const
        -> void;

    auto switchToCamera(int index)
        -> void;

private:

    Window * handledWindow;

    std::unique_ptr<CameraManipulator> cameraManipulator;

    ScopedSignalConnection keyboardHandlerConnection;

};

auto getCameraSelector(StandardInputHandler const & handler)
    -> CameraSelector &;

auto getScene(StandardInputHandler const & handler)
    -> Scene &;

} // namespace ape
