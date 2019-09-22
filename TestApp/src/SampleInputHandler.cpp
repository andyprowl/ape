#include "SampleInputHandler.hpp"

#include <Ape/Camera.hpp>
#include "SampleScene.hpp"
#include <Ape/ShaderProgram.hpp>
#include <Ape/Window.hpp>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

namespace
{

auto toggle(bool & b)
    -> void
{
    b = !b;
}

auto rotateBodyAroundOwnX(Body & body, float const radians)
    -> void
{
    auto const & transformation = getTransformation(body);

    auto const newTransformation = glm::rotate(transformation, radians, {1.0f, 0.0f, 0.0f});
    
    setTransformation(body, newTransformation);
}

auto rotateLightAroundWorldY(SampleScene & scene, float const radians)
    -> void
{
    auto & body = *scene.lamps.front();

    auto const position = getPosition(body);
    
    auto const revolution = glm::rotate(glm::mat4{1.0f}, radians, {0.0f, 1.0f, 0.0f});

    auto const newPosition = glm::vec3{revolution * glm::vec4{position, 1.0f}};

    auto const newTransformation  = 
        glm::translate(glm::mat4{1.0f}, newPosition) *
        glm::rotate(glm::mat4{1.0f}, radians, {0.0f, 1.0f, 0.0f}) *
        glm::translate(glm::mat4{1.0f}, -position) *
        getTransformation(body);

    setTransformation(body, newTransformation);
}

} // unnamed namespace

SampleInputHandler::SampleInputHandler(
    Window & window,
    SampleScene & scene,
    StandardShaderProgram & shader)
    : window{&window}
    , scene{&scene}
    , cameraManipulator{scene, window, 0.1f}
    , blinnPhongSwitcher{window, shader}
    , keyboardHandlerConnection{registerKeyboardEventHandler()}
{
}

auto SampleInputHandler::registerKeyboardEventHandler() const
    -> ScopedSignalConnection
{
    return window->onKeyboard.registerHandler([this] (auto const ... args)
    {
        onKeyboardEvent(std::forward<decltype(args)>(args)...);
    });
}

// virtual (from InputHandler)
auto SampleInputHandler::processInput(double const lastFrameDuration)
    -> void
{
    processTerminationRequest();
    
    processMouseCapture();

    processCameraManipulation(lastFrameDuration);

    processShapeModification(lastFrameDuration);

    processLightRevolution(lastFrameDuration);
}

auto SampleInputHandler::processTerminationRequest() const
    -> void
{
    if (window->isKeyPressed(Key::keyEscape))
    {
        window->requestClosure();
    }
}

auto SampleInputHandler::processMouseCapture() const
    -> void
{
    if (window->isKeyPressed(Key::keyM))
    {
        window->releaseMouse();
    }
    else if (window->isKeyPressed(Key::keyN))
    {
        window->captureMouse();
    }
}

auto SampleInputHandler::processCameraManipulation(double const lastFrameDuration)
    -> void
{
    cameraManipulator.update(lastFrameDuration);
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
    auto const rotationDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    if (window->isKeyPressed(Key::keyA))
    {
        rotateBodyAroundOwnX(*scene->rotatingContainer, +rotationDelta);
    }
    else if (window->isKeyPressed(Key::keyD))
    {
        rotateBodyAroundOwnX(*scene->rotatingContainer, -rotationDelta);
    }
}

auto SampleInputHandler::processShapeScaling(double const lastFrameDuration) const
    -> void
{
    auto const scalingDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    if (window->isKeyPressed(Key::keyS))
    {
        scaleUniformly(*scene->scalingContainer, 1 + scalingDelta);
    }
    else if (window->isKeyPressed(Key::keyW))
    {
        scaleUniformly(*scene->scalingContainer, 1 - scalingDelta);
    }
}

auto SampleInputHandler::processLightRevolution(double const lastFrameDuration) const
    -> void
{
    auto const rotationDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    if (window->isKeyPressed(Key::keyO))
    {
        rotateLightAroundWorldY(*scene, +rotationDelta);
    }
    else if (window->isKeyPressed(Key::keyP))
    {
        rotateLightAroundWorldY(*scene, -rotationDelta);
    }
}

auto SampleInputHandler::onKeyboardEvent(
    Key const key,
    KeyAction const action,
    KeyModifier const modifier) const
    -> void
{
    if (action != KeyAction::press)
    {
        return;
    }

    processLightToggling(key, modifier);

    processCameraSwitching(key, modifier);

    processFullScreenToggling(key);
}

auto SampleInputHandler::processLightToggling(Key const key, KeyModifier const modifier) const
    -> void
{
    if ((key < Key::key1) || (key > Key::key9))
    {
        return;
    }

    auto const index = static_cast<int>(key) - static_cast<int>(Key::key1);

    if (modifier == KeyModifier::shift)
    {
        togglePointLight(index);
    }
    else if (modifier == KeyModifier::none)
    {
        toggleSpotLight(index);
    }
}

auto SampleInputHandler::processCameraSwitching(Key const key, KeyModifier const modifier) const
    -> void
{
    if ((key < Key::key1) || (key > Key::key9))
    {
        return;
    }

    auto const index = static_cast<int>(key) - static_cast<int>(Key::key1);

    if (modifier == KeyModifier::control)
    {
        switchToCamera(index);
    }
}

auto SampleInputHandler::processFullScreenToggling(Key const key) const
    -> void
{
    if (key != Key::keyF11)
    {
        return;
    }

    if (!window->isFullScreen())
    {
        window->setFullScreen();
    }
    else
    {
        window->exitFullScreen();
    }
}

auto SampleInputHandler::togglePointLight(int const index) const
    -> void
{
    if (index >= static_cast<int>(scene->lighting.point.size()))
    {
        return;
    }

    toggle(scene->lighting.point[index].isTurnedOn);
}

auto SampleInputHandler::toggleSpotLight(int const index) const
    -> void
{
    if (index >= static_cast<int>(scene->lighting.spot.size()))
    {
        return;
    }

    toggle(scene->lighting.spot[index].isTurnedOn);
}

auto SampleInputHandler::switchToCamera(int const index) const
    -> void
{
    auto & cameras = scene->cameraSystem.cameras;

    if (index >= static_cast<int>(cameras.size()))
    {
        return;
    }

    scene->cameraSystem.activeCamera = &cameras[index];

    scene->cameraSystem.activeCamera->setAspectRatio(window->getAspectRatio());
}
