#include <Ape/Rendering/Lighting/DepthBodyRenderer.hpp>

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

} // namespace ape
