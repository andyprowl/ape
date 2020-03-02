#include <Ape/Rendering/Lighting/DepthBodyRenderer.hpp>

#include <Ape/Rendering/Lighting/DepthMapping.hpp>

#include <glad/glad.h>

#include <iostream>

namespace ape
{

DepthBodyRenderer::DepthBodyRenderer(
    MonoDepthBodyRenderer monoRenderer,
    OmniDepthBodyRenderer omniRenderer)
    : monoRenderer{std::move(monoRenderer)}
    , omniRenderer{std::move(omniRenderer)}
{
}

auto DepthBodyRenderer::render(
    BodySetView const & bodies,
    Camera const & viewerCamera,
    LightSystemView const & lightSystemView,
    DepthMapping & target) const
    -> void
{
    setupViewport(target);

    monoRenderer.render(bodies, viewerCamera, lightSystemView, target);

    omniRenderer.render(bodies, viewerCamera, lightSystemView, target);
}

auto DepthBodyRenderer::isFrustumCullingEnabled() const
    -> bool
{
    return monoRenderer.isFrustumCullingEnabled();
}

auto DepthBodyRenderer::enableFrustumCulling(bool const enable)
    -> void
{
    monoRenderer.enableFrustumCulling(enable);

    omniRenderer.enableFrustumCulling(enable);
}

auto DepthBodyRenderer::setProfiler(TaskTimeProfiler & newProfiler)
    -> void
{
    monoRenderer.setProfiler(newProfiler);

    omniRenderer.setProfiler(newProfiler);
}

auto DepthBodyRenderer::setupViewport(DepthMapping & target) const
    -> void
{
    auto const mapSize = target.getMapSize();

    glViewport(0, 0, mapSize.width, mapSize.height);
}

} // namespace ape
