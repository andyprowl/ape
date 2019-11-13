#include <Ape/Shape/ShapeBufferObjectDrawer.hpp>

#include <Ape/Shape/Shape.hpp>

#include <Ape/BufferObject/VertexLayout.hpp>

#include <glad/glad.h>

namespace ape
{

auto ShapeBufferObjectDrawer::render(Shape const & shape) const
    -> void
{
    shape.getVertexBufferObject().bind();

    shape.getElementBufferObject().bind();

    sendVertexLayoutToGpu<ShapeVertex>();

    auto const numOfVertices = shape.getNumOfVertices();

    glDrawElements(GL_TRIANGLES, numOfVertices, GL_UNSIGNED_INT, 0);
}

} // namespace ape
