#include <Core/BufferObject.hpp>

#include <glad/glad.h>

namespace ape
{

BufferObject::BufferObject(unsigned int const id)
    : id{id}
{
}

BufferObject::BufferObject(BufferObject && rhs) noexcept
    : id{rhs.id}
{
    rhs.id = 0;
}

auto BufferObject::operator = (BufferObject && rhs) noexcept
    -> BufferObject &
{
    id = rhs.id;

    rhs.id = 0;

    return *this;
}

BufferObject::~BufferObject()
{
    glDeleteVertexArrays(1, &id);
}

} // namespace ape
