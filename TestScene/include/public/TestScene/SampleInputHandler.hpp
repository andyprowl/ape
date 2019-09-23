#pragma once

#include <Ape/BlinnPhongKeySwitcher.hpp>
#include <Ape/CameraManipulator.hpp>
#include <Ape/StandardInputHandler.hpp>

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

class SampleInputHandler : public ape::StandardInputHandler
{

public:

    SampleInputHandler(
        ape::Window & window,
        SampleScene & scene,
        ape::StandardShaderProgram & shader);

    auto getScene() const
        -> SampleScene &;

private:

    // virtual (from StandardInputHandler)
    auto onProcessInput(double lastFrameDuration)
        -> void override;

    // virtual (from StandardInputHandler)
    auto onKeyPressed(ape::Key key, ape::KeyModifier modifier)
        -> bool override;

    auto processShapeModification(double lastFrameDuration) const
        -> void;

    auto processShapeRotation(double lastFrameDuration) const
        -> void;

    auto processShapeScaling(double lastFrameDuration) const
        -> void;

    auto processLightRevolution(double lastFrameDuration) const
        -> void;

    auto processMouseCapture(ape::Key const key, ape::KeyModifier const modifier) const
        -> bool;

private:

    ape::BlinnPhongKeySwitcher blinnPhongSwitcher;

};
