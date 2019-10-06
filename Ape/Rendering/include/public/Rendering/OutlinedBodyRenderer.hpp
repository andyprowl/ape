#pragma once

#include <Rendering/LineStyle.hpp>

#include <Scene/BodyRange.hpp>

namespace ape
{

class Body;
class Camera;
class StandardBodyRenderer;
class WireframeBodyRenderer;

class OutlinedBodyRenderer
{

public:

    OutlinedBodyRenderer(
        StandardBodyRenderer const & standardRenderer,
        WireframeBodyRenderer const & wireframeRenderer);

    auto render(
        BodyRange const & bodies,
        Camera const & camera,
        LineStyle const & outlineStyle) const
        -> void;

private:

    auto performStandardRenderingAndFillStencilBuffer(
        BodyRange const & bodies,
        Camera const & camera) const
        -> void;

    auto performWireframeRenderingWhereStencilBuffferIsNotFilled(
        BodyRange const & bodies,
        Camera const & camera,
        LineStyle const & outlineStyle) const
        -> void;

    auto restoreDefaultStencilState() const
        -> void;

private:

    StandardBodyRenderer const * standardRenderer;
    
    WireframeBodyRenderer const * wireframeRenderer;

};

} // namespace ape
