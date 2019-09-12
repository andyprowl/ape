#include "input.hpp"

#include "camera.hpp"
#include "shader.hpp"
#include "world.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

namespace
{

auto rotateCameraHorizontally(Camera & camera, float const radians)
    -> void
{
    auto const rotation = glm::rotate(glm::mat4{1.0f}, radians, camera.getUp());

    auto const position = camera.getPosition();

    auto const direction = camera.getDirection();

    auto const newDirection = rotation * glm::vec4{direction, 1.0f};

    camera.setDirection(newDirection);
}

auto moveCameraAlongDirection(Camera & camera, float const magnitude)
    -> void
{
    auto const position = camera.getPosition();

    auto const direction = camera.getDirection();

    auto const newPosition = position + (direction * magnitude);

    camera.setPosition(newPosition);
}

auto moveCameraSideways(Camera & camera, float const magnitude)
    -> void
{
    auto const position = camera.getPosition();

    auto const direction = camera.getDirection();

    auto const up = camera.getUp();

    auto const movementDirection = glm::cross(direction, up);

    auto const newPosition = position + (movementDirection * magnitude);

    camera.setPosition(newPosition);
}

auto rotateWidgetAroundX(Widget & widget, float const radians)
    -> void
{
    auto const rot = glm::rotate(widget.getModelTransformation(), radians, {1.0f, 0.0f, 0.0f});
    
    widget.setModelTransformation(rot);
}

} // unnamed namespace

InputHandler::InputHandler(
    World & world,
    Camera & camera,
    GLFWwindow & window,
    ShaderProgram const & program)
    : world{&world}
    , camera{&camera}
    , window{&window}
    , program{&program}
    , cameraManipulator{window, camera, 0.1f}
{
}

auto InputHandler::processInput(double const lastFrameDuration)
    -> void
{
    processTerminationRequest();
    
    processMouseCapture();

    processMouseMovement(lastFrameDuration);

    processRotationalMovement(lastFrameDuration);

    processLateralMovement(lastFrameDuration);

    processShapeModification(lastFrameDuration);

    processStyleModification(lastFrameDuration);
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

auto InputHandler::processMouseMovement(double const lastFrameDuration)
    -> void
{
    cameraManipulator.update(lastFrameDuration);
}

auto InputHandler::processRotationalMovement(double const lastFrameDuration) const
    -> void
{
    auto const rotationDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        rotateCameraHorizontally(*camera, +rotationDelta);
    }
    else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        rotateCameraHorizontally(*camera, -rotationDelta);
    }
}

auto InputHandler::processLateralMovement(double const lastFrameDuration) const
    -> void
{
    auto const translationDelta = static_cast<float>(lastFrameDuration * 5.0f);

    if ((glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS))
    {
        moveCameraAlongDirection(*camera, +translationDelta);
    }
    else if ((glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) ||
             (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS))
    {
        moveCameraAlongDirection(*camera, -translationDelta);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        moveCameraSideways(*camera, -translationDelta);
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        moveCameraSideways(*camera, +translationDelta);
    }
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
        rotateWidgetAroundX(world->widgets[0], +rotationDelta);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        rotateWidgetAroundX(world->widgets[0], -rotationDelta);
    }
}

auto InputHandler::processShapeScaling(double const lastFrameDuration) const
    -> void
{
    auto const scalingDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        world->widgets[1].scaleUniformly(1 + scalingDelta);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        world->widgets[1].scaleUniformly(1 - scalingDelta);
    }
}

auto InputHandler::processStyleModification(double const lastFrameDuration) const
    -> void
{
    program->use();

    auto const transitionDelta = glm::radians(static_cast<float>(lastFrameDuration * 50.0f));

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        const auto weight = program->getFloat("textureWeight");

        program->set("textureWeight", std::min(1.0f, weight + transitionDelta));
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        const auto weight = program->getFloat("textureWeight");

        program->set("textureWeight", std::max(0.0f, weight - transitionDelta));
    }
    
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        const auto weight = program->getFloat("colorWeight");

        program->set("colorWeight", std::min(1.0f, weight + transitionDelta));
    }
    else if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
        const auto weight = program->getFloat("colorWeight");

        program->set("colorWeight", std::max(0.0f, weight - transitionDelta));
    }
}
