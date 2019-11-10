#include <Ape/Rendering/DepthBodyRenderer.hpp>

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
    LightingView const & lightingView,
    DepthMapping & target) const
    -> void
{
    monoRenderer.render(bodies, lightingView, target);

    omniRenderer.render(bodies, lightingView, target);
}

} // namespace ape
