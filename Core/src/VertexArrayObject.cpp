#include <Core/VertexArrayObject.hpp>

#include <glad/glad.h>

namespace ape
{

namespace
{

auto generateVertexArrayId()
    -> int
{
    auto id = 0u;

    glGenVertexArrays(1, &id);

    return id;
}

} // unnamed namespace

VertexArrayObject::VertexArrayObject()
    : BufferObject{generateVertexArrayId()}
{
}

auto VertexArrayObject::bind() const
    -> void
{
    glBindVertexArray(id);
}

auto VertexArrayObject::unbind() const
    -> void
{
    glBindVertexArray(0);
}

} // namespace ape
