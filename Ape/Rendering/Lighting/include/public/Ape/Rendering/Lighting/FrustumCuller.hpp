#pragma once

#include <Ape/World/Scene/PerspectiveProjection.hpp>

#include <glm/vec3.hpp>

namespace ape
{

class Camera;
class ShapeBounds;
class Sphere;

class FrustumCuller
{

public:

    enum class FrustumRelation
    {
        fullyOutside,
        partlyInside,
        fullyInside
    };

public:

    explicit FrustumCuller(Camera const & camera);

    auto computeFrustumRelation(ShapeBounds const & bounds) const
        -> FrustumRelation;

    auto computeFrustumRelation(Sphere const & sphere) const
        -> FrustumRelation;

private:

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
