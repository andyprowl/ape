#include <GpuResource/VertexArrayObject.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
{

namespace
{

auto createArrayObjectResource()
    -> GpuResource
{
    auto id = 0u;

    glGenVertexArrays(1, &id);

    return GpuResource{id, [] (unsigned int const id) { glDeleteVertexArrays(1, &id); }};
}

} // unnamed namespace

VertexArrayObject::VertexArrayObject()
    : BufferObject{createArrayObjectResource()}
{
}

auto VertexArrayObject::bind() const
    -> void
{
    assert(isValid());

    auto const id = resource.get();

    glBindVertexArray(id);
}

auto VertexArrayObject::unbind() const
    -> void
{
    glBindVertexArray(0);
}

} // namespace ape
