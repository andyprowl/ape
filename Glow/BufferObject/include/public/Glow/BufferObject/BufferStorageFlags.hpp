#pragma once

namespace glow
{

using GLenum = unsigned int;

enum class BufferStorageFlags
{
    none = 0,
    dynamicStorage = 1 << 0,
    mapRead = 1 << 1,
    mapWrite = 1 << 2,
    mapPersistent = 1 << 3,
    mapCoherent = 1 << 4,
    clientStorage = 1 << 5,
};

inline auto operator & (BufferStorageFlags const lhs, BufferStorageFlags rhs)
    -> BufferStorageFlags
{
    return static_cast<BufferStorageFlags>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

inline auto operator | (BufferStorageFlags const lhs, BufferStorageFlags rhs)
    -> BufferStorageFlags
{
    return static_cast<BufferStorageFlags>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

auto convertToOpenGLFlags(BufferStorageFlags flags)
    -> GLenum;

} // namespace glow
