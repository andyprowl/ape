#include <Ape/Scene/OrthographicProjection.hpp>

#include <Ape/Scene/Camera.hpp>

#include <Foundational/CompilerSupport/CompilerWarnings.hpp>

disableCompilerWarnings()
#include <glm/gtc/matrix_transform.hpp>
restoreCompilerWarnings()

namespace ape
{ 

OrthographicProjection::OrthographicProjection(Frustum const & frustum, Camera & parent)
    : frustum{frustum}
    , transformation{makeProjection()}
    , parent{&parent}
{
}

auto OrthographicProjection::getTransformation() const
    -> glm::mat4 const &
{
    return transformation;
}

auto OrthographicProjection::getFrustum() const
    -> Frustum const &
{
    return frustum;
}

auto OrthographicProjection::setFrustum(Frustum const & newFrustum)
    -> void
{
    frustum = newFrustum;

    recalculateProjectionAndNotifyParent();
}

auto OrthographicProjection::getParent() const
    -> Camera &
{
    return *parent;
}

auto OrthographicProjection::makeProjection() const
    -> glm::mat4
{
    return glm::ortho(
        frustum.left,
        frustum.right,
        frustum.bottom,
        frustum.top,
        frustum.near,
        frustum.far);
}

auto OrthographicProjection::recalculateProjectionAndNotifyParent()
    -> void
{
    transformation = makeProjection();

    parent->updateTransformationAfterProjectionChanged(transformation);
}

auto OrthographicProjection::setParent(Camera & camera)
    -> void
{
    parent = &camera;
}

} // namespace ape
