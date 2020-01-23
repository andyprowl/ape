#include <Ape/World/Shape/SharedArrayObjectDrawer.hpp>

#include <Ape/World/Shape/Shape.hpp>
#include <Ape/World/Shape/ShapeBufferObjectCreator.hpp>

#include <Glow/BufferObject/VertexLayout.hpp>
#include <Glow/GpuResource/ScopedBinder.hpp>

#include <glad/glad.h>

namespace ape
{

SharedArrayObjectDrawer::SharedArrayObjectDrawer(std::vector<Shape *> const & shapes)
{
    // This design is weak and error-prone, as it relies on consistent order of insertion and
    // creation of shapes. It should be changed to something more robust.

    bufferObjects.resize(shapes.size());

    setupBufferObjectsForShapes(shapes, bufferObjects);

    setupVertexFormat();
}

// virtual (from ShapeDrawer)
auto SharedArrayObjectDrawer::beginRenderBatch()
    -> void
{
    arrayObject.bind();
}

auto SharedArrayObjectDrawer::render(Shape const & shape)
    -> void
{
    auto const & shapeBuffers = getBufferObjectsForShape(shape);

    for (auto i = 0; i < ShapeVertex::numOfComponents; ++i)
    {
        auto binding = arrayObject.getBinding(i);
        
        binding.setVertexSource(shapeBuffers.vertex, 0, sizeof(ShapeVertex));
    }

    arrayObject.setIndexSource(shapeBuffers.element);

    auto const numOfIndices = getNumOfIndices(shape);

    glDrawElements(GL_TRIANGLES, numOfIndices, GL_UNSIGNED_INT, 0);
}

// virtual (from ShapeDrawer)
auto SharedArrayObjectDrawer::endRenderBatch()
    -> void
{
    arrayObject.unbind();
}

auto SharedArrayObjectDrawer::setupBufferObjectsForShapes(
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

auto SharedArrayObjectDrawer::setupBufferObjectsForShape(
    Shape const & shape,
    BufferObjectSet & destination) const
    -> void
{
    auto const creator = ShapeBufferObjectCreator{};

    auto shapeBuffers = creator.makeBuffers(shape);
    
    destination.vertex = std::move(shapeBuffers.vertex);
    
    destination.element = std::move(shapeBuffers.element);
}

auto SharedArrayObjectDrawer::getBufferObjectsForShape(Shape const & shape) const
    -> const BufferObjectSet &
{
    auto const shapeIndex = shape.getInstanceIndex();

    return bufferObjects[shapeIndex];
}

auto SharedArrayObjectDrawer::setupVertexFormat()
    -> void
{
    auto const layout = glow::getVertexLayout<ShapeVertex>();

    auto const & attributes = layout.getAttributes();

    for (auto i = 0; i < static_cast<int>(attributes.size()); ++i)
    {
        auto binding = arrayObject.getBinding(i);

        binding.setAttributeIndex(i);

        binding.setFormat(attributes[i]);

        arrayObject.enableAttribute(i);
    }
}

} // namespace ape
