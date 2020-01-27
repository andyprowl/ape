#include <Glow/BufferObject/BufferStorageFlags.hpp>

#include <glad/glad.h>

#include <array>

namespace glow
{

namespace
{

constexpr auto const storageFlags = std::array<GLenum, 7u>{{
    0,
    GL_DYNAMIC_STORAGE_BIT,
    GL_MAP_READ_BIT,
    GL_MAP_WRITE_BIT,
    GL_MAP_PERSISTENT_BIT,
    GL_MAP_COHERENT_BIT,
    GL_CLIENT_STORAGE_BIT}};

} // unnamed namespace

auto convertToOpenGLFlags(BufferStorageFlags const flags)
    -> GLenum
{
    return 
        storageFlags[static_cast<std::size_t>(flags & BufferStorageFlags::dynamicStorage)] |
        storageFlags[static_cast<std::size_t>(flags & BufferStorageFlags::mapRead)] |
        storageFlags[static_cast<std::size_t>(flags & BufferStorageFlags::mapWrite)] |
        storageFlags[static_cast<std::size_t>(flags & BufferStorageFlags::mapPersistent)] |
        storageFlags[static_cast<std::size_t>(flags & BufferStorageFlags::mapCoherent)] |
        storageFlags[static_cast<std::size_t>(flags & BufferStorageFlags::clientStorage)];
}

} // namespace glow
