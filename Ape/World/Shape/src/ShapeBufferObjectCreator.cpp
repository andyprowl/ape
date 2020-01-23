#include <Ape/World/Shape/ShapeBufferObjectCreator.hpp>

#include <Ape/World/Shape/Shape.hpp>

#include <glad/glad.h>

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

auto ShapeBufferObjectCreator::makeBuffers(Shape const & shape) const
    -> BufferObjectSet
{
    auto vbo = makeVertexBufferObject(shape.getVertices());

    auto ebo = makeIndexBufferObject(shape.getIndices());

    return {std::move(vbo), std::move(ebo)};
}

} // namespace ape
