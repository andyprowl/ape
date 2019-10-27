#include <Ape/Rendering/EffectRenderer.hpp>

#include <Ape/Rendering/EffectSelector.hpp>
#include <Ape/Rendering/EffectShaderProgram.hpp>

#include <Ape/GpuResource/Texture.hpp>
#include <Ape/GpuResource/ScopedBinder.hpp>

#include <glad/glad.h>

namespace ape
{

EffectRenderer::EffectRenderer(EffectSelector & selector)
    : selector{&selector}
{
}

auto EffectRenderer::render(Texture const & texture) const
    -> void
{
    auto const effect = selector->getActiveEffect();

    if (effect == nullptr)
    {
        return;
    }

    renderWithEffect(texture, *effect);
}

auto EffectRenderer::renderWithEffect(Texture const & texture, EffectShaderProgram & effect) const
    -> void
{
    auto const shaderBinder = bind(effect);

    effect.source = texture;

    glDisable(GL_DEPTH_TEST);

    drawQuad();

    glEnable(GL_DEPTH_TEST);
}

auto EffectRenderer::drawQuad() const
    -> void
{
    auto const arrayObjectBinder = bind(planeShape.getArrayObject());

    auto const numOfVertices = planeShape.getNumOfVertices();

    glDrawArrays(GL_TRIANGLES, 0, numOfVertices);
}

} // namespace ape
