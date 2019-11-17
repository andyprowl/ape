#pragma once

#include <Ape/Rendering/Lighting/MonoDepthBodyRenderer.hpp>
#include <Ape/Rendering/Lighting/OmniDepthBodyRenderer.hpp>

namespace ape
{

class DepthBodyRenderer
{

public:

    using BodySetView = basix::ContainerView<std::vector<Body>>;

public:

    DepthBodyRenderer(MonoDepthBodyRenderer monoRenderer, OmniDepthBodyRenderer omniRenderer);

    auto render(
        BodySetView const & bodies,
        LightSystemView const & lightSystemView,
        DepthMapping & target) const
        -> void;

    auto isFrustumCullingEnabled() const
        -> bool;

    auto enableFrustumCulling(bool enable)
        -> void;

private:

    MonoDepthBodyRenderer monoRenderer;
    
    OmniDepthBodyRenderer omniRenderer;

};

} // namespace ape
