#include <GpuResource/VertexBufferObject.hpp>

#include <glad/glad.h>

namespace ape
{

namespace
{

auto createVertexBufferResource()
    -> GpuResource
{
    auto id = 0u;

    glGenBuffers(1, &id);

    return GpuResource{id, [] (unsigned int const id) { glDeleteBuffers(1, &id); }};
}

} // unnamed namespace

VertexBufferObject::VertexBufferObject()
    : BufferObject{createVertexBufferResource()}
{
}

auto VertexBufferObject::bind() const
    -> void
{
    auto const id = resource.get();

    glBindBuffer(GL_ARRAY_BUFFER, id);
}

auto VertexBufferObject::unbind() const
    -> void
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace ape
