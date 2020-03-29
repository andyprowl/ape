#include <Ape/Rendering/Lighting/MaterialSetUniformSetter.hpp>

#include <Ape/Rendering/Lighting/MaterialSetUniformBlock.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
{

MaterialSetUniformSetter::MaterialSetUniformSetter(
    MaterialSet const & materials,
    int const size)
    : materials{&materials}
    , uniformBuffer{makeUniformBuffer(size)}
    , mappedBuffer{mapUniformBuffer(size)}
    , dirty{true}
{
}

auto MaterialSetUniformSetter::flush(MaterialSetUniformBlock & block)
    -> void
{
    if (not dirty)
    {
        return;
    }

    block.set(*materials, mappedBuffer);

    dirty = false;
}

auto MaterialSetUniformSetter::getUniformBuffer()
    -> glow::UniformBufferObject &
{
    return uniformBuffer;
}

auto MaterialSetUniformSetter::makeUniformBuffer(int const size) const
    -> glow::UniformBufferObject
{
    auto buffer = glow::UniformBufferObject{};

    auto const storageFlags = 
        glow::BufferStorageFlags::mapWrite |
        glow::BufferStorageFlags::mapPersistent |
        glow::BufferStorageFlags::mapCoherent;

    buffer.createStorage(nullptr, size, storageFlags);

    return buffer;
}

auto MaterialSetUniformSetter::mapUniformBuffer(int const size) const
    -> std::byte *
{
    auto const mappingFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_UNSYNCHRONIZED_BIT;

    auto const bufferId = uniformBuffer.getId();

    auto const bytes = glMapNamedBufferRange(bufferId, 0, size, mappingFlags);

    assert(bytes != nullptr);

    return reinterpret_cast<std::byte *>(bytes);
}

} // namespace ape
