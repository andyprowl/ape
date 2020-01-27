#include <Glow/BufferObject/BufferObject.hpp>

#include <Glow/BufferObject/BufferStorageFlags.hpp>

#include <glad/glad.h>

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
}

} // namespace glow
