#include <Ape/Rendering/ShapeBufferObjectRenderer.hpp>

#include <Ape/GpuResource/VertexLayout.hpp>
#include <Ape/Model/Shape.hpp>

#include <glad/glad.h>

namespace ape
{

auto ShapeBufferObjectRenderer::render(Shape const & shape) const
    -> void
{
    shape.getVertexBufferObject().bind();

    shape.getElementBufferObject().bind();

    sendVertexLayoutToGpu<ShapeVertex>();

    auto const numOfVertices = shape.getNumOfVertices();

    glDrawElements(GL_TRIANGLES, numOfVertices, GL_UNSIGNED_INT, 0);
}

} // namespace ape
