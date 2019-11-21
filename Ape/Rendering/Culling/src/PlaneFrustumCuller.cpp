#include <Ape/Rendering/Culling/PlaneFrustumCuller.hpp>

#include <Ape/World/Scene/Camera.hpp>
#include <Ape/World/Shape/Sphere.hpp>

#include <Basix/Range/Search.hpp>

namespace ape
{

namespace
{

auto makePlane(glm::vec4 const & v)
    -> Plane
{
    return Plane{glm::vec3{v}, v.w};
}

} // unnamed namespace

PlaneFrustumCuller::PlaneFrustumCuller(Camera const & camera)
    : camera{&camera}
    , frustum{extractCameraFrustum()}
{
}

auto PlaneFrustumCuller::computeFrustumRelation(Sphere const & sphere) const
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

auto PlaneFrustumCuller::computeFrustumRelation(Frustum const & f) const
    -> ContainmentRelation
{
    // Not yet implemented
    (void)f;
    throw std::exception{};
}

auto PlaneFrustumCuller::isPointInFrustum(glm::vec3 const & point) const
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

auto PlaneFrustumCuller::extractCameraFrustum() const
    -> Frustum
{
    auto const transformation = camera->getTransformation();

    return {
        makePlane(-transformation[0] + transformation[3]),
        makePlane(+transformation[0] + transformation[3]),
        makePlane(-transformation[1] + transformation[3]),
        makePlane(+transformation[1] + transformation[3]),
        makePlane(-transformation[2] + transformation[3]),
        makePlane(+transformation[2] + transformation[3])};
}

} // namespace ape
