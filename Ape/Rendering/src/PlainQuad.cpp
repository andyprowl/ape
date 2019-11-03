#include <Ape/Rendering/PlainQuad.hpp>

#include <Ape/Rendering/PlainQuadVertex.hpp>

#include <Ape/GpuResource/ScopedBinder.hpp>
#include <Ape/GpuResource/VertexArrayObject.hpp>
#include <Ape/GpuResource/VertexLayout.hpp>

#include <glad/glad.h>

namespace ape
{

namespace
{

auto makeVertexBufferObject(std::vector<PlainQuadVertex> const & vertices)
    -> VertexBufferObject
{
    auto vbo = VertexBufferObject{};

    auto const binder = bind(vbo);

    auto const vertexBufferSize = vertices.size() * sizeof(PlainQuadVertex);

    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertices.data(), GL_STATIC_DRAW);

    sendVertexLayoutToGpu<PlainQuadVertex>();

    return vbo;
}

auto makeArrayObject(VertexBufferObject const & vertexBuffer)
    -> VertexArrayObject
{
    auto arrayObject = VertexArrayObject{};

    auto const binder = bind(arrayObject);

    vertexBuffer.bind();

    sendVertexLayoutToGpu<PlainQuadVertex>();

    return arrayObject;
}

} // unnamed namespace

PlainQuad::PlainQuad()
    : vertexBuffer{makeVertices()}
    , arrayObject{makeArrayObject(vertexBuffer)}
{
}

auto PlainQuad::getVertexBuffer() const 
    -> const VertexBufferObject &
{
    return vertexBuffer;
}

auto PlainQuad::getArrayObject() const 
    -> const VertexArrayObject &
{
    return arrayObject;
}

auto PlainQuad::getNumOfVertices() const
    -> int
{
    return 6;
}

auto PlainQuad::makeVertices() const
    -> VertexBufferObject
{
    auto vao = VertexArrayObject{};

    auto const binder = bind(vao);

    auto const vertices = std::vector<PlainQuadVertex>{
        {{-1.0f, +1.0f}, {+0.0f, +1.0f}},
        {{-1.0f, -1.0f}, {+0.0f, +0.0f}},
        {{+1.0f, -1.0f}, {+1.0f, +0.0f}},
        {{-1.0f, +1.0f}, {+0.0f, +1.0f}},
        {{+1.0f, -1.0f}, {+1.0f, +0.0f}},
        {{+1.0f, +1.0f}, {+1.0f, +1.0f}}};

    return makeVertexBufferObject(vertices);
}

} // namespace ape
