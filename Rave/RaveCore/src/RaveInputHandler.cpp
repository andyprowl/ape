#include <Rave/RaveCore/RaveInputHandler.hpp>

#include <Rave/RaveCore/RaveScene.hpp>

#include <Ape/Rendering/LineStyleProvider.hpp>
#include <Ape/Rendering/StandardShaderProgram.hpp>
#include <Ape/Scene/BodySelector.hpp>
#include <Ape/Scene/CameraSelector.hpp>
#include <Ape/Windowing/Window.hpp>

#include <Foundational/CompilerSupport/CompilerWarnings.hpp>
#include <Foundational/Mathematics/Math.hpp>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

namespace rave
{

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

auto getFunctionKey(int const i)
    -> ape::Key
{
    return static_cast<ape::Key>(static_cast<int>(ape::Key::keyF1) + i);
}

} // unnamed namespace

RaveInputHandler::RaveInputHandler(
    ape::Window & window,
    ape::CameraSelector & cameraSelector,
    ape::SkyboxSelector & skyboxSelector,
    ape::EffectSelector & effectSelector,
    ape::BodySelector & bodyPicker,
    ape::StandardShaderProgram & standardShader,
    ape::LineStyleProvider & outlineStyleProvider,
    maybeUnused RaveScene & scene)
    : StandardInputHandler{window, cameraSelector, skyboxSelector, effectSelector}
    , bodyPicker{&bodyPicker}
    , standardShader{&standardShader}
    , outlineStyleProvider{&outlineStyleProvider}
{
    assert(&scene == &cameraSelector.getScene());
}

auto RaveInputHandler::getScene() const
    -> RaveScene &
{
    auto & scene = ape::getScene(*this);

    return static_cast<RaveScene &>(scene);
}

// virtual (from InputHandler)
auto RaveInputHandler::onFrame(std::chrono::nanoseconds const frameDuration)
    -> void
{
    StandardInputHandler::onFrame(frameDuration);

    auto const frameDurationInSeconds = frameDuration.count() / 1'000'000'000.0;

    processShapeModification(frameDurationInSeconds);

    processLightRevolution(frameDurationInSeconds);
}

auto RaveInputHandler::onKeyPress(ape::Key const key, ape::KeyModifier const modifier)
    -> void
{
    StandardInputHandler::onKeyPress(key, modifier);

    if ((key == ape::Key::keyB) && (modifier == ape::KeyModifier::none))
    {
        toggleBlinnPhongModel();
    }
    else if ((key == ape::Key::keyF) && (modifier == ape::KeyModifier::none))
    {
        togglePercentageCloserFiltering();
    }
    else if ((key == ape::Key::keyP) && (modifier == ape::KeyModifier::none))
    {
        togglePickedObjects();
    }
    else if ((key == ape::Key::keyQ) && (modifier == ape::KeyModifier::control))
    {
        increaseOutlineWidth(0.01f);
    }
    else if ((key == ape::Key::keyE) && (modifier == ape::KeyModifier::control))
    {
        increaseOutlineWidth(-0.01f);
    }
    else if (key == ape::Key::keyEscape)
    {
        getWindow().close();
    }
}

auto RaveInputHandler::processShapeModification(double const lastFrameDuration) const
    -> void
{
    processShapeRotation(lastFrameDuration);

    processShapeScaling(lastFrameDuration);
}

auto RaveInputHandler::processShapeRotation(double const lastFrameDuration) const
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

auto RaveInputHandler::processShapeScaling(double const lastFrameDuration) const
    -> void
{
    auto & scene = getScene();

    auto const & window = getWindow();

    auto const scalingDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    if (window.isKeyPressed(ape::Key::keyZ))
    {
        ape::scaleUniformly(*scene.scalingContainer, 1 + scalingDelta);
    }
    else if (window.isKeyPressed(ape::Key::keyC))
    {
        ape::scaleUniformly(*scene.scalingContainer, 1 - scalingDelta);
    }
}

auto RaveInputHandler::processLightRevolution(double const lastFrameDuration) const
    -> void
{
    auto const & window = getWindow();

    auto const rotationDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    for (auto i = 0; i <= 9; ++i)
    {
        auto const keyCode = static_cast<ape::Key>(static_cast<int>(ape::Key::keyF1) + i);

        if (window.isKeyPressed(ape::Key::keyLeftShift))
        {
            auto const & lampBodies = getScene().lamps;

            if (i < static_cast<int>(lampBodies.size()) && window.isKeyPressed(getFunctionKey(i)))
            {
                rotateBodyAroundWorldY(*lampBodies[i], +rotationDelta);
            }
        }
        else
        {
            auto const & lampBodies = getScene().flashlights;

            if (i < static_cast<int>(lampBodies.size()) && window.isKeyPressed(getFunctionKey(i)))
            {
                rotateBodyAroundWorldY(*lampBodies[i], +rotationDelta);
            }
        }
    }
}

auto RaveInputHandler::toggleBlinnPhongModel() const
    -> void
{
    standardShader->bind();

    standardShader->useBlinnPhongModel = !standardShader->useBlinnPhongModel;
}

auto RaveInputHandler::togglePercentageCloserFiltering() const
    -> void
{
    standardShader->bind();

    standardShader->usePercentageCloserFiltering = !standardShader->usePercentageCloserFiltering;
}

auto RaveInputHandler::togglePickedObjects() const
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

auto RaveInputHandler::pickObjects() const
    -> void
{
    auto & scene = getScene();

    bodyPicker->selectBody(*scene.rotatingContainer);

    bodyPicker->selectBody(*scene.scalingContainer);

    bodyPicker->selectBody(*scene.dragon);

    bodyPicker->selectBody(*scene.nanosuit);

    bodyPicker->selectBody(*scene.lamps.front());
}

auto RaveInputHandler::increaseOutlineWidth(float amount) const
    -> void
{
    auto const outliningStyle = outlineStyleProvider->getStyle();

    auto const newWidth = ape::clamp(outliningStyle.width + amount, 0.0f, 0.1f);

    auto const newStyle = ape::LineStyle{newWidth, outliningStyle.color};

    outlineStyleProvider->setStyle(newStyle);
}

} // namespace rave