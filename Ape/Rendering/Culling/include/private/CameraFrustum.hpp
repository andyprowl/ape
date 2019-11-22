#pragma once

#include <Ape/World/Scene/PerspectiveProjection.hpp>

namespace ape
{

class Camera;

auto getPerspectiveFrustum(Camera const & camera)
    -> PerspectiveProjection::Frustum const &;

} // namespace ape
