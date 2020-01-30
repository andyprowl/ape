#include <Ape/Rendering/Effect/EffectRenderer.hpp>

#include <Ape/Rendering/Effect/EffectSelector.hpp>
#include <Ape/Rendering/Effect/EffectShaderProgram.hpp>

#include <Glow/GpuResource/ScopedBinder.hpp>
#include <Glow/Texture/Texture.hpp>

#include <glad/glad.h>

namespace ape
{

EffectRenderer::EffectRenderer(EffectSelector & selector)
    : selector{&selector}
{
}

auto EffectRenderer::render(glow::Texture const & texture) const
    -> void
{
    auto const effect = selector->getActiveEffect();

    if (effect == nullptr)
    {
        return;
    }

    renderWithEffect(texture, *effect);
}

auto EffectRenderer::renderWithEffect(
    glow::Texture const & texture,
    EffectShaderProgram & effect) const
    -> void
{
    auto const shaderBinder = glow::bind(effect);

    effect.source.set(texture);

    glDisable(GL_DEPTH_TEST);

    drawQuad();

    glEnable(GL_DEPTH_TEST);
}

auto EffectRenderer::drawQuad() const
    -> void
{
    auto const arrayObjectBinder = glow::bind(planeShape.getArrayObject());

    auto const numOfVertices = planeShape.getNumOfVertices();

    glDrawArrays(GL_TRIANGLES, 0, numOfVertices);
}

} // namespace ape
