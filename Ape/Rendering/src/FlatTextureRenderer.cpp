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
    auto const shaderBinder = ScopedBinder{*shader};

    shader->source = texture;

    glDisable(GL_DEPTH_TEST);

    drawQuad();

    glEnable(GL_DEPTH_TEST);
}

auto FlatTextureRenderer::drawQuad() const
    -> void
{
    auto const arrayObjectBinder = ScopedBinder{planeShape.getArrayObject()};

    auto const numOfVertices = planeShape.getNumOfVertices();

    glDrawElements(GL_TRIANGLES, numOfVertices, GL_UNSIGNED_INT, 0);
}

} // namespace ape
