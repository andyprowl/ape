#include <Ape/World/Shape/ShapeArrayObjectDrawer.hpp>

#include <Ape/World/Shape/Shape.hpp>

#include <Glow/BufferObject/VertexLayout.hpp>
#include <Glow/GpuResource/ScopedBinder.hpp>

#include <Basix/Range/Insertion.hpp>

#include <glad/glad.h>

#include <algorithm>

namespace ape
{

namespace
{

auto setupRenderingState(Shape const & shape)
    -> void
{
    shape.getVertexBufferObject().bind();

    shape.getElementBufferObject().bind();

    glow::sendVertexLayoutToGpu<ShapeVertex>();
}

} // unnamed namespace

ShapeArrayObjectDrawer::ShapeArrayObjectDrawer(std::vector<Shape *> const & shapes)
{
    registerShapes(shapes);
}

auto ShapeArrayObjectDrawer::registerShapes(std::vector<Shape *> const & shapes)
    -> void
{
    // This design is weak and error-prone, as it relies on consistent order of insertion and
    // creation of shapes. It should be changed to something more robust.

    shapeArrayObjects.resize(shapeArrayObjects.size() + shapes.size());

    setupArrayObjectsForShapes(shapes, shapeArrayObjects);
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

    arrayObject.bind();

    auto const numOfVertices = shape.getNumOfVertices();

    glDrawElements(GL_TRIANGLES, numOfVertices, GL_UNSIGNED_INT, 0);

    arrayObject.unbind();
}

// virtual (from ShapeDrawer)
auto ShapeArrayObjectDrawer::endRenderBatch()
    -> void
{
}

auto ShapeArrayObjectDrawer::setupArrayObjectsForShapes(
    std::vector<Shape *> const & shapes,
    std::vector<glow::VertexArrayObject> & destination) const
    -> void
{
    for (auto const shape : shapes)
    {
        auto const id = shape->getInstanceIndex();

        auto & arrayObject = destination[id];

        setupArrayObjectForShape(*shape, arrayObject);
    }
}

auto ShapeArrayObjectDrawer::setupArrayObjectForShape(
    Shape const & shape,
    glow::VertexArrayObject & destination) const
    -> void
{
    auto const binder = glow::bind(destination);

    setupRenderingState(shape);
}

auto ShapeArrayObjectDrawer::getArrayObjectForShape(Shape const & shape) const
    -> const glow::VertexArrayObject &
{
    auto const shapeIndex = shape.getInstanceIndex();

    return shapeArrayObjects[shapeIndex];
}

} // namespace ape
