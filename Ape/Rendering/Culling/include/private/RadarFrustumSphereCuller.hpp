#pragma once

#include <Ape/Rendering/Culling/RadarFrustumCuller.hpp>

namespace ape
{

class RadarFrustumCuller::SphereCuller
{

public:

    SphereCuller(
        Camera const & camera,
        Sphere const & sphere,
        PrecalculatedValueSet const & precalculatedValues);

    auto computeRelation() const
        -> ContainmentRelation;

private:

    auto computeCameraZ() const
        -> float;

    auto computeCameraY() const
        -> float;

    auto computeCameraX() const
        -> float;

    auto computeRelationOnZ(float const cameraZ) const
        -> ContainmentRelation;

    auto computeRelationOnY(float const cameraY, float const cameraZ) const
        -> ContainmentRelation;

    auto computeRelationOnX(float const cameraX, float const cameraZ) const
        -> ContainmentRelation;

    auto getSphereCenterInCameraSystem() const
        -> glm::vec3;

    auto determineContainmentRelation(
        ContainmentRelation const relationOnX,
        ContainmentRelation const relationOnY,
        ContainmentRelation const relationOnZ) const
        -> ContainmentRelation;

private:

    Camera const * camera;

    Sphere const * sphere;

    PrecalculatedValueSet precalculatedValues;

    float radius;

    glm::vec3 center;

};

} // namespace ape
