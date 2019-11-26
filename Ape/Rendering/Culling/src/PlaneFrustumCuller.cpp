#include <Ape/Rendering/Culling/PlaneFrustumCuller.hpp>

#include "CameraFrustum.hpp"

#include <Ape/World/Scene/Camera.hpp>
#include <Ape/World/Shape/Sphere.hpp>

#include <Basix/Range/Search.hpp>

namespace ape
{

PlaneFrustumCuller::PlaneFrustumCuller(Camera const & camera)
    : camera{&camera}
    , frustum{extractCameraFrustum(camera)}
{
}

auto PlaneFrustumCuller::isSphereContained(Sphere const & sphere) const
    -> ContainmentRelation
{
    auto relation = ContainmentRelation::fullyInside;

    auto const center = sphere.getCenter();

    auto const radius = sphere.getRadius();

    for (auto const & plane : frustum.getPlanes())
    {
        auto const distance = computeSignedDistance(center, plane);

        if (distance < -radius)
        {
            return ContainmentRelation::fullyOutside;
        }
        else if (distance < radius)
        {
            return ContainmentRelation::partlyInside;
        }
    }

    return relation;
}

auto PlaneFrustumCuller::isPointContained(glm::vec3 const & point) const
    -> bool
{
    auto const & planes = frustum.getPlanes();

    auto const isOnPositiveSide = [&point] (Plane const & plane)
    {
        auto const signedDistance = computeSignedDistance(point, plane);

        return (signedDistance >= 0.0f);
    };

    return basix::containsIf(planes, isOnPositiveSide);
}

} // namespace ape
