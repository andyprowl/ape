#include <Scene/CameraProjection.hpp>

namespace ape
{

CameraProjection::CameraProjection(
    OrthographicProjection::Frustum const & orthographicFrustum,
    Camera & parent)
    : projection{OrthographicProjection{orthographicFrustum, parent}}
{
}

CameraProjection::CameraProjection(
    PerspectiveProjection::Frustum const & perspectiveFrustum,
    Camera & parent)
    : projection{PerspectiveProjection{perspectiveFrustum, parent}}
{
}

auto CameraProjection::operator = (OrthographicProjection::Frustum const & orthographicFrustum)
    -> CameraProjection &
{
    projection = OrthographicProjection{orthographicFrustum, getParent()};

    return *this;
}

auto CameraProjection::operator = (PerspectiveProjection::Frustum const & orthographicFrustum)
    -> CameraProjection &
{
    projection = PerspectiveProjection{orthographicFrustum, getParent()};

    return *this;
}

auto CameraProjection::getTransformation() const
    -> glm::mat4 const &
{
    auto getter = [] (auto const & projection)
        -> glm::mat4 const &
    {
        return projection.getTransformation();
    };

    return std::visit(getter, projection);
}

auto CameraProjection::getParent() const
    -> Camera &
{
    auto getter = [] (auto const & projection)
        -> Camera &
    {
        return projection.getParent();
    };

    return std::visit(getter, projection);
}

auto CameraProjection::setParent(Camera & camera)
    -> void
{
    auto getter = [&camera] (auto & projection)
    {
        projection.setParent(camera);
    };

    return std::visit(getter, projection);
}

} // namespace ape
