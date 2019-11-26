#pragma once

#include <Ape/Rendering/Culling/Frustum.hpp>

#include <Ape/World/Scene/PerspectiveProjection.hpp>

namespace ape
{

class Camera;

auto getPerspectiveFrustum(Camera const & camera)
    -> PerspectiveProjection::Frustum const &;

auto extractCameraFrustum(Camera const & camera)
    -> Frustum;

} // namespace ape
