#include <Glow/BufferObject/VertexArrayObject.hpp>

#include <Glow/BufferObject/ElementBufferObject.hpp>

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

    glCreateVertexArrays(1, &id);

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

auto VertexArrayObject::setIndexSource(ElementBufferObject const & buffer)
    -> void
{
    auto const id = resource.get();

    glVertexArrayElementBuffer(id, buffer.getId());
}

auto VertexArrayObject::enableAttribute(int const attributeIndex)
    -> void
{
    auto const id = resource.get();

    glEnableVertexArrayAttrib(id, attributeIndex);
}

auto VertexArrayObject::disableAttribute(int const attributeIndex)
    -> void
{
    auto const id = resource.get();

    glDisableVertexArrayAttrib(id, attributeIndex);
}

} // namespace glow
