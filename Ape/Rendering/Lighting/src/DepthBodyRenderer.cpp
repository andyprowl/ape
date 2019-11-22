#include <Ape/Rendering/Lighting/DepthBodyRenderer.hpp>

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
    LightSystemView const & lightSystemView,
    DepthMapping & target) const
    -> void
{
    monoRenderer.render(bodies, lightSystemView, target);

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
