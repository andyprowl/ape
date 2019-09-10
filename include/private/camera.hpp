#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>

#include <vector>

class ShaderProgram;
class Widget;
class World;

class Camera
{

public:

    Camera(glm::vec3 const & position, glm::vec3 const & lookAt, glm::vec3 const & up);

    auto getView() const
        -> glm::mat4;

    auto getPosition() const
        -> glm::vec3;

    auto setPosition(glm::vec3 const & newPosition)
        -> void;

    auto getLookAt() const
        -> glm::vec3;

    auto setLookAt(glm::vec3 const & newLookAt)
        -> void;

    auto getUp() const
        -> glm::vec3;

    auto setUp(glm::vec3 const & newUp)
        -> void;

    auto getDirection() const
        -> glm::vec3;

private:

    auto makeView() const
        -> glm::mat4;

private:

    glm::vec3 position;

    glm::vec3 lookAt;

    glm::vec3 up;

    glm::mat4 view;

};

auto render(
    GLFWwindow & window,
    World & world,
    Camera const & camera,
    ShaderProgram const & program)
    -> void;