#include "RadarFrustumPointCuller.hpp"

#include "CameraFrustum.hpp"

#include <Ape/World/Scene/Camera.hpp>
#include <Ape/World/Shape/Sphere.hpp>

namespace ape
{

RadarFrustumCuller::PointCuller::PointCuller(
    Camera const & camera,
    glm::vec3 const & point,
    PrecalculatedValueSet const & precalculatedValues)
    : camera{&camera}
    , point{getPointInCameraSystem(point)}
    , precalculatedValues{precalculatedValues}
{
}

auto RadarFrustumCuller::PointCuller::computeContainment() const
    -> bool
{
    auto const cameraZ = computeCameraZ();

    if (!isContainedOnZ(cameraZ))
    {
        return false;
    }

    auto const cameraY = computeCameraY();

    if (!isContainedOnY(cameraY, cameraZ))
    {
        return false;
    }

    auto const cameraX = computeCameraX();

    return isContainedOnX(cameraX, cameraZ);
}

auto RadarFrustumCuller::PointCuller::computeCameraZ() const
    -> float
{
    auto const & lookAt = camera->getView().getDirection();

    return glm::dot(point, lookAt);
}

auto RadarFrustumCuller::PointCuller::computeCameraY() const
    -> float
{
    auto const & up = camera->getView().getUp();

    return glm::dot(point, up);
}

auto RadarFrustumCuller::PointCuller::computeCameraX() const
    -> float
{
    auto const & right = camera->getView().getRight();

    return glm::dot(point, right);
}

auto RadarFrustumCuller::PointCuller::isContainedOnZ(const float cameraZ) const
    -> bool
{
    auto const & frustum = ape::getPerspectiveFrustum(*camera);

    return ((cameraZ >= frustum.near) && (cameraZ <= frustum.far));
}

auto RadarFrustumCuller::PointCuller::isContainedOnY(
    const float cameraY,
    const float cameraZ) const
    -> bool
{
    auto const thresholdY = cameraZ * precalculatedValues.fieldOfViewTangent;
    
    return ((cameraY >= -thresholdY) && (cameraY <= thresholdY));
}

auto RadarFrustumCuller::PointCuller::isContainedOnX(
    const float cameraX,
    const float cameraZ) const
    -> bool
{
    auto const tangent = precalculatedValues.fieldOfViewTangent;

    auto const aspectRatio = ape::getPerspectiveFrustum(*camera).aspectRatio;

    auto const thresholdX = cameraZ * tangent * aspectRatio;

    return ((cameraX >= -thresholdX) || (cameraX <= thresholdX));
}

auto RadarFrustumCuller::PointCuller::getPointInCameraSystem(glm::vec3 const & p) const
    -> glm::vec3
{
    auto const & cameraPosition = camera->getView().getPosition();

    return (p - cameraPosition);
}

} // namespace ape
