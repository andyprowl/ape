#include <Ape/Rendering/Culling/RadarFrustumCuller.hpp>

#include "CameraFrustum.hpp"
#include "RadarFrustumPointCuller.hpp"
#include "RadarFrustumSphereCuller.hpp"

#include <glm/trigonometric.hpp>

namespace ape
{

RadarFrustumCuller::RadarFrustumCuller(Camera const & camera)
    : camera{&camera}
    , precalculatedValues{precalculateValues()}
{
}

auto RadarFrustumCuller::isSphereContained(Sphere const & sphere) const
    -> ContainmentRelation
{
    return SphereCuller{*camera, sphere, precalculatedValues}.computeRelation();
}

auto RadarFrustumCuller::isPointContained(glm::vec3 const & point) const
    -> bool
{
    return PointCuller{*camera, point, precalculatedValues}.computeContainment();
}

auto RadarFrustumCuller::precalculateValues() const
    -> PrecalculatedValueSet
{
    auto const & frustum = ape::getPerspectiveFrustum(*camera);

    auto const halfFieldOfViewY = frustum.fieldOfView / 2.0f;

    auto const factorY = 1.0f / glm::cos(halfFieldOfViewY);

    auto const tangent = glm::tan(halfFieldOfViewY);

    auto const halfFieldOfViewX = glm::atan(tangent * frustum.aspectRatio);

    auto const factorX = 1.0f / glm::cos(halfFieldOfViewX);

    return {tangent, factorX, factorY};
}

} // namespace ape
