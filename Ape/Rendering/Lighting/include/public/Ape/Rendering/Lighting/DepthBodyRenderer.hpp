#pragma once

#include <Ape/Rendering/Lighting/MonoDepthBodyRenderer.hpp>
#include <Ape/Rendering/Lighting/OmniDepthCubeBodyRenderer.hpp>
#include <Ape/Rendering/Lighting/OmniDepthFlatBodyRenderer.hpp>

namespace ape
{

class Camera;

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

private:

    MonoDepthBodyRenderer monoRenderer;
    
    OmniDepthCubeBodyRenderer omniCubeRenderer;

    OmniDepthFlatBodyRenderer omniFlatRenderer;

    bool useOmniFlatRenderer;

};

} // namespace ape
