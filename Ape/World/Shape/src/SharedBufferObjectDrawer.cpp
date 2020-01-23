#include <Ape/World/Shape/SharedBufferObjectDrawer.hpp>

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

    auto const vertexBufferSize = vertices.size() * sizeof(ShapeVertex);

    glNamedBufferStorage(vbo.getId(), vertexBufferSize, vertices.data(), 0);

    return vbo;
}

auto makeIndexBufferObject(std::vector<unsigned int> const & indices)
    -> glow::ElementBufferObject
{
    auto ebo = glow::ElementBufferObject{};

    auto const indexBufferSize = indices.size() * sizeof(unsigned int);

    glNamedBufferStorage(ebo.getId(), indexBufferSize, indices.data(), 0);

    return ebo;
}

} // unnamed namespace

SharedBufferObjectDrawer::SharedBufferObjectDrawer(std::vector<Shape *> const & shapes)
    : SharedBufferObjectDrawer{mergeShapeGeometry(shapes)}
{
}

// virtual (from ShapeDrawer)
auto SharedBufferObjectDrawer::beginRenderBatch()
    -> void
{
    bufferObjects.vertex.bind();

    bufferObjects.element.bind();

    glow::sendVertexLayoutToGpu<ShapeVertex>();
}

// virtual (from ShapeDrawer)
auto SharedBufferObjectDrawer::render(Shape const & shape)
    -> void
{
    auto const & offsets = getBufferOffsetsForShape(shape);
    
    auto const numOfIndices = getNumOfIndices(shape);

    glDrawElementsBaseVertex(
        GL_TRIANGLES,
        numOfIndices,
        GL_UNSIGNED_INT,
        reinterpret_cast<void *>(static_cast<std::uintptr_t>(offsets.elementBufferOffset)),
        offsets.vertexBufferOffset);
}

// virtual (from ShapeDrawer)
auto SharedBufferObjectDrawer::endRenderBatch()
    -> void
{
    bufferObjects.element.unbind();

    bufferObjects.vertex.unbind();
}

SharedBufferObjectDrawer::SharedBufferObjectDrawer(ShapeGeometry geometry)
    : bufferObjects{
        makeVertexBufferObject(geometry.vertices),
        makeIndexBufferObject(geometry.indices)}
    , bufferOffsets{std::move(geometry.offsets)}
{
}

auto SharedBufferObjectDrawer::getBufferOffsetsForShape(Shape const & shape) const
    -> const ShapeBufferOffset &
{
    auto const shapeIndex = shape.getInstanceIndex();

    return bufferOffsets[shapeIndex];
}

auto SharedBufferObjectDrawer::mergeShapeGeometry(std::vector<Shape *> const & shapes) const
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

} // namespace ape
