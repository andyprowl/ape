#include "InputHandler.hpp"

#include "Camera.hpp"
#include "Scene.hpp"
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

auto rotateMeshAroundOwnX(Mesh & mesh, float const radians)
    -> void
{
    auto const rotation = glm::rotate(mesh.getModelTransformation(), radians, {1.0f, 0.0f, 0.0f});
    
    mesh.setModelTransformation(rotation);
}

auto rotateLightAroundSceneY(Scene & scene, float const radians)
    -> void
{
    auto & light = scene.lighting.point[0];

    auto const position = light.position;
    
    auto const revolution = glm::rotate(glm::mat4{1.0f}, radians, {0.0f, 1.0f, 0.0f});

    auto const newPosition = glm::vec3{revolution * glm::vec4{position, 1.0f}};

    light.position = newPosition;

    auto & mesh = *(scene.meshes.end() - 2);

    auto const transformation  = 
        glm::translate(glm::mat4{1.0f}, newPosition) *
        glm::rotate(glm::mat4{1.0f}, radians, {0.0f, 1.0f, 0.0f}) *
        glm::translate(glm::mat4{1.0f}, -position) *
        mesh.getModelTransformation();

    mesh.setModelTransformation(transformation);
}

} // unnamed namespace

InputHandler::InputHandler(
    Scene & scene,
    Window & window,
    ShaderProgram const & program)
    : scene{&scene}
    , window{&window}
    , program{&program}
    , cameraManipulator{scene, window, 0.1f}
    , keyboardHandlerConnection{registerKeyboardEventHandler()}
{
}

auto InputHandler::registerKeyboardEventHandler() const
    -> ScopedSignalConnection
{
    return window->onKeyboard.registerHandler([this] (auto const ... args)
    {
        onKeyboardEvent(std::forward<decltype(args)>(args)...);
    });
}

auto InputHandler::processInput(double const lastFrameDuration)
    -> void
{
    processTerminationRequest();
    
    processMouseCapture();

    processCameraManipulation(lastFrameDuration);

    processShapeModification(lastFrameDuration);

    processLightRevolution(lastFrameDuration);
}

auto InputHandler::processTerminationRequest() const
    -> void
{
    if (window->getKeyStatus(GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        window->requestClosure();
    }
}

auto InputHandler::processMouseCapture() const
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

auto InputHandler::processCameraManipulation(double const lastFrameDuration)
    -> void
{
    cameraManipulator.update(lastFrameDuration);
}

auto InputHandler::processShapeModification(double const lastFrameDuration) const
    -> void
{
    processShapeRotation(lastFrameDuration);

    processShapeScaling(lastFrameDuration);
}

auto InputHandler::processShapeRotation(double const lastFrameDuration) const
    -> void
{
    auto const rotationDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    if (window->getKeyStatus(GLFW_KEY_A) == GLFW_PRESS)
    {
        rotateMeshAroundOwnX(scene->meshes.front(), +rotationDelta);
    }
    else if (window->getKeyStatus(GLFW_KEY_D) == GLFW_PRESS)
    {
        rotateMeshAroundOwnX(scene->meshes.front(), -rotationDelta);
    }
}

auto InputHandler::processShapeScaling(double const lastFrameDuration) const
    -> void
{
    auto const scalingDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    if (window->getKeyStatus(GLFW_KEY_S) == GLFW_PRESS)
    {
        scene->meshes[1].scaleUniformly(1 + scalingDelta);
    }
    else if (window->getKeyStatus(GLFW_KEY_W) == GLFW_PRESS)
    {
        scene->meshes[1].scaleUniformly(1 - scalingDelta);
    }
}

auto InputHandler::processLightRevolution(double const lastFrameDuration) const
    -> void
{
    auto const rotationDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    if (window->getKeyStatus(GLFW_KEY_O) == GLFW_PRESS)
    {
        rotateLightAroundSceneY(*scene, +rotationDelta);
    }
    else if (window->getKeyStatus(GLFW_KEY_P) == GLFW_PRESS)
    {
        rotateLightAroundSceneY(*scene, -rotationDelta);
    }
}

auto InputHandler::onKeyboardEvent(
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

    processFullScreenToggling(key);
}

auto InputHandler::processLightToggling(int const key, int const mods) const
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
    else
    {
        toggleSpotLight(index);
    }
}

auto InputHandler::processFullScreenToggling(int const key) const
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

auto InputHandler::togglePointLight(int const index) const
    -> void
{
    if (index >= static_cast<int>(scene->lighting.point.size()))
    {
        return;
    }

    toggle(scene->lighting.point[index].isTurnedOn);
}

auto InputHandler::toggleSpotLight(int const index) const
    -> void
{
    if (index >= static_cast<int>(scene->lighting.spot.size()))
    {
        return;
    }

    toggle(scene->lighting.spot[index].isTurnedOn);
}
