#include "Camera.hpp"

#include "Body.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "Window.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(
    glm::vec3 const & position,
    glm::vec3 const & direction,
    glm::vec3 const & up,
    float const fieldOfView,
    float const aspectRatio)
    : position{position}
    , direction{normalize(direction)}
    , up{glm::normalize(up)}
    , fieldOfView{fieldOfView}
    , aspectRatio{aspectRatio}
    , view{makeView()}
    , projection{makeProjection()}
{
}

auto Camera::getView() const
    -> glm::mat4
{
    return view;
}

auto Camera::getProjection() const
    -> glm::mat4
{
    return projection;
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

auto Camera::getFieldOfView() const
    -> float
{
    return fieldOfView;
}

auto Camera::setFieldOfView(float const newFieldOfView)
    -> void
{
    fieldOfView = newFieldOfView;

    projection = makeProjection();
}

auto Camera::getAspectRatio() const
    -> float
{
    return aspectRatio;
}

auto Camera::setAspectRatio(float const newAspectRatio)
    -> void
{
    aspectRatio = newAspectRatio;

    projection = makeProjection();
}

auto Camera::makeView() const
    -> glm::mat4
{
    return glm::lookAt(position, position + direction, up);
}

auto Camera::makeProjection() const
    -> glm::mat4
{
    return glm::perspective(fieldOfView, aspectRatio, 0.1f, 100.0f);
}
