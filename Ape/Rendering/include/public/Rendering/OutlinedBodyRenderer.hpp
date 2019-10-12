#pragma once

#include <Rendering/StandardBodyRenderer.hpp>
#include <Rendering/WireframeBodyRenderer.hpp>

#include <Scene/BodyRange.hpp>

namespace ape
{

class Body;
class Camera;
class Lighting;
class ShadowMapping;

class OutlinedBodyRenderer
{

public:

    OutlinedBodyRenderer(
        StandardBodyRenderer standardRenderer,
        WireframeBodyRenderer wireframeRenderer);
    
    auto getOutliningStyle() const
        -> LineStyle;

    auto setOutliningStyle(LineStyle const & newStyle)
        -> void;

    auto setViewport(Viewport const & newViewport)
        -> void;

    auto render(
        BodyRange const & bodies,
        Camera const & camera,
        Lighting const & lighting,
        ShadowMapping const & shadowMapping) const
        -> void;

private:

    auto performStandardRenderingAndFillStencilBuffer(
        BodyRange const & bodies,
        Camera const & camera,
        Lighting const & lighting,
        ShadowMapping const & shadowMapping) const
        -> void;

    auto performWireframeRenderingWhereStencilBuffferIsNotFilled(
        BodyRange const & bodies,
        Camera const & camera) const
        -> void;

private:

    StandardBodyRenderer standardRenderer;
    
    WireframeBodyRenderer wireframeRenderer;

};

} // namespace ape
