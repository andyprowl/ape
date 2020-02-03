#include <Glow/BufferObject/BufferObject.hpp>

#include <Glow/BufferObject/BufferStorageFlags.hpp>

#include <glad/glad.h>

#include <cassert>

namespace glow
{

auto BufferObject::createStorage(
    void const * data,
    std::size_t const sizeInBytes,
    BufferStorageFlags const flags)
    -> void
{
    auto const id = getId();

    auto const openGLFlags = convertToOpenGLFlags(flags);

    glNamedBufferStorage(id, sizeInBytes, data, openGLFlags);

    assert(glGetError() == GL_NO_ERROR);
}

} // namespace glow
