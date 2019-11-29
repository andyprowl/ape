#include "CameraFrustum.hpp"

#include <Ape/World/Scene/Camera.hpp>

#include <glm/gtc/matrix_access.hpp>

namespace ape
{

namespace
{

auto makePlane(glm::vec4 const & v)
    -> Plane
{
    return Plane{glm::vec3{v}, v.w};
}

auto getCameraProjection(Camera const & camera)
    -> PerspectiveProjection const &
{
    return camera.getProjection().as<PerspectiveProjection>();
}

} // unnamed namespace

auto getPerspectiveFrustum(Camera const & camera)
    -> PerspectiveProjection::Frustum const &
{
    auto const & projection = getCameraProjection(camera);

    return projection.getFrustum();
}

auto extractCameraFrustum(Camera const & camera)
    -> Frustum
{
    auto const transformation = camera.getTransformation();

    auto const row0 = glm::row(transformation, 0);

    auto const row1 = glm::row(transformation, 1);

    auto const row2 = glm::row(transformation, 2);

    auto const row3 = glm::row(transformation, 3);

    return {
        makePlane(-row0 + row3),
        makePlane(+row0 + row3),
        makePlane(-row1 + row3),
        makePlane(+row1 + row3),
        makePlane(+row2 + row3),
        makePlane(-row2 + row3)};
}

} // namespace ape
