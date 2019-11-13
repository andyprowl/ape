#include <Ape/BufferObject/VertexBufferObject.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
{

namespace
{

auto createVertexBufferResource()
    -> GpuResource
{
    auto id = GpuResource::Id{};

    glGenBuffers(1, &id);

    return GpuResource{id, [] (GpuResource::Id const id) { glDeleteBuffers(1, &id); }};
}

} // unnamed namespace

VertexBufferObject::VertexBufferObject()
    : BufferObject{createVertexBufferResource()}
{
}

auto VertexBufferObject::bind() const
    -> void
{
    assert(isValid());

    auto const id = resource.get();

    glBindBuffer(GL_ARRAY_BUFFER, id);
}

auto VertexBufferObject::unbind() const
    -> void
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // namespace ape
