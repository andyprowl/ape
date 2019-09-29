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

    // virtual (from InputHandler)
    auto onKeyPress(Key key, KeyModifier modifier)
        -> void override;

    // virtual (from InputHandler)
    auto onKeyRelease(Key key, KeyModifier modifier)
        -> void override;

    // virtual (from InputHandler)
    auto onMouseMove(Position<int> position)
        -> void override;

    // virtual (from InputHandler)
    auto onMouseWheel(Offset<int> position)
        -> void override;

    // virtual (from InputHandler)
    auto onFocusAcquired()
        -> void override;

    // virtual (from InputHandler)
    auto onFocusLost()
        -> void override;

private:

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

};

auto getCameraSelector(StandardInputHandler & handler)
    -> CameraSelector &;

auto getScene(StandardInputHandler const & handler)
    -> Scene &;

} // namespace ape
