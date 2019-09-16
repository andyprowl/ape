#include "Camera.hpp"

#include "CompilerWarnings.hpp"
#include "Mesh.hpp"
#include "ShaderProgram.hpp"
#include "Window.hpp"

// Quaternion implementation uses some non-standard extensions.
disableCompilerWarnings()
#include <glm/gtc/quaternion.hpp>
disableCompilerWarnings()

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(
    glm::vec3 const & position,
    glm::vec3 const & direction,
    glm::vec3 const & up,
    float const fieldOfView,
    float const aspectRatio)
    : Transformable{}
    , position{position}
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
    return projection * view;
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

    view = glm::translate(view, -offset);
}

auto Camera::getDirection() const
    -> glm::vec3
{
    return direction;
}

auto rotationBetweenVectors(glm::vec3 const & start, glm::vec3 const & dest)
    -> glm::mat4
{
    auto const cosTheta = glm::dot(start, dest);

    auto const rotationAxis = glm::cross(start, dest);

    auto const s = glm::sqrt((1 + cosTheta) * 2);

    auto const invs = 1 / s;

    auto const quat = glm::quat{
        s * 0.5f, 
        rotationAxis.x * invs,
        rotationAxis.y * invs,
        rotationAxis.z * invs};

    return glm::mat4{quat};
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
    
    updateView();
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

    updateProjection();
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

    updateProjection();
}

// override (from Transformable)
auto Camera::onContextTransformationChanged(glm::mat4 const & contextTransformation)
    -> void
{
    transformation = projection * contextTransformation * view;
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

auto Camera::updateView()
    -> void
{
    view = makeView();

    updateTransformation();
}

auto Camera::updateProjection()
    -> void
{
    projection = makeProjection();

    updateTransformation();
}

auto Camera::updateTransformation()
    -> void
{
    transformation = projection * getContextTransformation() * view;
}
