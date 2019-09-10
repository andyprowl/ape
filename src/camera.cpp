#include "camera.hpp"

#include "shader.hpp"
#include "widget.hpp"
#include "window.hpp"
#include "world.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 const & position, glm::vec3 const & lookAt, glm::vec3 const & up)
    : position{position}
    , lookAt{lookAt}
    , up{glm::normalize(up)}
    , view{makeView()}
{
    assert(lookAt != position);
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
    auto const offset = (newPosition - position);

    position = newPosition;

    lookAt += offset;

    view = makeView();
}

auto Camera::getLookAt() const
    -> glm::vec3
{
    return lookAt;
}

auto Camera::setLookAt(glm::vec3 const & newLookAt)
    -> void
{
    lookAt = newLookAt;

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

auto Camera::getDirection() const
    -> glm::vec3
{
    return glm::normalize(lookAt - position);
}

auto Camera::makeView() const
    -> glm::mat4
{
    return glm::lookAt(position, lookAt, up);
}
