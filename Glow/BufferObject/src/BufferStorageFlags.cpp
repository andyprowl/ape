#include <Glow/BufferObject/BufferStorageFlags.hpp>

#include <glad/glad.h>

#include <array>

namespace glow
{

namespace
{

constexpr auto const storageFlags = std::array<GLenum, 6u>{{
    GL_DYNAMIC_STORAGE_BIT,
    GL_MAP_READ_BIT,
    GL_MAP_WRITE_BIT,
    GL_MAP_PERSISTENT_BIT,
    GL_MAP_COHERENT_BIT,
    GL_CLIENT_STORAGE_BIT}};

} // unnamed namespace

auto isFlagSet(BufferStorageFlags const flag, BufferStorageFlags const flagSet)
    -> bool
{
    return ((flag & flagSet) == flag);
}

auto convertToOpenGLFlags(BufferStorageFlags const flagSet)
    -> GLenum
{
    auto flags = static_cast<GLenum>(0);

    for (auto i = 0u; i < storageFlags.size(); ++i)
    {
        auto const flag = static_cast<BufferStorageFlags>(1 << i);

        if (isFlagSet(flag, flagSet))
        {
            flags |= storageFlags[i];
        }
    }

    return flags;
}

} // namespace glow
