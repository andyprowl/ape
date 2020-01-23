#include <Ape/World/Shape/ShapeBufferObjectDrawer.hpp>

#include <Ape/World/Shape/Shape.hpp>
#include <Ape/World/Shape/ShapeBufferObjectCreator.hpp>

#include <Glow/BufferObject/VertexLayout.hpp>
#include <Glow/GpuResource/ScopedBinder.hpp>

#include <glad/glad.h>

namespace ape
{

ShapeBufferObjectDrawer::ShapeBufferObjectDrawer(std::vector<Shape *> const & shapes)
{
    // This design is weak and error-prone, as it relies on consistent order of insertion and
    // creation of shapes. It should be changed to something more robust.

    bufferObjects.resize(shapes.size());

    setupBufferObjectsForShapes(shapes, bufferObjects);
}

// virtual (from ShapeDrawer)
auto ShapeBufferObjectDrawer::beginRenderBatch()
    -> void
{
}

auto ShapeBufferObjectDrawer::render(Shape const & shape)
    -> void
{
    auto const & shapeBuffers = getBufferObjectsForShape(shape);

    auto const vertexBinder = glow::bind(shapeBuffers.vertex);

    auto const elementBinder = glow::bind(shapeBuffers.element);

    glow::sendVertexLayoutToGpu<ShapeVertex>();

    auto const numOfIndices = getNumOfIndices(shape);

    glDrawElements(GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, 0);
}

// virtual (from ShapeDrawer)
auto ShapeBufferObjectDrawer::endRenderBatch() 
    -> void
{
}

auto ShapeBufferObjectDrawer::setupBufferObjectsForShapes(
    std::vector<Shape *> const & shapes,
    std::vector<BufferObjectSet> & destination) const
    -> void
{
    for (auto const shape : shapes)
    {
        auto const id = shape->getInstanceIndex();

        auto & shapeBuffers = destination[id];

        setupBufferObjectsForShape(*shape, shapeBuffers);
    }
}

auto ShapeBufferObjectDrawer::setupBufferObjectsForShape(
    Shape const & shape,
    BufferObjectSet & destination) const
    -> void
{
    auto const creator = ShapeBufferObjectCreator{};

    auto shapeBuffers = creator.makeBuffers(shape);
    
    destination.vertex = std::move(shapeBuffers.vertex);
    
    destination.element = std::move(shapeBuffers.element);
}

auto ShapeBufferObjectDrawer::getBufferObjectsForShape(Shape const & shape) const
    -> const BufferObjectSet &
{
    auto const shapeIndex = shape.getInstanceIndex();

    return bufferObjects[shapeIndex];
}

} // namespace ape
