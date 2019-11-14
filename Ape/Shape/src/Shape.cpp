#include <Ape/Shape/Shape.hpp>

#include <Glow/BufferObject/VertexArrayObject.hpp>
#include <Glow/BufferObject/VertexLayout.hpp>

#include <glad/glad.h>

namespace ape
{

namespace
{

auto makeVertexBufferObject(std::vector<ShapeVertex> const & vertices)
    -> glow::VertexBufferObject
{
    auto vbo = glow::VertexBufferObject{};

    vbo.bind();

    auto const vertexBufferSize = vertices.size() * sizeof(ShapeVertex);

    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertices.data(), GL_STATIC_DRAW);

    glow::sendVertexLayoutToGpu<ShapeVertex>();

    return vbo;
}

auto makeVertexIndexBufferObject(std::vector<unsigned int> const & indices)
    -> glow::ElementBufferObject
{
    auto ebo = glow::ElementBufferObject{};

    ebo.bind();

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(),
        GL_STATIC_DRAW);

    return ebo;
}

} // unnamed namespace

Shape::Shape(std::vector<ShapeVertex> const & vertices, std::vector<unsigned int> const & indices)
    : bufferObjects{makeVertices(vertices, indices)}
    , numOfVertices{static_cast<int>(indices.size())}
{
}

auto Shape::getVertexBufferObject() const 
    -> const glow::VertexBufferObject &
{
    return bufferObjects.vertex;
}

auto Shape::getElementBufferObject() const 
    -> const glow::ElementBufferObject &
{
    return bufferObjects.element;
}

auto Shape::getNumOfVertices() const
    -> int
{
    return numOfVertices;
}

auto Shape::makeVertices(
    std::vector<ShapeVertex> const & vertices,
    std::vector<unsigned int> const & indices) const
    -> BufferObjects
{
    auto vao = glow::VertexArrayObject{};

    vao.bind();

    auto vbo = makeVertexBufferObject(vertices);

    auto ebo = makeVertexIndexBufferObject(indices);

    vao.unbind();

    return {std::move(vbo), std::move(ebo)};
}

} // namespace ape
