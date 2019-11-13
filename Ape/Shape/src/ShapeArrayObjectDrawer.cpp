#include <Ape/Shape/ShapeArrayObjectDrawer.hpp>

#include <Ape/Shape/Shape.hpp>

#include <Ape/BufferObject/VertexLayout.hpp>
#include <Ape/GpuResource/ScopedBinder.hpp>

#include <Foundational/Range/Insertion.hpp>

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

    sendVertexLayoutToGpu<ShapeVertex>();
}

} // unnamed namespace

ShapeArrayObjectDrawer::ShapeArrayObjectDrawer(ShapeCollection const & shapes)
{
    registerShapes(shapes);
}

auto ShapeArrayObjectDrawer::registerShapes(ShapeCollection const & shapes)
    -> void
{
    auto const lastShapeIndex = shapes.back()->getInstanceIndex();

    shapeArrayObjects.resize(shapeArrayObjects.size() + shapes.size());

    setupArrayObjectsForShapes(shapes, shapeArrayObjects);
}

// virtual (from ShapeDrawer)
auto ShapeArrayObjectDrawer::render(Shape const & shape) const
    -> void
{
    auto & arrayObject = getArrayObjectForShape(shape);

    arrayObject.bind();

    auto const numOfVertices = shape.getNumOfVertices();

    glDrawElements(GL_TRIANGLES, numOfVertices, GL_UNSIGNED_INT, 0);

    arrayObject.unbind();
}

auto ShapeArrayObjectDrawer::setupArrayObjectsForShapes(
    ShapeCollection const & shapes,
    std::vector<VertexArrayObject> & destination) const
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
    VertexArrayObject & destination) const
    -> void
{
    auto const binder = bind(destination);

    setupRenderingState(shape);
}

auto ShapeArrayObjectDrawer::getArrayObjectForShape(Shape const & shape) const
    -> const VertexArrayObject &
{
    auto const shapeIndex = shape.getInstanceIndex();

    return shapeArrayObjects[shapeIndex];
}

} // namespace ape
