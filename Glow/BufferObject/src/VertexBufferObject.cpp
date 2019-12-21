#include <Glow/BufferObject/VertexBufferObject.hpp>

#include <glad/glad.h>

#include <cassert>

namespace glow
{

namespace
{

auto createVertexBufferResource()
    -> GpuResource
{
    auto id = GpuResource::Id{};

    glCreateBuffers(1, &id);

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

} // namespace glow
