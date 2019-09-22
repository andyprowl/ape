#include <Ape/Camera.hpp>

#include <Ape/CompilerWarnings.hpp>
#include <Ape/ShaderProgram.hpp>
#include <Ape/Window.hpp>

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
    , transformation{projection * view}
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

auto Camera::getTransformation() const
    -> glm::mat4
{
    return transformation;
}

auto Camera::getPosition() const
    -> glm::vec3
{
    return position;
}

auto Camera::setPosition(glm::vec3 const & newPosition)
    -> void
{
    if (position == newPosition)
    {
        return;
    }

    auto const offset = newPosition - position;

    position = newPosition;

    setViewAndFireEvent(glm::translate(view, -offset));
}

auto Camera::getDirection() const
    -> glm::vec3
{
    return direction;
}

auto Camera::setDirection(glm::vec3 const & newDirection)
    -> void
{
    auto const normalizedDirection = glm::normalize(newDirection);

    if (direction == normalizedDirection)
    {
        return;
    }

    direction = normalizedDirection;

    auto const right = glm::normalize(glm::cross(direction, glm::vec3{0.0, 1.0, 0.0}));
    
    up = glm::cross(right, direction);
    
    setViewAndFireEvent(makeView());
}

auto Camera::getUp() const
    -> glm::vec3
{
    return up;
}

auto Camera::getFieldOfView() const
    -> float
{
    return fieldOfView;
}

auto Camera::setFieldOfView(float const newFieldOfView)
    -> void
{
    if (fieldOfView == newFieldOfView)
    {
        return;
    }

    fieldOfView = newFieldOfView;

    setProjectionAndFireEvent(makeProjection());
}

auto Camera::getAspectRatio() const
    -> float
{
    return aspectRatio;
}

auto Camera::setAspectRatio(float const newAspectRatio)
    -> void
{
    if (aspectRatio == newAspectRatio)
    {
        return;
    }

    aspectRatio = newAspectRatio;

    setProjectionAndFireEvent(makeProjection());
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

auto Camera::setViewAndFireEvent(glm::mat4 const & newView)
    -> void
{
    view = newView;

    transformation = projection * view;

    onViewChanged.fire(view);

    onTransformationChanged.fire(view);
}

auto Camera::setProjectionAndFireEvent(glm::mat4 const & newProjection)
    -> void
{
    projection = newProjection;

    transformation = projection * view;

    onProjectionChanged.fire(view);

    onTransformationChanged.fire(view);
}
