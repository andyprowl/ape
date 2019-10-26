#include <Ape/Rendering/FlatTextureRenderer.hpp>

#include <Ape/Rendering/FlatQuadShaderProgram.hpp>

#include <Ape/GpuResource/Texture.hpp>
#include <Ape/GpuResource/ScopedBinder.hpp>

#include <glad/glad.h>

namespace ape
{

FlatTextureRenderer::FlatTextureRenderer(FlatQuadShaderProgram & shader)
    : shader{&shader}
{
}

auto FlatTextureRenderer::render(Texture const & texture) const
    -> void
{
    auto const shaderBinder = bind(*shader);

    shader->source = texture;

    glDisable(GL_DEPTH_TEST);

    drawQuad();

    glEnable(GL_DEPTH_TEST);
}

auto FlatTextureRenderer::drawQuad() const
    -> void
{
    auto const arrayObjectBinder = bind(planeShape.getArrayObject());

    auto const numOfVertices = planeShape.getNumOfVertices();

    glDrawArrays(GL_TRIANGLES, 0, numOfVertices);
}

} // namespace ape
