#pragma once

#include <InputHandling/CameraManipulator.hpp>
#include <InputHandling/StandardInputHandler.hpp>

#include <vector>

namespace ape
{

enum class Key;
enum class KeyAction;
enum class KeyModifier;

class BodySelector;
class CameraSelector;
class LineStyleProvider;
class StandardShaderProgram;
class Window;
class WireframeShaderProgram;

} // namespace ape

class SampleScene;

class SampleInputHandler : public ape::StandardInputHandler
{

public:

    SampleInputHandler(
        ape::Window & window,
        ape::CameraSelector & cameraSelector,
        ape::BodySelector & bodyPicker,
        ape::StandardShaderProgram & standardShader,
        ape::LineStyleProvider & outlineStyleProvider,
        SampleScene & scene);

    auto getScene() const
        -> SampleScene &;

private:

    // virtual (from StandardInputHandler)
    auto onFrame(std::chrono::nanoseconds frameDuration)
        -> void override;
    
    // virtual (from InputHandler)
    auto onKeyPress(ape::Key key, ape::KeyModifier modifier)
        -> void override;

    auto processShapeModification(double lastFrameDuration) const
        -> void;

    auto processShapeRotation(double lastFrameDuration) const
        -> void;

    auto processShapeScaling(double lastFrameDuration) const
        -> void;

    auto processLightRevolution(double lastFrameDuration) const
        -> void;

    auto toggleBlinnPhongModel() const
        -> void;

    auto togglePickedObjects() const
        -> void;

    auto pickObjects() const
        -> void;

    auto increaseOutlineWidth(float amount) const
        -> void;

private:

    ape::BodySelector * bodyPicker;

    ape::StandardShaderProgram * standardShader;

    ape::LineStyleProvider * outlineStyleProvider;

};
