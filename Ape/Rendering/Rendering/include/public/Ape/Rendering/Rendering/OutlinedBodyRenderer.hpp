#pragma once

#include <Ape/Rendering/Lighting/BlinnPhongBodyRenderer.hpp>
#include <Ape/Rendering/Wireframe/WireframeBodyRenderer.hpp>

#include <Ape/World/Scene/BodyRange.hpp>

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
        BlinnPhongBodyRenderer standardRenderer,
        WireframeBodyRenderer wireframeRenderer);

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

    BlinnPhongBodyRenderer standardRenderer;
    
    WireframeBodyRenderer wireframeRenderer;

};

} // namespace ape
