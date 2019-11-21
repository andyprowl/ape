#include "CameraFrustum.hpp"

#include <Ape/World/Scene/Camera.hpp>

namespace ape
{

namespace
{

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

} // namespace ape
