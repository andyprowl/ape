#include <Glow/BufferObject/UniformBufferObject.hpp>

#include <glad/glad.h>

#include <cassert>

namespace glow
{

namespace
{

auto createUniformBufferResource()
    -> GpuResource
{
    auto id = GpuResource::Id{};

    glCreateBuffers(1, &id);

    return GpuResource{id, [] (GpuResource::Id const id) { glDeleteBuffers(1, &id); }};
}

} // unnamed namespace

UniformBufferObject::UniformBufferObject()
    : BufferObject{createUniformBufferResource()}
{
}

auto UniformBufferObject::bind(int const bindingPoint) const
    -> void
{
    assert(isValid());

    auto const id = resource.get();

    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, id);

    assert(glGetError() == GL_NO_ERROR);
}

auto UniformBufferObject::bind(int const bindingPoint, int const offset, int const size) const
    -> void
{
    assert(isValid());

    auto const id = resource.get();

    glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, id, offset, size);

    assert(glGetError() == GL_NO_ERROR);
}

} // namespace glow
