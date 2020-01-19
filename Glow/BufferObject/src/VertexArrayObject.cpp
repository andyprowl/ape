#include <Glow/BufferObject/VertexArrayObject.hpp>

#include <glad/glad.h>

#include <cassert>

namespace glow
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

auto VertexArrayObject::getBinding(int const bindingIndex) const
    -> VertexArrayAttributeBinding
{
    return {*this, bindingIndex};
}

} // namespace glow
