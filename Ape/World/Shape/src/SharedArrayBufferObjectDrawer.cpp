#include <Ape/World/Shape/SharedArrayBufferObjectDrawer.hpp>

#include <Ape/World/Shape/Shape.hpp>
#include <Ape/World/Shape/ShapeBufferObjectCreator.hpp>

#include <Glow/BufferObject/VertexLayout.hpp>
#include <Glow/GpuResource/ScopedBinder.hpp>

#include <Basix/Range/Insertion.hpp>

#include <glad/glad.h>

#include <algorithm>

namespace ape
{

namespace
{

auto makeVertexBufferObject(std::vector<ShapeVertex> const & vertices)
    -> glow::VertexBufferObject
{
    auto vbo = glow::VertexBufferObject{};

    auto const vertexData = static_cast<void const *>(vertices.data());

    auto const vertexBufferSize = vertices.size() * sizeof(ShapeVertex);

    vbo.createStorage(vertexData, vertexBufferSize);

    return vbo;
}

auto makeIndexBufferObject(std::vector<unsigned int> const & indices)
    -> glow::ElementBufferObject
{
    auto ebo = glow::ElementBufferObject{};

    auto const indexData = static_cast<void const *>(indices.data());

    auto const indexBufferSize = indices.size() * sizeof(unsigned int);

    ebo.createStorage(indexData, indexBufferSize);

    return ebo;
}

} // unnamed namespace

SharedArrayBufferObjectDrawer::SharedArrayBufferObjectDrawer(std::vector<Shape *> const & shapes)
    : SharedArrayBufferObjectDrawer{mergeShapeGeometry(shapes)}
{
}

// virtual (from ShapeDrawer)
auto SharedArrayBufferObjectDrawer::beginRenderBatch()
    -> void
{
    bufferObjects.array.bind();
}

// virtual (from ShapeDrawer)
auto SharedArrayBufferObjectDrawer::render(Shape const & shape)
    -> void
{
    auto const & offsets = getBufferOffsetsForShape(shape);
    
    auto const numOfIndices = getNumOfIndices(shape);

    auto const indexBufferOffset = offsets.elementBufferOffset * sizeof(unsigned int);

    glDrawElementsBaseVertex(
        GL_TRIANGLES,
        numOfIndices,
        GL_UNSIGNED_INT,
        reinterpret_cast<void *>(static_cast<std::uintptr_t>(indexBufferOffset)),
        offsets.vertexBufferOffset);
}

// virtual (from ShapeDrawer)
auto SharedArrayBufferObjectDrawer::endRenderBatch()
    -> void
{
    bufferObjects.array.unbind();
}

SharedArrayBufferObjectDrawer::SharedArrayBufferObjectDrawer(ShapeGeometry geometry)
    : bufferObjects{
        glow::VertexArrayObject{},
        makeVertexBufferObject(geometry.vertices),
        makeIndexBufferObject(geometry.indices)}
    , bufferOffsets{std::move(geometry.offsets)}
{
    setupVertexFormat();
}

auto SharedArrayBufferObjectDrawer::getBufferOffsetsForShape(Shape const & shape) const
    -> const ShapeBufferOffset &
{
    auto const shapeIndex = shape.getInstanceIndex();

    return bufferOffsets[shapeIndex];
}

auto SharedArrayBufferObjectDrawer::mergeShapeGeometry(std::vector<Shape *> const & shapes) const
    -> ShapeGeometry
{
    auto geometry = ShapeGeometry{};

    for (auto const shape : shapes)
    {
        geometry.offsets.emplace_back(
            static_cast<int>(geometry.vertices.size()),
            static_cast<int>(geometry.indices.size()));

        basix::append(geometry.vertices, shape->getVertices());

        basix::append(geometry.indices, shape->getIndices());
    }

    return geometry;
}

auto SharedArrayBufferObjectDrawer::setupVertexFormat()
    -> void
{
    auto const layout = glow::getVertexLayout<ShapeVertex>();

    auto const & attributes = layout.getAttributes();

    for (auto i = 0; i < static_cast<int>(attributes.size()); ++i)
    {
        auto binding = bufferObjects.array.getBinding(i);

        binding.setAttributeIndex(i);

        binding.setFormat(attributes[i]);
        
        binding.setVertexSource(bufferObjects.vertex, 0, sizeof(ShapeVertex));

        bufferObjects.array.enableAttribute(i);
    }

    bufferObjects.array.setIndexSource(bufferObjects.element);
}

} // namespace ape
