#pragma once

#include <Ape/Rendering/Culling/Culler.hpp>
#include <Ape/Rendering/Culling/Frustum.hpp>

namespace ape
{

class Camera;

class PlaneFrustumCuller : public Culler
{

public:

    explicit PlaneFrustumCuller(Camera const & camera);

    // virtual (from Culler)
    auto isSphereContained(Sphere const & sphere) const
        -> ContainmentRelation override;
        
    // virtual (from Culler)
    auto isPointContained(glm::vec3 const & point) const
        -> bool override;

private:

    Camera const * camera;

    Frustum frustum;

};

} // namespace ape
