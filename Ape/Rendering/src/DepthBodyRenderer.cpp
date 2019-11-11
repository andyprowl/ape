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
    LightSystemView const & lightSystemView,
    DepthMapping & target) const
    -> void
{
    monoRenderer.render(bodies, lightSystemView, target);

    omniRenderer.render(bodies, lightSystemView, target);
}

} // namespace ape
