#include <Core/VertexBufferObject.hpp>

#include <glad/glad.h>

namespace ape
{

namespace
{

auto generateVertexBufferId()
    -> int
{
    auto id = 0u;

    glGenBuffers(1, &id);

    return id;
}

} // unnamed namespace

VertexBufferObject::VertexBufferObject()
    : BufferObject{generateVertexBufferId()}
{
}

auto VertexBufferObject::bind() const
    -> void
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

auto VertexBufferObject::unbind() const
    -> void
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace ape
