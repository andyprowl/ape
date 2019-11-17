#include <Ape/Rendering/Lighting/FrustumCuller.hpp>

#include <Ape/World/Shape/ShapeBounds.hpp>
#include <Ape/World/Scene/Camera.hpp>

#include <glm/trigonometric.hpp>

namespace ape
{

namespace
{

auto getCameraProjection(Camera const & camera)
    -> PerspectiveProjection const &
{
    return camera.getProjection().as<PerspectiveProjection>();
}

auto getCameraFrustum(Camera const & camera)
    -> PerspectiveProjection::Frustum const &
{
    auto const & projection = getCameraProjection(camera);

    return projection.getFrustum();
}

} // unnamed namespace

class FrustumCuller::SphereCuller
{

public:

    SphereCuller(
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

    auto computeRelation()
        -> FrustumRelation
    {
        auto const cameraZ = computeCameraZ();

        auto const relationOnZ = computeRelationOnZ(cameraZ);

        if (relationOnZ == FrustumRelation::fullyOutside)
        {
            return FrustumRelation::fullyOutside;
        }

        auto const cameraY = computeCameraY();

        auto const relationOnY = computeRelationOnY(cameraY, cameraZ);

        if (relationOnY == FrustumRelation::fullyOutside)
        {
            return FrustumRelation::fullyOutside;
        }

        auto const cameraX = computeCameraX();

        auto const relationOnX = computeRelationOnX(cameraX, cameraZ);

        if (relationOnX == FrustumRelation::fullyOutside)
        {
            return FrustumRelation::fullyOutside;
        }

        return determineFrustumRelation(relationOnX, relationOnY, relationOnZ);
    }

private:

    auto computeCameraZ() const
        -> float
    {
        auto const & lookAt = camera->getView().getDirection();

        return glm::dot(center, lookAt);
    }

    auto computeCameraY() const
        -> float
    {
        auto const & up = camera->getView().getDirection();

        return glm::dot(center, up);
    }

    auto computeCameraX() const
        -> float
    {
        auto const & right = camera->getView().getRight();

        return glm::dot(center, right);
    }

    auto computeRelationOnZ(const float cameraZ)
        -> FrustumRelation
    {
        auto const & frustum = getCameraFrustum(*camera);

        if ((cameraZ < (frustum.near - radius)) || (cameraZ > (frustum.far + radius)))
        {
            return FrustumRelation::fullyOutside;
        }
    
        if ((cameraZ < (frustum.near + radius)) || (cameraZ > (frustum.far - radius)))
        {
            return FrustumRelation::partlyInside;
        }

        return FrustumRelation::fullyInside;
    }

    auto computeRelationOnY(const float cameraY, const float cameraZ)
        -> FrustumRelation
    {
        auto const dY = radius * precalculatedValues.factorY;

        auto const thresholdY = cameraZ * precalculatedValues.factorY;

        if ((cameraY > (thresholdY + dY)) || (cameraY < (-thresholdY - dY)))
        {
            return FrustumRelation::fullyOutside;
        }

        if ((cameraY > (thresholdY - dY)) || (cameraY < (-thresholdY + dY)))
        {
            return FrustumRelation::partlyInside;
        }

        return FrustumRelation::fullyInside;
    }

    auto computeRelationOnX(const float cameraX, const float cameraZ)
        -> FrustumRelation
    {
        auto const dX = radius * precalculatedValues.factorX;

        auto const thresholdX = cameraZ * precalculatedValues.factorX;

        if ((cameraX > (thresholdX + dX)) || (cameraX < (-thresholdX - dX)))
        {
            return FrustumRelation::fullyOutside;
        }

        if ((cameraX > (thresholdX - dX)) || (cameraX < (-thresholdX + dX)))
        {
            return FrustumRelation::partlyInside;
        }

        return FrustumRelation::fullyInside;
    }

    auto getSphereCenterInCameraSystem() const
        -> glm::vec3
    {
        auto const & cameraPosition = camera->getView().getPosition();

        auto const & sphereCenter = sphere->getCenter();

        return (sphereCenter - cameraPosition);
    }

    auto determineFrustumRelation(
        FrustumRelation const relationOnX,
        FrustumRelation const relationOnY,
        FrustumRelation const relationOnZ) const
        -> FrustumRelation
    {
        auto const isFullyInside =
            (relationOnX == FrustumRelation::fullyInside) &&
            (relationOnY == FrustumRelation::fullyInside) &&
            (relationOnZ == FrustumRelation::fullyInside);

        return (isFullyInside ? FrustumRelation::fullyInside : FrustumRelation::partlyInside);
    }

private:

    Camera const * camera;

    Sphere const * sphere;

    PrecalculatedValueSet precalculatedValues;

    float radius;

    glm::vec3 center;

};

FrustumCuller::FrustumCuller(Camera const & camera)
    : camera{&camera}
    , precalculatedValues{precalculateValues()}
{
}

auto FrustumCuller::computeFrustumRelation(ShapeBounds const & bounds) const
    -> FrustumRelation
{
    return computeFrustumRelation(bounds.getSphere());
}

auto FrustumCuller::computeFrustumRelation(Sphere const & sphere) const
    -> FrustumRelation
{
    return SphereCuller{*camera, sphere, precalculatedValues}.computeRelation();
}

auto FrustumCuller::precalculateValues() const
    -> PrecalculatedValueSet
{
    auto const & frustum = getCameraFrustum(*camera);

    auto const halfFieldOfViewY = frustum.fieldOfView / 2.0f;

    auto const factorY = 1.0f / glm::cos(halfFieldOfViewY);

    auto const tangent = glm::tan(halfFieldOfViewY);

    auto const halfFieldOfViewX = glm::atan(tangent * frustum.aspectRatio);

    auto const factorX = 1.0f / glm::cos(halfFieldOfViewX);

    return {tangent, factorX, factorY};
}

} // namespace ape
