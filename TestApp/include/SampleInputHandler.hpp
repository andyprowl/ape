#pragma once

#include <Ape/BlinnPhongKeySwitcher.hpp>
#include <Ape/CameraManipulator.hpp>
#include <Ape/InputHandler.hpp>
#include <Ape/MouseTracker.hpp>

#include <vector>

enum class Key;
enum class KeyAction;
enum class KeyModifier;

class SampleScene;
class StandardShaderProgram;
class Window;

class SampleInputHandler : public InputHandler
{

public:

    SampleInputHandler(Window & window, SampleScene & scene, StandardShaderProgram & shader);
    
    // virtual (from InputHandler)
    auto processInput(double lastFrameDuration)
        -> void override;

private:

    auto registerKeyboardEventHandler() const
        -> ScopedSignalConnection;

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

    auto onKeyboardEvent(Key key, KeyAction action, KeyModifier modifier) const
        -> void;

    auto processLightToggling(Key key, KeyModifier const modifier) const
        -> void;

    auto processCameraSwitching(Key key, KeyModifier const modifier) const
        -> void;

    auto processFullScreenToggling(Key key) const
        -> void;

    auto togglePointLight(int index) const
        -> void;

    auto toggleSpotLight(int index) const
        -> void;

    auto switchToCamera(int index) const
        -> void;

private:

    Window * window;

    SampleScene * scene;

    CameraManipulator cameraManipulator;

    BlinnPhongKeySwitcher blinnPhongSwitcher;

    ScopedSignalConnection keyboardHandlerConnection;

};
