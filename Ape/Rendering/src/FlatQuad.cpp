#include <Ape/Rendering/FlatQuad.hpp>

#include <Ape/Rendering/FlatQuadVertex.hpp>

#include <Ape/GpuResource/VertexArrayObject.hpp>
#include <Ape/GpuResource/VertexLayout.hpp>

#include <glad/glad.h>

namespace ape
{

namespace
{

auto makeVertexBufferObject(std::vector<FlatQuadVertex> const & vertices)
    -> VertexBufferObject
{
    auto vbo = VertexBufferObject{};

    vbo.bind();

    auto const vertexBufferSize = vertices.size() * sizeof(FlatQuadVertex);

    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertices.data(), GL_STATIC_DRAW);

    sendVertexLayoutToGpu<FlatQuadVertex>();

    return vbo;
}

auto makeVertexIndexBufferObject(std::vector<unsigned int> const & indices)
    -> ElementBufferObject
{
    auto ebo = ElementBufferObject{};

    ebo.bind();

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(unsigned int),
        indices.data(),
        GL_STATIC_DRAW);

    return ebo;
}

} // unnamed namespace

FlatQuad::FlatQuad()
    : vertexBuffer{makeVertices()}
{
}

auto FlatQuad::getVertexBufferObject() const 
    -> const VertexBufferObject &
{
    return vertexBuffer;
}

auto FlatQuad::getNumOfVertices() const
    -> int
{
    return 6;
}

auto FlatQuad::makeVertices() const
    -> VertexBufferObject
{
    auto vao = VertexArrayObject{};

    vao.bind();

    auto const vertices = std::vector<FlatQuadVertex>{
        {{-1.0f, +1.0f}, {+0.0f, +1.0f}},
        {{-1.0f, -1.0f}, {+0.0f, +0.0f}},
        {{+1.0f, -1.0f}, {+1.0f, +0.0f}},
        {{-1.0f, +1.0f}, {+0.0f, +1.0f}},
        {{+1.0f, -1.0f}, {+1.0f, +0.0f}},
        {{+1.0f, +1.0f}, {+1.0f, +1.0f}}};

    auto vbo = makeVertexBufferObject(vertices);

    vao.unbind();

    return vbo;
}

} // namespace ape
