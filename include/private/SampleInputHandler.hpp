#pragma once

#include "CameraManipulator.hpp"
#include "MouseTracker.hpp"

#include <vector>

class SampleScene;
class ShaderProgram;
class Window;

class SampleInputHandler
{

public:

    SampleInputHandler(SampleScene & scene, Window & window, ShaderProgram const & program);

    auto processInput(double lastFrameDuration)
        -> void;

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

    auto onKeyboardEvent(int key, int scancode, int action, int mods) const
        -> void;

    auto processLightToggling(int key, int mods) const
        -> void;

    auto processCameraSwitching(int key, int mods) const
        -> void;

    auto processFullScreenToggling(int key) const
        -> void;

    auto togglePointLight(int index) const
        -> void;

    auto toggleSpotLight(int index) const
        -> void;

    auto switchToCamera(int index) const
        -> void;

private:

    SampleScene * scene;

    Window * window;

    ShaderProgram const * program;

    CameraManipulator cameraManipulator;

    ScopedSignalConnection keyboardHandlerConnection;

};
