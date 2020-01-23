#include <Ape/World/Shape/ShapeArrayObjectDrawer.hpp>

#include <Ape/World/Shape/Shape.hpp>
#include <Ape/World/Shape/ShapeBufferObjectCreator.hpp>

#include <Glow/BufferObject/VertexLayout.hpp>
#include <Glow/GpuResource/ScopedBinder.hpp>

#include <Basix/Range/Insertion.hpp>

#include <glad/glad.h>

#include <algorithm>

namespace ape
{

ShapeArrayObjectDrawer::ShapeArrayObjectDrawer(std::vector<Shape *> const & shapes)
{
    // This design is weak and error-prone, as it relies on consistent order of insertion and
    // creation of shapes. It should be changed to something more robust.

    bufferObjects.resize(shapes.size());

    setupBufferObjectsForShapes(shapes, bufferObjects);
}

// virtual (from ShapeDrawer)
auto ShapeArrayObjectDrawer::beginRenderBatch()
    -> void
{
}

// virtual (from ShapeDrawer)
auto ShapeArrayObjectDrawer::render(Shape const & shape)
    -> void
{
    auto & arrayObject = getArrayObjectForShape(shape);

    auto const binder = glow::bind(arrayObject);

    auto const numOfIndices = getNumOfIndices(shape);

    glDrawElements(GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, 0);
}

// virtual (from ShapeDrawer)
auto ShapeArrayObjectDrawer::endRenderBatch()
    -> void
{
}

auto ShapeArrayObjectDrawer::setupBufferObjectsForShapes(
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

auto ShapeArrayObjectDrawer::setupBufferObjectsForShape(
    Shape const & shape,
    BufferObjectSet & destination) const
    -> void
{
    destination.array.bind();

    setupVertexAndElementBuffersForShape(shape, destination);

    setupRenderingState(destination);

    destination.array.unbind();
}

auto ShapeArrayObjectDrawer::setupVertexAndElementBuffersForShape(
    Shape const & shape,
    BufferObjectSet & destination) const
    -> void
{
    auto const creator = ShapeBufferObjectCreator{};

    auto shapeBuffers = creator.makeBuffers(shape);
    
    destination.vertex = std::move(shapeBuffers.vertex);
    
    destination.element = std::move(shapeBuffers.element);
}

auto ShapeArrayObjectDrawer::setupRenderingState(BufferObjectSet & destination) const
    -> void
{
    auto const vertexBinder = glow::bind(destination.vertex);

    auto const elementBinder = glow::bind(destination.element);

    glow::sendVertexLayoutToGpu<ShapeVertex>();
}

auto ShapeArrayObjectDrawer::getArrayObjectForShape(Shape const & shape) const
    -> const glow::VertexArrayObject &
{
    auto const shapeIndex = shape.getInstanceIndex();

    return bufferObjects[shapeIndex].array;
}

} // namespace ape
