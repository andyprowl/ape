#include <Ape/ElementBufferObject.hpp>

#include <glad/glad.h>

namespace ape
{

namespace
{

auto generateElementBufferId()
    -> int
{
    auto id = 0u;

    glGenBuffers(1, &id);

    return id;
}

} // unnamed namespace

ElementBufferObject::ElementBufferObject()
    : BufferObject{generateElementBufferId()}
{
}

auto ElementBufferObject::bind() const
    -> void
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

auto ElementBufferObject::unbind() const
    -> void
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace ape
