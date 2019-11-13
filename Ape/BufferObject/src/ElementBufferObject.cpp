#include <Ape/BufferObject/ElementBufferObject.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
{

namespace
{

auto createElementBufferResource()
    -> GpuResource
{
    auto id = GpuResource::Id{};

    glGenBuffers(1, &id);

    return GpuResource{id, [] (GpuResource::Id const id) { glDeleteBuffers(1, &id); }};
}

} // unnamed namespace

ElementBufferObject::ElementBufferObject()
    : BufferObject{createElementBufferResource()}
{
}

auto ElementBufferObject::bind() const
    -> void
{
    assert(isValid());

    auto const id = resource.get();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

auto ElementBufferObject::unbind() const
    -> void
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace ape
