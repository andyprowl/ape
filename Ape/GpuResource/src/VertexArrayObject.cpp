#include <GpuResource/VertexArrayObject.hpp>

#include <glad/glad.h>

namespace ape
{

namespace
{

auto createVertexBufferResource()
    -> GpuResource
{
    auto id = 0u;

    glGenVertexArrays(1, &id);

    return GpuResource{id, [] (unsigned int const id) { glDeleteVertexArrays(1, &id); }};
}

} // unnamed namespace

VertexArrayObject::VertexArrayObject()
    : BufferObject{createVertexBufferResource()}
{
}

auto VertexArrayObject::bind() const
    -> void
{
    auto const id = resource.get();

    glBindVertexArray(id);
}

auto VertexArrayObject::unbind() const
    -> void
{
    glBindVertexArray(0);
}

} // namespace ape
