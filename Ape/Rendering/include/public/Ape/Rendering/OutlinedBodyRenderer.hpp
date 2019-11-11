#pragma once

#include <Ape/Rendering/StandardBodyRenderer.hpp>
#include <Ape/Rendering/WireframeBodyRenderer.hpp>

#include <Ape/Scene/BodyRange.hpp>

namespace ape
{

class Body;
class Camera;
class LightSystem;
class ShadowMapping;

class OutlinedBodyRenderer
{

public:

    OutlinedBodyRenderer(
        StandardBodyRenderer standardRenderer,
        WireframeBodyRenderer wireframeRenderer);

    auto setViewport(Viewport const & newViewport)
        -> void;

    auto render(
        BodyRange const & bodies,
        Camera const & camera,
        LightSystem const & lightSystem,
        ShadowMapping const & shadowMapping) const
        -> void;

private:

    auto performStandardRenderingAndFillStencilBuffer(
        BodyRange const & bodies,
        Camera const & camera,
        LightSystem const & lightSystem,
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
