#include <GpuResource/ElementBufferObject.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
{

namespace
{

auto createElementBufferResource()
    -> GpuResource
{
    auto id = 0u;

    glGenBuffers(1, &id);

    return GpuResource{id, [] (unsigned int const id) { glDeleteBuffers(1, &id); }};
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
