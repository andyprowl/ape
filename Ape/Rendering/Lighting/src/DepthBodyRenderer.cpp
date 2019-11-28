#include <Ape/Rendering/Lighting/DepthBodyRenderer.hpp>

namespace ape
{

DepthBodyRenderer::DepthBodyRenderer(
    MonoDepthBodyRenderer monoRenderer,
    OmniDepthCubeBodyRenderer omniRenderer)
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
    monoRenderer.render(bodies, viewerCamera, lightSystemView, target);

    omniRenderer.render(bodies, lightSystemView, target);
}

auto DepthBodyRenderer::isFrustumCullingEnabled() const
    -> bool
{
    return monoRenderer.isFrustumCullingEnabled();
}

auto DepthBodyRenderer::enableFrustumCulling(bool const enable)
    -> void
{
    return monoRenderer.enableFrustumCulling(enable);
}

} // namespace ape
