#pragma once

#include <Ape/Rendering/Culling/RadarFrustumCuller.hpp>

namespace ape
{

class RadarFrustumCuller::PointCuller
{

public:

    PointCuller(
        Camera const & camera,
        glm::vec3 const & point,
        PrecalculatedValueSet const & precalculatedValues);

    auto computeContainment() const
        -> bool;

private:

    auto computeCameraZ() const
        -> float;

    auto computeCameraY() const
        -> float;

    auto computeCameraX() const
        -> float;

    auto isContainedOnZ(float const cameraZ) const
        -> bool;

    auto isContainedOnY(float const cameraY, float const cameraZ) const
        -> bool;

    auto isContainedOnX(float const cameraX, float const cameraZ) const
        -> bool;

    auto getPointInCameraSystem(glm::vec3 const & point) const
        -> glm::vec3;

private:

    Camera const * camera;

    glm::vec3 point;

    PrecalculatedValueSet precalculatedValues;

};

} // namespace ape
