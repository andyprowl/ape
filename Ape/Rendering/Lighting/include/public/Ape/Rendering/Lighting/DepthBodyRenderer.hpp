#pragma once

#include <Ape/Rendering/Lighting/MonoDepthBodyRenderer.hpp>
#include <Ape/Rendering/Lighting/OmniDepthCubeBodyRenderer.hpp>
#include <Ape/Rendering/Lighting/OmniDepthFlatBodyRenderer.hpp>

namespace ape
{

class Camera;
class TaskTimeProfiler;

class DepthBodyRenderer
{

public:

    using BodySetView = basix::ContainerView<std::vector<Body>>;

public:

    DepthBodyRenderer(
        MonoDepthBodyRenderer monoRenderer,
        OmniDepthCubeBodyRenderer omniCubeRenderer,
        OmniDepthFlatBodyRenderer omniFlatRenderer);

    auto render(
        BodySetView const & bodies,
        Camera const & viewerCamera,
        LightSystemView const & lightSystemView,
        DepthMapping & target) const
        -> void;

    auto isFrustumCullingEnabled() const
        -> bool;

    auto enableFrustumCulling(bool enable)
        -> void;

    auto isOmniFlatShadowMappingEnabled() const
        -> bool;

    auto enableOmniFlatShadowMapping(bool enable)
        -> void;

    auto setProfiler(TaskTimeProfiler & newProfiler)
        -> void;

private:

    MonoDepthBodyRenderer monoRenderer;
    
    OmniDepthCubeBodyRenderer omniCubeRenderer;

    OmniDepthFlatBodyRenderer omniFlatRenderer;

    bool useOmniFlatRenderer;

};

} // namespace ape
