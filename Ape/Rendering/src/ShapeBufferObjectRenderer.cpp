#include <Rendering/ShapeBufferObjectRenderer.hpp>

#include <Model/Shape.hpp>
#include <Model/VertexLayout.hpp>

#include <glad/glad.h>

namespace ape
{

auto ShapeBufferObjectRenderer::render(Shape const & shape) const
    -> void
{
    shape.getVertexBufferObject().bind();

    sendVertexLayoutToGpu<ShapeVertex>();

    shape.getElementBufferObject().bind();

    auto const numOfVertices = shape.getNumOfVertices();

    glDrawElements(GL_TRIANGLES, numOfVertices, GL_UNSIGNED_INT, 0);
}

} // namespace ape
