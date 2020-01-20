#include <Ape/World/Shape/ShapeBufferObjectDrawer.hpp>

#include <Ape/World/Shape/Shape.hpp>

#include <Glow/BufferObject/VertexLayout.hpp>

#include <glad/glad.h>

namespace ape
{

// virtual (from ShapeDrawer)
auto ShapeBufferObjectDrawer::beginRenderBatch()
    -> void
{
}

auto ShapeBufferObjectDrawer::render(Shape const & shape)
    -> void
{
    shape.getVertexBufferObject().bind();

    shape.getElementBufferObject().bind();

    glow::sendVertexLayoutToGpu<ShapeVertex>();

    auto const numOfVertices = shape.getNumOfVertices();

    glDrawElements(GL_TRIANGLES, numOfVertices, GL_UNSIGNED_INT, 0);
}

// virtual (from ShapeDrawer)
auto ShapeBufferObjectDrawer::endRenderBatch() 
    -> void
{
}

} // namespace ape
