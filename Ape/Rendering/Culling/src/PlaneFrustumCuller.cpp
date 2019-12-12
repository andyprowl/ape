#include <Ape/Rendering/Culling/PlaneFrustumCuller.hpp>

#include "CameraFrustum.hpp"

#include <Ape/World/Scene/Camera.hpp>
#include <Ape/World/Shape/Sphere.hpp>

#include <Basix/Range/Search.hpp>

namespace ape
{

namespace
{

auto isFrustumBehindPlane(Frustum const & f, Plane const & p)
    -> bool
{
    auto const isVertexInFrontOfPlane = [&p] (glm::vec3 const & v)
    {
        return (computeSignedDistance(v, p) >= 0.0f);
    };

    return basix::noneOf(f.getVertices(), isVertexInFrontOfPlane);
}

template<typename PlaneContainer>
auto isFrustumBeyondAtLeastOnePlane(Frustum const & subject, PlaneContainer const & planes)
    -> bool
{
    auto const isSeparatingPlane = [&subject] (Plane const & plane)
    {
        return isFrustumBehindPlane(subject, plane);
    };

    return basix::containsIf(planes, isSeparatingPlane);
}

} // unnamed namespace

PlaneFrustumCuller::PlaneFrustumCuller(Frustum const & frustum)
    : frustum{frustum}
{
}

PlaneFrustumCuller::PlaneFrustumCuller(Camera const & camera)
    : frustum{extractCameraFrustum(camera)}
{
}

auto PlaneFrustumCuller::isFrustumFullyOutside(Frustum const & f) const
    -> bool
{
    return isFrustumBeyondAtLeastOnePlane(f, frustum.getPlanes())
        || isFrustumBeyondAtLeastOnePlane(frustum, f.getPlanes());
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
            relation = ContainmentRelation::partlyInside;
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
