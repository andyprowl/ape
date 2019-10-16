#include <Ape/GpuResource/VertexArrayObject.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
{

namespace
{

auto createArrayObjectResource()
    -> GpuResource
{
    auto id = GpuResource::Id{};

    glGenVertexArrays(1, &id);

    return GpuResource{id, [] (GpuResource::Id const id) { glDeleteVertexArrays(1, &id); }};
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
