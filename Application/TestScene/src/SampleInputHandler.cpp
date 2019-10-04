#include <TestScene/SampleInputHandler.hpp>

#include <TestScene/SampleScene.hpp>

#include <Engine/BodySelector.hpp>
#include <Engine/CameraSelector.hpp>
#include <Engine/StandardShaderProgram.hpp>
#include <Engine/Window.hpp>
#include <Engine/WireframeShaderProgram.hpp>

#include <CompilerSupport/CompilerWarnings.hpp>

#include <Mathematics/Math.hpp>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

namespace
{

auto rotateBodyAroundOwnX(ape::Body & body, float const radians)
    -> void
{
    auto const & transformation = ape::getTransformation(body);

    auto const newTransformation = glm::rotate(transformation, radians, {1.0f, 0.0f, 0.0f});
    
    ape::setTransformation(body, newTransformation);
}

auto rotateBodyAroundWorldY(ape::Body & body, float const radians)
    -> void
{
    auto const position = ape::getPosition(body);
    
    auto const revolution = glm::rotate(glm::mat4{1.0f}, radians, {0.0f, 1.0f, 0.0f});

    auto const newPosition = glm::vec3{revolution * glm::vec4{position, 1.0f}};

    auto const newTransformation  = 
        glm::translate(glm::mat4{1.0f}, newPosition) *
        glm::rotate(glm::mat4{1.0f}, radians, {0.0f, 1.0f, 0.0f}) *
        glm::translate(glm::mat4{1.0f}, -position) *
        getTransformation(body);

    ape::setTransformation(body, newTransformation);
}

} // unnamed namespace

SampleInputHandler::SampleInputHandler(
    ape::Window & window,
    ape::CameraSelector & cameraSelector,
    ape::BodySelector & bodyPicker,
    ape::StandardShaderProgram & standardShader,
    ape::WireframeShaderProgram & wireframeShader,
    maybeUnused SampleScene & scene)
    : StandardInputHandler{window, cameraSelector}
    , standardShader{&standardShader}
    , wireframeShader{&wireframeShader}
    , bodyPicker{&bodyPicker}
{
    assert(&scene == &cameraSelector.getScene());
}

auto SampleInputHandler::getScene() const
    -> SampleScene &
{
    auto & scene = ape::getScene(*this);

    return static_cast<SampleScene &>(scene);
}

// virtual (from InputHandler)
auto SampleInputHandler::onFrame(std::chrono::nanoseconds const frameDuration)
    -> void
{
    StandardInputHandler::onFrame(frameDuration);

    auto const frameDurationInSeconds = frameDuration.count() / 1'000'000'000.0;

    processShapeModification(frameDurationInSeconds);

    processLightRevolution(frameDurationInSeconds);
}

auto SampleInputHandler::onKeyPress(ape::Key const key, ape::KeyModifier const modifier)
    -> void
{
    StandardInputHandler::onKeyPress(key, modifier);

    if ((key == ape::Key::keyB) && (modifier == ape::KeyModifier::none))
    {
        toggleBlinnPhongModel();
    }
    else if ((key == ape::Key::keyP) && (modifier == ape::KeyModifier::none))
    {
        togglePickedObjects();
    }
    else if ((key == ape::Key::keyQ) && (modifier == ape::KeyModifier::control))
    {
        increaseWireframeLineWidth(0.01f);
    }
    else if ((key == ape::Key::keyE) && (modifier == ape::KeyModifier::control))
    {
        increaseWireframeLineWidth(-0.01f);
    }
    else if (key == ape::Key::keyEscape)
    {
        getWindow().close();
    }
}

auto SampleInputHandler::processShapeModification(double const lastFrameDuration) const
    -> void
{
    processShapeRotation(lastFrameDuration);

    processShapeScaling(lastFrameDuration);
}

auto SampleInputHandler::processShapeRotation(double const lastFrameDuration) const
    -> void
{
    auto & scene = getScene();

    auto const & window = getWindow();

    auto const rotationDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    if (window.isKeyPressed(ape::Key::keyA))
    {
        rotateBodyAroundOwnX(*scene.rotatingContainer, +rotationDelta);
    }
    else if (window.isKeyPressed(ape::Key::keyD))
    {
        rotateBodyAroundOwnX(*scene.rotatingContainer, -rotationDelta);
    }
}

auto SampleInputHandler::processShapeScaling(double const lastFrameDuration) const
    -> void
{
    auto & scene = getScene();

    auto const & window = getWindow();

    auto const scalingDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    if (window.isKeyPressed(ape::Key::keyS))
    {
        ape::scaleUniformly(*scene.scalingContainer, 1 + scalingDelta);
    }
    else if (window.isKeyPressed(ape::Key::keyX))
    {
        ape::scaleUniformly(*scene.scalingContainer, 1 - scalingDelta);
    }
}

auto SampleInputHandler::processLightRevolution(double const lastFrameDuration) const
    -> void
{
    auto & lampBody = *getScene().lamps.front();

    auto const & window = getWindow();

    auto const rotationDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    if (window.isKeyPressed(ape::Key::keyO))
    {
        rotateBodyAroundWorldY(lampBody, +rotationDelta);
    }
    else if (window.isKeyPressed(ape::Key::keyI))
    {
        rotateBodyAroundWorldY(lampBody, -rotationDelta);
    }
}

auto SampleInputHandler::toggleBlinnPhongModel() const
    -> void
{
    standardShader->use();

    standardShader->useBlinnPhongModel = !standardShader->useBlinnPhongModel;
}

auto SampleInputHandler::togglePickedObjects() const
    -> void
{
    auto & scene = getScene();

    auto const areObjectsPicked = bodyPicker->isBodySelected(*scene.rotatingContainer);

    if (areObjectsPicked)
    {
        bodyPicker->deselectAllBodies();
    }
    else
    {
        pickObjects();
    }
}

auto SampleInputHandler::pickObjects() const
    -> void
{
    auto & scene = getScene();

    bodyPicker->selectBody(*scene.rotatingContainer);

    bodyPicker->selectBody(*scene.scalingContainer);

    bodyPicker->selectBody(*scene.dragon);

    bodyPicker->selectBody(*scene.nanosuit);

    bodyPicker->selectBody(*scene.lamps[0]);
}

auto SampleInputHandler::increaseWireframeLineWidth(float amount) const
    -> void
{
    auto const newWidth = wireframeShader->lineWidth.get() + amount;

    wireframeShader->lineWidth = ape::clamp(newWidth, 0.0f, 0.1f);
}
