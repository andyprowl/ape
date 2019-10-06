#include <Rendering/OutlinedBodyRenderer.hpp>

#include <Rendering/StandardBodyRenderer.hpp>
#include <Rendering/WireframeBodyRenderer.hpp>

#include <glad/glad.h>

namespace ape
{

namespace
{

auto clearStencilBuffer()
    -> void
{
    glStencilMask(0xff);

    glClear(GL_STENCIL_BUFFER_BIT);
}

} // unnamed namespace

OutlinedBodyRenderer::OutlinedBodyRenderer(
    StandardBodyRenderer const & standardRenderer,
    WireframeBodyRenderer const & wireframeRenderer)
    : standardRenderer{&standardRenderer}
    , wireframeRenderer{&wireframeRenderer}
{
}

auto OutlinedBodyRenderer::render(
    BodyRange const & bodies,
    Camera const & camera,
    LineStyle const & outlineStyle) const
    -> void
{
    performStandardRenderingAndFillStencilBuffer(bodies, camera);

    performWireframeRenderingWhereStencilBuffferIsNotFilled(bodies, camera, outlineStyle);

    restoreDefaultStencilState();
}

auto OutlinedBodyRenderer::performStandardRenderingAndFillStencilBuffer(
    BodyRange const & bodies,
    Camera const & camera) const
    -> void
{
    clearStencilBuffer();

    // The stencil test always passes.
    glStencilFunc(GL_ALWAYS, 1, 0xff);

    // When depth (and stencil) tests pass, replace the current stencil value with 1 (which is the
    // reference value specified when calling glStencilFunc()).
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    standardRenderer->render(bodies, camera);
}

auto OutlinedBodyRenderer::performWireframeRenderingWhereStencilBuffferIsNotFilled(
    BodyRange const & bodies,
    Camera const & camera,
    LineStyle const & outlineStyle) const
    -> void
{
    // The stencil test passes if the value is not 1 (pixels *not* drawn in the previous call).
    glStencilFunc(GL_NOTEQUAL, 1, 0xff);

    glStencilMask(0x00);
    
    wireframeRenderer->render(bodies, camera, outlineStyle);
}

auto OutlinedBodyRenderer::restoreDefaultStencilState() const
    -> void
{
    glStencilFunc(GL_ALWAYS, 0, 0xff);

    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

} // namespace ape
