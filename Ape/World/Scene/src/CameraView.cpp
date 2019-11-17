#include <Ape/World/Scene/CameraView.hpp>

#include <Ape/World/Scene/Camera.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace ape
{

CameraView::CameraView(System const & system, Camera & parent)
    : system{system}
    , transformation{makeView()}
    , parent{&parent}
{
}

auto CameraView::getTransformation() const
    -> glm::mat4 const &
{
    return transformation;
}

auto CameraView::getPosition() const
    -> glm::vec3 const &
{
    return system.position;
}

auto CameraView::setPosition(glm::vec3 const & newPosition)
    -> void
{
    if (system.position == newPosition)
    {
        return;
    }

    auto const offset = newPosition - system.position;

    system.position = newPosition;

    transformation = glm::translate(transformation, -offset);

    parent->updateTransformationAfterViewChanged(transformation);
}

auto CameraView::getDirection() const
    -> glm::vec3 const &
{
    return system.direction;
}

auto CameraView::setDirection(glm::vec3 const & newDirection)
    -> void
{
    auto const normalizedDirection = glm::normalize(newDirection);

    if (system.direction == normalizedDirection)
    {
        return;
    }

    system.direction = normalizedDirection;

    system.right = glm::normalize(glm::cross(system.direction, glm::vec3{0.0, 1.0, 0.0}));
    
    system.up = glm::cross(system.right, system.direction);
    
    recalculateViewAndNotifyParent();
}

auto CameraView::getUp() const
    -> glm::vec3 const &
{
    return system.up;
}

auto CameraView::getRight() const
    -> glm::vec3 const &
{
    return system.right;
}

auto CameraView::getParent() const
    -> Camera &
{
    return *parent;
}

auto CameraView::makeView() const
    -> glm::mat4
{
    return glm::lookAt(system.position, system.position + system.direction, system.up);
}

auto CameraView::recalculateViewAndNotifyParent()
    -> void
{
    transformation = makeView();

    parent->updateTransformationAfterViewChanged(transformation);
}

auto CameraView::setParent(Camera & camera)
    -> void
{
    parent = &camera;
}

} // namespace ape
