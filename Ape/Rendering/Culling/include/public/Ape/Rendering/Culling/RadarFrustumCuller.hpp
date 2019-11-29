#pragma once

#include <Ape/Rendering/Culling/Culler.hpp>

#include <Ape/World/Scene/PerspectiveProjection.hpp>

namespace ape
{

class Camera;

class RadarFrustumCuller : public Culler
{

public:

    explicit RadarFrustumCuller(Camera const & camera);

    // virtual (from Culler)
    auto isSphereContained(Sphere const & sphere) const
        -> ContainmentRelation override;

    // virtual (from Culler)
    auto isPointContained(glm::vec3 const & point) const
        -> bool override;

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
