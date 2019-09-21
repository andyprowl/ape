#include "SampleInputHandler.hpp"

#include "Camera.hpp"
#include "SampleScene.hpp"
#include "ShaderProgram.hpp"
#include "Window.hpp"

#include "GLFW.hpp"

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
    SampleScene & scene,
    Window & window,
    ShaderProgram const & program)
    : scene{&scene}
    , window{&window}
    , program{&program}
    , cameraManipulator{scene, window, 0.1f}
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
    if (window->getKeyStatus(GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        window->requestClosure();
    }
}

auto SampleInputHandler::processMouseCapture() const
    -> void
{
    if (window->getKeyStatus(GLFW_KEY_M) == GLFW_PRESS)
    {
        window->releaseMouse();
    }
    else if (window->getKeyStatus(GLFW_KEY_N) == GLFW_PRESS)
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

    if (window->getKeyStatus(GLFW_KEY_A) == GLFW_PRESS)
    {
        rotateBodyAroundOwnX(*scene->rotatingContainer, +rotationDelta);
    }
    else if (window->getKeyStatus(GLFW_KEY_D) == GLFW_PRESS)
    {
        rotateBodyAroundOwnX(*scene->rotatingContainer, -rotationDelta);
    }
}

auto SampleInputHandler::processShapeScaling(double const lastFrameDuration) const
    -> void
{
    auto const scalingDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    if (window->getKeyStatus(GLFW_KEY_S) == GLFW_PRESS)
    {
        scaleUniformly(*scene->scalingContainer, 1 + scalingDelta);
    }
    else if (window->getKeyStatus(GLFW_KEY_W) == GLFW_PRESS)
    {
        scaleUniformly(*scene->scalingContainer, 1 - scalingDelta);
    }
}

auto SampleInputHandler::processLightRevolution(double const lastFrameDuration) const
    -> void
{
    auto const rotationDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    if (window->getKeyStatus(GLFW_KEY_O) == GLFW_PRESS)
    {
        rotateLightAroundWorldY(*scene, +rotationDelta);
    }
    else if (window->getKeyStatus(GLFW_KEY_P) == GLFW_PRESS)
    {
        rotateLightAroundWorldY(*scene, -rotationDelta);
    }
}

auto SampleInputHandler::onKeyboardEvent(
    int const key,
    int const /*scancode*/,
    int const action,
    int const mods) const
    -> void
{
    if (action != GLFW_PRESS)
    {
        return;
    }

    processLightToggling(key, mods);

    processCameraSwitching(key, mods);

    processFullScreenToggling(key);
}

auto SampleInputHandler::processLightToggling(int const key, int const mods) const
    -> void
{
    if ((key < GLFW_KEY_1) || (key > GLFW_KEY_9))
    {
        return;
    }

    auto const index = key - GLFW_KEY_1;

    if (mods & GLFW_MOD_SHIFT)
    {
        togglePointLight(index);
    }
    else if (mods == 0)
    {
        toggleSpotLight(index);
    }
}

auto SampleInputHandler::processCameraSwitching(int const key, int const mods) const
    -> void
{
    if ((key < GLFW_KEY_1) || (key > GLFW_KEY_9))
    {
        return;
    }

    auto const index = key - GLFW_KEY_1;

    if (mods & GLFW_MOD_CONTROL)
    {
        switchToCamera(index);
    }
}

auto SampleInputHandler::processFullScreenToggling(int const key) const
    -> void
{
    if (key != GLFW_KEY_F11)
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
