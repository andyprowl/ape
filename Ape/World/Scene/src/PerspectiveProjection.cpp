#include <Ape/World/Scene/PerspectiveProjection.hpp>

#include <Ape/World/Scene/Camera.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace ape
{

PerspectiveProjection::PerspectiveProjection(Frustum const & frustum, Camera & parent)
    : frustum{frustum}
    , transformation{makeProjection()}
    , parent{&parent}
{
}

auto PerspectiveProjection::getTransformation() const
    -> glm::mat4 const &
{
    return transformation;
}

auto PerspectiveProjection::getFrustum() const
    -> Frustum const &
{
    return frustum;
}

auto PerspectiveProjection::setFrustum(Frustum const & newFrustum)
    -> void
{
    frustum = newFrustum;

    recalculateProjectionAndNotifyParent();
}

auto PerspectiveProjection::getFieldOfView() const
    -> float
{
    return frustum.fieldOfView;
}

auto PerspectiveProjection::setFieldOfView(float const newFieldOfView)
    -> void
{
    if (frustum.fieldOfView == newFieldOfView)
    {
        return;
    }

    frustum.fieldOfView = newFieldOfView;

    recalculateProjectionAndNotifyParent();
}

auto PerspectiveProjection::getAspectRatio() const
    -> float
{
    return frustum.aspectRatio;
}

auto PerspectiveProjection::setAspectRatio(float const newAspectRatio)
    -> void
{
    if (frustum.aspectRatio == newAspectRatio)
    {
        return;
    }

    frustum.aspectRatio = newAspectRatio;

    recalculateProjectionAndNotifyParent();
}

auto PerspectiveProjection::getParent() const
    -> Camera &
{
    return *parent;
}

auto PerspectiveProjection::makeProjection() const
    -> glm::mat4
{
    return glm::perspective(frustum.fieldOfView, frustum.aspectRatio, frustum.near, frustum.far);
}

auto PerspectiveProjection::recalculateProjectionAndNotifyParent()
    -> void
{
    transformation = makeProjection();

    parent->updateTransformationAfterProjectionChanged(transformation);
}

auto PerspectiveProjection::setParent(Camera & camera)
    -> void
{
    parent = &camera;
}

} // namespace ape
