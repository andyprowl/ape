#include "RadarFrustumSphereCuller.hpp"

#include "CameraFrustum.hpp"

#include <Ape/World/Scene/Camera.hpp>
#include <Ape/World/Shape/Sphere.hpp>

namespace ape
{

RadarFrustumCuller::SphereCuller::SphereCuller(
    Camera const & camera,
    Sphere const & sphere,
    PrecalculatedValueSet const & precalculatedValues)
    : camera{&camera}
    , sphere{&sphere}
    , precalculatedValues{precalculatedValues}
    , radius{sphere.getRadius()}
    , center{getSphereCenterInCameraSystem()}
{
}

auto RadarFrustumCuller::SphereCuller::computeRelation() const
    -> ContainmentRelation
{
    auto const cameraZ = computeCameraZ();

    auto const relationOnZ = computeRelationOnZ(cameraZ);

    if (relationOnZ == ContainmentRelation::fullyOutside)
    {
        return ContainmentRelation::fullyOutside;
    }

    auto const cameraY = computeCameraY();

    auto const relationOnY = computeRelationOnY(cameraY, cameraZ);

    if (relationOnY == ContainmentRelation::fullyOutside)
    {
        return ContainmentRelation::fullyOutside;
    }

    auto const cameraX = computeCameraX();

    auto const relationOnX = computeRelationOnX(cameraX, cameraZ);

    if (relationOnX == ContainmentRelation::fullyOutside)
    {
        return ContainmentRelation::fullyOutside;
    }

    return determineContainmentRelation(relationOnX, relationOnY, relationOnZ);
}

auto RadarFrustumCuller::SphereCuller::computeCameraZ() const
    -> float
{
    auto const & lookAt = camera->getView().getDirection();

    return glm::dot(center, lookAt);
}

auto RadarFrustumCuller::SphereCuller::computeCameraY() const
    -> float
{
    auto const & up = camera->getView().getUp();

    return glm::dot(center, up);
}

auto RadarFrustumCuller::SphereCuller::computeCameraX() const
    -> float
{
    auto const & right = camera->getView().getRight();

    return glm::dot(center, right);
}

auto RadarFrustumCuller::SphereCuller::computeRelationOnZ(const float cameraZ) const
    -> ContainmentRelation
{
    auto const & frustum = ape::getPerspectiveFrustum(*camera);

    if ((cameraZ < (frustum.near - radius)) || (cameraZ > (frustum.far + radius)))
    {
        return ContainmentRelation::fullyOutside;
    }
    
    if ((cameraZ < (frustum.near + radius)) || (cameraZ > (frustum.far - radius)))
    {
        return ContainmentRelation::partlyInside;
    }

    return ContainmentRelation::fullyInside;
}

auto RadarFrustumCuller::SphereCuller::computeRelationOnY(
    const float cameraY,
    const float cameraZ) const
    -> ContainmentRelation
{
    auto const dY = radius * precalculatedValues.factorY;

    auto const tangent = precalculatedValues.fieldOfViewTangent;

    auto const thresholdY = glm::abs(cameraZ * tangent);

    auto const normalizedCameraY = cameraY * glm::sign(cameraZ);

    if ((normalizedCameraY > (thresholdY + dY)) || (normalizedCameraY < (-thresholdY - dY)))
    {
        return ContainmentRelation::fullyOutside;
    }

    if ((normalizedCameraY > (thresholdY - dY)) || (normalizedCameraY < (-thresholdY + dY)))
    {
        return ContainmentRelation::partlyInside;
    }

    return ContainmentRelation::fullyInside;
}

auto RadarFrustumCuller::SphereCuller::computeRelationOnX(
    const float cameraX,
    const float cameraZ) const
    -> ContainmentRelation
{
    auto const dX = radius * precalculatedValues.factorX;

    auto const tangent = precalculatedValues.fieldOfViewTangent;

    auto const aspectRatio = ape::getPerspectiveFrustum(*camera).aspectRatio;

    auto const thresholdX = glm::abs(cameraZ * tangent * aspectRatio);

    auto const normalizedCameraX = cameraX * glm::sign(cameraZ);

    if ((normalizedCameraX > (thresholdX + dX)) || (normalizedCameraX < (-thresholdX - dX)))
    {
        return ContainmentRelation::fullyOutside;
    }

    if ((normalizedCameraX > (thresholdX - dX)) || (normalizedCameraX < (-thresholdX + dX)))
    {
        return ContainmentRelation::partlyInside;
    }

    return ContainmentRelation::fullyInside;
}

auto RadarFrustumCuller::SphereCuller::getSphereCenterInCameraSystem() const
    -> glm::vec3
{
    auto const & cameraPosition = camera->getView().getPosition();

    auto const & sphereCenter = sphere->getCenter();

    return (sphereCenter - cameraPosition);
}

auto RadarFrustumCuller::SphereCuller::determineContainmentRelation(
    ContainmentRelation const relationOnX,
    ContainmentRelation const relationOnY,
    ContainmentRelation const relationOnZ) const
    -> ContainmentRelation
{
    auto const isFullyInside =
        (relationOnX == ContainmentRelation::fullyInside) &&
        (relationOnY == ContainmentRelation::fullyInside) &&
        (relationOnZ == ContainmentRelation::fullyInside);

    return (isFullyInside ? ContainmentRelation::fullyInside : ContainmentRelation::partlyInside);
};

} // namespace ape
