#pragma once

#include <Ape/BlinnPhongKeySwitcher.hpp>
#include <Ape/CameraManipulator.hpp>
#include <Ape/InputHandler.hpp>
#include <Ape/MouseTracker.hpp>

#include <vector>

namespace ape
{

enum class Key;
enum class KeyAction;
enum class KeyModifier;

class StandardShaderProgram;
class Window;

} // namespace ape

class SampleScene;

class SampleInputHandler : public ape::InputHandler
{

public:

    SampleInputHandler(
        ape::Window & window,
        SampleScene & scene,
        ape::StandardShaderProgram & shader);
    
    // virtual (from InputHandler)
    auto processInput(double lastFrameDuration)
        -> void override;

private:

    auto registerKeyboardEventHandler() const
        -> ape::ScopedSignalConnection;

    auto processTerminationRequest() const
        -> void;

    auto processMouseCapture() const
        -> void;

    auto processCameraManipulation(double lastFrameDuration)
        -> void;

    auto processShapeModification(double lastFrameDuration) const
        -> void;

    auto processShapeRotation(double lastFrameDuration) const
        -> void;

    auto processShapeScaling(double lastFrameDuration) const
        -> void;

    auto processLightRevolution(double lastFrameDuration) const
        -> void;

    auto onKeyboardEvent(ape::Key key, ape::KeyAction action, ape::KeyModifier modifier) const
        -> void;

    auto processLightToggling(ape::Key key, ape::KeyModifier modifier) const
        -> void;

    auto processCameraSwitching(ape::Key key, ape::KeyModifier modifier) const
        -> void;

    auto processFullScreenToggling(ape::Key key) const
        -> void;

    auto togglePointLight(int index) const
        -> void;

    auto toggleSpotLight(int index) const
        -> void;

    auto switchToCamera(int index) const
        -> void;

private:

    ape::Window * window;

    SampleScene * scene;

    ape::CameraManipulator cameraManipulator;

    ape::BlinnPhongKeySwitcher blinnPhongSwitcher;

    ape::ScopedSignalConnection keyboardHandlerConnection;

};
