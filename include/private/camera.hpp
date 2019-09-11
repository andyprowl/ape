#pragma once

#include "glfw.hpp"

#include <glm/mat4x4.hpp>

#include <vector>

class ShaderProgram;
class Widget;
class World;

class Camera
{

public:

    Camera(glm::vec3 const & position, glm::vec3 const & direction, glm::vec3 const & up);

    auto getView() const
        -> glm::mat4;

    auto getPosition() const
        -> glm::vec3;

    auto setPosition(glm::vec3 const & newPosition)
        -> void;

    auto getDirection() const
        -> glm::vec3;

    auto setDirection(glm::vec3 const & newDirection)
        -> void;

    auto getUp() const
        -> glm::vec3;

    auto setUp(glm::vec3 const & newUp)
        -> void;

private:

    auto makeView() const
        -> glm::mat4;

private:

    glm::vec3 position;

    glm::vec3 direction;

    glm::vec3 up;

    glm::mat4 view;

};
