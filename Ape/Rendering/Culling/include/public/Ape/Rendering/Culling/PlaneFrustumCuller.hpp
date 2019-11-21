#pragma once

#include <Ape/Rendering/Culling/ContainmentRelation.hpp>

#include <Ape/Rendering/Culling/Frustum.hpp>

namespace ape
{

class Camera;
class Frustum;
class Sphere;

class PlaneFrustumCuller
{

public:

    explicit PlaneFrustumCuller(Camera const & camera);

    auto computeFrustumRelation(Sphere const & sphere) const
        -> ContainmentRelation;

    auto computeFrustumRelation(Frustum const & f) const
        -> ContainmentRelation;
        
    auto isPointInFrustum(glm::vec3 const & point) const
        -> bool;

private:

    auto extractCameraFrustum() const
        -> Frustum;

private:

    Camera const * camera;

    Frustum frustum;

};

} // namespace ape
