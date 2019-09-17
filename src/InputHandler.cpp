#include "InputHandler.hpp"

#include "Camera.hpp"
#include "KeyboardPublisher.hpp"
#include "Scene.hpp"
#include "ShaderProgram.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

namespace
{

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

    auto & mesh = *(scene.bodies.end() - 2);

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
    GLFWwindow & window,
    MouseWheelPublisher & wheelPublisher,
    KeyboardPublisher & keyboardPublisher,
    ShaderProgram const & program)
    : scene{&scene}
    , window{&window}
    , program{&program}
    , cameraManipulator{scene, window, wheelPublisher, 0.1f}
{
    registerKeyboardEventHandler(keyboardPublisher);
}

auto InputHandler::registerKeyboardEventHandler(KeyboardPublisher & keyboardPublisher) const
    -> void
{
    keyboardPublisher.onKeyboardEvent.registerHandler([this] (auto const ... args)
    {
        onKeyboardEvent(std::forward<decltype(args)>(args)...);
    });
}

auto InputHandler::processInput(double const lastFrameDuration)
    -> void
{
    processTerminationRequest();
    
    processMouseCapture();

    processFullScreenToggle();

    processCameraManipulation(lastFrameDuration);

    processShapeModification(lastFrameDuration);

    processLightRevolution(lastFrameDuration);
}

auto InputHandler::processTerminationRequest() const
    -> void
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

auto InputHandler::processMouseCapture() const
    -> void
{
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

auto InputHandler::processFullScreenToggle() const
    -> void
{
    if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS)
    {
        auto const monitor = glfwGetPrimaryMonitor();

        // get reolution of monitor
        const auto mode = glfwGetVideoMode(monitor);

        // switch to full screen
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    else if (glfwGetKey(window, GLFW_KEY_F12) == GLFW_PRESS)
    {
        glfwSetWindowMonitor(window, nullptr, 100, 100, 1024, 768, GLFW_DONT_CARE);
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

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        rotateMeshAroundOwnX(scene->bodies.front(), +rotationDelta);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        rotateMeshAroundOwnX(scene->bodies.front(), -rotationDelta);
    }
}

auto InputHandler::processShapeScaling(double const lastFrameDuration) const
    -> void
{
    auto const scalingDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        scene->bodies[1].scaleUniformly(1 + scalingDelta);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        scene->bodies[1].scaleUniformly(1 - scalingDelta);
    }
}

auto InputHandler::processLightRevolution(double const lastFrameDuration) const
    -> void
{
    auto const rotationDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        rotateLightAroundSceneY(*scene, +rotationDelta);
    }
    else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        rotateLightAroundSceneY(*scene, -rotationDelta);
    }
}

auto InputHandler::onKeyboardEvent(
    int const key,
    int const /*scancode*/,
    int const action,
    int const /*mods*/) const
    -> void
{
    if ((key == GLFW_KEY_1) && (action == GLFW_PRESS))
    {
        scene->lighting.spot[0].isTurnedOn = !(scene->lighting.spot[0].isTurnedOn);
    }
    else if ((key == GLFW_KEY_2) && (action == GLFW_PRESS))
    {
        scene->lighting.spot[1].isTurnedOn = !(scene->lighting.spot[1].isTurnedOn);
    }
}
