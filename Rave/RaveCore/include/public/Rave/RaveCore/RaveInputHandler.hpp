#pragma once

#include <Ape/Engine/UpdateHandling/CameraManipulator.hpp>
#include <Ape/Engine/UpdateHandling/StandardInputHandler.hpp>

#include <vector>

namespace ape
{

enum class Key;
enum class KeyAction;
enum class KeyModifier;

class BodySelector;
class CameraSelector;
class SkyboxSelector;
class EffectSelector;
class LineStyleProvider;
class LightingShaderProgram;
class Window;
class WireframeShaderProgram;

} // namespace ape

namespace rave
{

class RaveScene;

class RaveInputHandler : public ape::StandardInputHandler
{

public:

    RaveInputHandler(
        ape::Window & window,
        ape::CameraSelector & cameraSelector,
        ape::SkyboxSelector & skyboxSelector,
        ape::EffectSelector & effectSelector,
        ape::BodySelector & bodyPicker,
        ape::LightingShaderProgram & standardShader,
        ape::LineStyleProvider & outlineStyleProvider,
        RaveScene & scene);

    auto getScene() const
        -> RaveScene &;

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

    auto togglePercentageCloserFiltering() const
        -> void;

    auto togglePickedObjects() const
        -> void;

    auto pickObjects() const
        -> void;

    auto increaseOutlineWidth(float amount) const
        -> void;

private:

    ape::BodySelector * bodyPicker;

    ape::LightingShaderProgram * standardShader;

    ape::LineStyleProvider * outlineStyleProvider;

};

} // namespace rave
