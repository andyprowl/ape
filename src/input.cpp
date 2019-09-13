#include "Input.hpp"

#include "Camera.hpp"
#include "Shader.hpp"
#include "World.hpp"

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

auto rotateBodyAroundOwnX(Body & body, float const radians)
    -> void
{
    auto const rotation = glm::rotate(body.getModelTransformation(), radians, {1.0f, 0.0f, 0.0f});
    
    body.setModelTransformation(rotation);
}

auto rotateLightAroundWorldY(World & world, float const radians)
    -> void
{
    auto & light = world.lighting.point[0];

    auto const position = light.position;
    
    auto const revolution = glm::rotate(glm::mat4{1.0f}, radians, {0.0f, 1.0f, 0.0f});

    auto const newPosition = glm::vec3{revolution * glm::vec4{position, 1.0f}};

    light.position = newPosition;

    auto & body = *(world.bodies.end() - 2);

    auto const transformation  = 
        glm::translate(glm::mat4{1.0f}, newPosition) *
        glm::rotate(glm::mat4{1.0f}, radians, {0.0f, 1.0f, 0.0f}) *
        glm::translate(glm::mat4{1.0f}, -position) *
        body.getModelTransformation();

    body.setModelTransformation(transformation);
}

} // unnamed namespace

InputHandler::InputHandler(
    World & world,
    GLFWwindow & window,
    ShaderProgram const & program)
    : world{&world}
    , window{&window}
    , program{&program}
    , cameraManipulator{window, world.camera, 0.1f}
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
        rotateCameraHorizontally(world->camera, +rotationDelta);
    }
    else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        rotateCameraHorizontally(world->camera, -rotationDelta);
    }
}

auto InputHandler::processLateralMovement(double const lastFrameDuration) const
    -> void
{
    auto const translationDelta = static_cast<float>(lastFrameDuration * 5.0f);

    if ((glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) ||
        (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS))
    {
        moveCameraAlongDirection(world->camera, +translationDelta);
    }
    else if ((glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) ||
             (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS))
    {
        moveCameraAlongDirection(world->camera, -translationDelta);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        moveCameraSideways(world->camera, -translationDelta);
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        moveCameraSideways(world->camera, +translationDelta);
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
        rotateBodyAroundOwnX(*(world->bodies.end() - 3), +rotationDelta);
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        rotateBodyAroundOwnX(*(world->bodies.end() - 3), -rotationDelta);
    }
}

auto InputHandler::processShapeScaling(double const lastFrameDuration) const
    -> void
{
    auto const scalingDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        (world->bodies.end() - 3)->scaleUniformly(1 + scalingDelta);
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        (world->bodies.end() - 3)->scaleUniformly(1 - scalingDelta);
    }
}

auto InputHandler::processLightRevolution(double const lastFrameDuration) const
    -> void
{
    auto const rotationDelta = glm::radians(static_cast<float>(lastFrameDuration * 100.0f));

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        rotateLightAroundWorldY(*world, +rotationDelta);
    }
    else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        rotateLightAroundWorldY(*world, -rotationDelta);
    }
}
