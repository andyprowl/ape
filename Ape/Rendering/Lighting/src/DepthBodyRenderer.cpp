#include <Ape/Rendering/Lighting/DepthBodyRenderer.hpp>

namespace ape
{

DepthBodyRenderer::DepthBodyRenderer(
    MonoDepthBodyRenderer monoRenderer,
    OmniDepthCubeBodyRenderer omniCubeRenderer,
    OmniDepthFlatBodyRenderer omniFlatRenderer)
    : monoRenderer{std::move(monoRenderer)}
    , omniCubeRenderer{std::move(omniCubeRenderer)}
    , omniFlatRenderer{std::move(omniFlatRenderer)}
    , useOmniFlatRenderer{false}
{
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
}

} // namespace ape
