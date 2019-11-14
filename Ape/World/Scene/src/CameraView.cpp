#include <Ape/World/Scene/CameraView.hpp>

#include <Ape/World/Scene/Camera.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace ape
{

CameraView::CameraView(Placement const & placement, Camera & parent)
    : placement{placement}
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
    -> glm::vec3
{
    return placement.position;
}

auto CameraView::setPosition(glm::vec3 const & newPosition)
    -> void
{
    if (placement.position == newPosition)
    {
        return;
    }

    auto const offset = newPosition - placement.position;

    placement.position = newPosition;

    transformation = glm::translate(transformation, -offset);

    parent->updateTransformationAfterViewChanged(transformation);
}

auto CameraView::getDirection() const
    -> glm::vec3
{
    return placement.direction;
}

auto CameraView::setDirection(glm::vec3 const & newDirection)
    -> void
{
    auto const normalizedDirection = glm::normalize(newDirection);

    if (placement.direction == normalizedDirection)
    {
        return;
    }

    placement.direction = normalizedDirection;

    auto const right = glm::normalize(glm::cross(placement.direction, glm::vec3{0.0, 1.0, 0.0}));
    
    placement.up = glm::cross(right, placement.direction);
    
    recalculateViewAndNotifyParent();
}

auto CameraView::getUp() const
    -> glm::vec3
{
    return placement.up;
}

auto CameraView::getParent() const
    -> Camera &
{
    return *parent;
}

auto CameraView::makeView() const
    -> glm::mat4
{
    return glm::lookAt(placement.position, placement.position + placement.direction, placement.up);
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
