#pragma once

#include <Ape/Rendering/Culling/Culler.hpp>
#include <Ape/Rendering/Culling/ConvexVolume.hpp>

namespace ape
{

class Camera;
class Sphere;

class PerspectiveLightCuller : public Culler
{

public:

    PerspectiveLightCuller(Camera const & lightPerspective, Camera const & viewerPerspective);

    auto isCullingVolumeEmpty() const
        -> bool;

    // virtual (from Culler)
    auto isSphereContained(Sphere const & sphere) const
        -> ContainmentRelation override;

    // virtual (from Culler)
    auto isPointContained(glm::vec3 const & point) const
        -> bool override;

private:

    auto makeCullingVolume() const
        -> ConvexVolume;

private:

    Camera const * lightPerspective;
    
    Camera const * viewerPerspective;

    ConvexVolume cullingVolume;

};

} // namespace ape
