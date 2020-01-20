#include <Ape/Rendering/Effect/PlainQuad.hpp>

#include <Ape/Rendering/Effect/PlainQuadVertex.hpp>

#include <Glow/BufferObject/VertexArrayObject.hpp>
#include <Glow/BufferObject/VertexLayout.hpp>
#include <Glow/GpuResource/ScopedBinder.hpp>

#include <glad/glad.h>

namespace ape
{

namespace
{

auto makeVertexBufferObject(std::vector<PlainQuadVertex> const & vertices)
    -> glow::VertexBufferObject
{
    auto vbo = glow::VertexBufferObject{};

    auto const binder = glow::bind(vbo);

    auto const vertexBufferSize = vertices.size() * sizeof(PlainQuadVertex);

    glBufferStorage(GL_ARRAY_BUFFER, vertexBufferSize, vertices.data(), 0);

    glow::sendVertexLayoutToGpu<PlainQuadVertex>();

    return vbo;
}

auto makeArrayObject(glow::VertexBufferObject const & vertexBuffer)
    -> glow::VertexArrayObject
{
    auto arrayObject = glow::VertexArrayObject{};

    auto const binder = glow::bind(arrayObject);

    vertexBuffer.bind();

    glow::sendVertexLayoutToGpu<PlainQuadVertex>();

    return arrayObject;
}

} // unnamed namespace

PlainQuad::PlainQuad()
    : vertexBuffer{makeVertices()}
    , arrayObject{makeArrayObject(vertexBuffer)}
{
}

auto PlainQuad::getVertexBuffer() const 
    -> const glow::VertexBufferObject &
{
    return vertexBuffer;
}

auto PlainQuad::getArrayObject() const 
    -> const glow::VertexArrayObject &
{
    return arrayObject;
}

auto PlainQuad::getNumOfVertices() const
    -> int
{
    return 6;
}

auto PlainQuad::makeVertices() const
    -> glow::VertexBufferObject
{
    auto vao = glow::VertexArrayObject{};

    auto const binder = glow::bind(vao);

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
