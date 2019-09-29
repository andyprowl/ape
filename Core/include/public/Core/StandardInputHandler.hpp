#pragma once

#include <Core/CameraManipulator.hpp>
#include <Core/InputHandler.hpp>
#include <Core/ScopedSignalConnection.hpp>

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

    virtual auto onMouseWheel(Offset<int> position)
        -> void;

    virtual auto onFocusAcquired()
        -> void;

    virtual auto onFocusLost()
        -> void;

private:

    auto registerKeyboardHandlerConnection()
        -> ScopedSignalConnection;

    auto registerMouseWheelHandlerConnection()
        -> ScopedSignalConnection;

    auto registerFocusAcquiredHandlerConnection()
        -> ScopedSignalConnection;

    auto registerFocusLostHandlerConnection()
        -> ScopedSignalConnection;

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

    CameraManipulator cameraManipulator;

    ScopedSignalConnection keyPressHandlerConnection;

    ScopedSignalConnection mouseWheelHandlerConnection;

    ScopedSignalConnection focusAcquiredHandlerConnection;

    ScopedSignalConnection focusLostHandlerConnection;

};

auto getCameraSelector(StandardInputHandler & handler)
    -> CameraSelector &;

auto getScene(StandardInputHandler const & handler)
    -> Scene &;

} // namespace ape
