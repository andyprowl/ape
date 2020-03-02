#pragma once

#include <Ape/Rendering/Lighting/MonoDepthBodyRenderer.hpp>
#include <Ape/Rendering/Lighting/OmniDepthBodyRenderer.hpp>

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
        OmniDepthBodyRenderer omniFlatRenderer);

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

    auto setProfiler(TaskTimeProfiler & newProfiler)
        -> void;

private:

    auto setupViewport(DepthMapping & target) const
        -> void;

private:

    MonoDepthBodyRenderer monoRenderer;

    OmniDepthBodyRenderer omniRenderer;


};

} // namespace ape
