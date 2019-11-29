#pragma once

#include <Ape/Rendering/Culling/Culler.hpp>
#include <Ape/Rendering/Culling/Frustum.hpp>

namespace ape
{

class Camera;

class PlaneFrustumCuller : public Culler
{

public:

    explicit PlaneFrustumCuller(Frustum const & frustum);

    explicit PlaneFrustumCuller(Camera const & camera);

    auto isFrustumFullyOutside(Frustum const & f) const
        -> bool;

    // virtual (from Culler)
    auto isSphereContained(Sphere const & sphere) const
        -> ContainmentRelation override;
        
    // virtual (from Culler)
    auto isPointContained(glm::vec3 const & point) const
        -> bool override;

private:

    Frustum frustum;

};

} // namespace ape
