#include <Rendering/OutlinedBodyRenderer.hpp>

#include <Rendering/StandardBodyRenderer.hpp>
#include <Rendering/WireframeBodyRenderer.hpp>

#include <glad/glad.h>

namespace ape
{

namespace
{

auto const stencilReferenceValue = 1;

auto disableWritingToStencilBuffer()
    -> void
{
    glStencilMask(0xff);
}

auto clearStencilBuffer()
    -> void
{
    disableWritingToStencilBuffer();

    glClear(GL_STENCIL_BUFFER_BIT);
}

auto restoreDefaultStencilState()
    -> void
{
    glStencilFunc(GL_ALWAYS, 0, 0xff);

    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

} // unnamed namespace

OutlinedBodyRenderer::OutlinedBodyRenderer(
    StandardBodyRenderer standardRenderer,
    WireframeBodyRenderer wireframeRenderer)
    : standardRenderer{std::move(standardRenderer)}
    , wireframeRenderer{std::move(wireframeRenderer)}
{
}

auto OutlinedBodyRenderer::setViewport(Viewport const & newViewport)
    -> void
{
    standardRenderer.setViewport(newViewport);
}

auto OutlinedBodyRenderer::render(
    BodyRange const & bodies,
    Camera const & camera,
    Lighting const & lighting,
    ShadowMapping const & shadowMapping) const
    -> void
{
    performStandardRenderingAndFillStencilBuffer(bodies, camera, lighting, shadowMapping);

    performWireframeRenderingWhereStencilBuffferIsNotFilled(bodies, camera);

    restoreDefaultStencilState();
}

auto OutlinedBodyRenderer::performStandardRenderingAndFillStencilBuffer(
    BodyRange const & bodies,
    Camera const & camera,
    Lighting const & lighting,
    ShadowMapping const & shadowMapping) const
    -> void
{
    clearStencilBuffer();

    // Make the stencil test always pass. At the same time, set the reference value.
    glStencilFunc(GL_ALWAYS, stencilReferenceValue, 0xff);

    // When depth (and stencil) tests pass, replace the current stencil value with the reference
    // value specified when calling glStencilFunc().
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    standardRenderer.render(bodies, camera, lighting, shadowMapping);
}

auto OutlinedBodyRenderer::performWireframeRenderingWhereStencilBuffferIsNotFilled(
    BodyRange const & bodies,
    Camera const & camera) const
    -> void
{
    // The stencil test passes if the value is not 1 (pixels *not* drawn in the previous call).
    glStencilFunc(GL_NOTEQUAL, stencilReferenceValue, 0xff);

    disableWritingToStencilBuffer();
    
    wireframeRenderer.render(bodies, camera);
}

} // namespace ape
