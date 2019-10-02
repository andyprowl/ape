#include <Glow/BufferObject.hpp>

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
    destroy();

    id = rhs.id;

    rhs.id = 0;

    return *this;
}

BufferObject::~BufferObject()
{
    destroy();
}

auto BufferObject::destroy()
    -> void
{
    glDeleteVertexArrays(1, &id);
}

} // namespace ape
