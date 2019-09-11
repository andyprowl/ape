#include "camera.hpp"

#include "shader.hpp"
#include "widget.hpp"
#include "window.hpp"
#include "world.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 const & position, glm::vec3 const & direction, glm::vec3 const & up)
    : position{position}
    , direction{normalize(direction)}
    , up{glm::normalize(up)}
    , view{makeView()}
{
}

auto Camera::getView() const
    -> glm::mat4
{
    return view;
}

auto Camera::getPosition() const
    -> glm::vec3
{
    return position;
}

auto Camera::setPosition(glm::vec3 const & newPosition)
    -> void
{
    position = newPosition;

    view = makeView();
}

auto Camera::getDirection() const
    -> glm::vec3
{
    return direction;
}

auto Camera::setDirection(glm::vec3 const & newDirection)
    -> void
{
    direction = glm::normalize(newDirection);

    view = makeView();
}

auto Camera::getUp() const
    -> glm::vec3
{
    return up;
}

auto Camera::setUp(glm::vec3 const & newUp)
    -> void
{
    up = newUp;

    view = makeView();
}

auto Camera::makeView() const
    -> glm::mat4
{
    return glm::lookAt(position, position + direction, up);
}
