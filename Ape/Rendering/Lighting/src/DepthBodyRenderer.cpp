#include <Ape/Rendering/Lighting/DepthBodyRenderer.hpp>

#include <Ape/Rendering/Lighting/DepthMapping.hpp>

#include <glad/glad.h>

#include <iostream>

namespace ape
{

namespace
{

auto printOmnidirectionalFlatMappingStatus(bool const enabled)
    -> void
{
    std::cout
        << "Omnidirectional shadow flat mapping "
        << (enabled ? "enabled" : "disabled")
        << "\n";
}

} // unnamed namespace

DepthBodyRenderer::DepthBodyRenderer(
    MonoDepthBodyRenderer monoRenderer,
    OmniDepthCubeBodyRenderer omniCubeRenderer,
    OmniDepthFlatBodyRenderer omniFlatRenderer)
    : monoRenderer{std::move(monoRenderer)}
    , omniCubeRenderer{std::move(omniCubeRenderer)}
    , omniFlatRenderer{std::move(omniFlatRenderer)}
    , useOmniFlatRenderer{true}
{
    printOmnidirectionalFlatMappingStatus(useOmniFlatRenderer);
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

    if (useOmniFlatRenderer)
    {
        omniFlatRenderer.render(bodies, viewerCamera, lightSystemView, target);
    }
    else
    {
        omniCubeRenderer.render(bodies, lightSystemView, target);
    }
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

    omniFlatRenderer.enableFrustumCulling(enable);
}

auto DepthBodyRenderer::isOmniFlatShadowMappingEnabled() const
    -> bool
{
    return useOmniFlatRenderer;
}

auto DepthBodyRenderer::enableOmniFlatShadowMapping(bool const enable)
    -> void
{
    useOmniFlatRenderer = enable;

    printOmnidirectionalFlatMappingStatus(useOmniFlatRenderer);
}

auto DepthBodyRenderer::setProfiler(TaskTimeProfiler & newProfiler)
    -> void
{
    monoRenderer.setProfiler(newProfiler);

    omniCubeRenderer.setProfiler(newProfiler);

    omniFlatRenderer.setProfiler(newProfiler);
}

auto DepthBodyRenderer::setupViewport(DepthMapping & target) const
    -> void
{
    auto const mapSize = target.getMapSize();

    glViewport(0, 0, mapSize.width, mapSize.height);
}

} // namespace ape
