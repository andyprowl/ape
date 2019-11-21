#pragma once

#include <Ape/Rendering/Culling/ContainmentRelation.hpp>

#include <Ape/World/Scene/PerspectiveProjection.hpp>

#include <glm/vec3.hpp>

namespace ape
{

class Camera;
class Sphere;

class RadarFrustumCuller
{

public:

    explicit RadarFrustumCuller(Camera const & camera);

    auto computeFrustumRelation(Sphere const & sphere) const
        -> ContainmentRelation;

    auto isPointInFrustum(glm::vec3 const & point) const
        -> bool;

private:

    class PointCuller;
    class SphereCuller;

    class PrecalculatedValueSet
    {

    public:

        PrecalculatedValueSet(
            float const fieldOfViewTangent,
            float const factorX,
            float const factorY)
            : fieldOfViewTangent{fieldOfViewTangent}
            , factorX{factorX}
            , factorY{factorY}
        {
        }

    public:

        float fieldOfViewTangent;

        float factorX;

        float factorY;

    };

private:

    auto precalculateValues() const
        -> PrecalculatedValueSet;

private:

    Camera const * camera;

    PrecalculatedValueSet precalculatedValues;

};

} // namespace ape
