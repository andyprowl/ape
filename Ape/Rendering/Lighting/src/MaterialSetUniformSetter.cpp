#include <Ape/Rendering/Lighting/MaterialSetUniformSetter.hpp>

#include <Ape/Rendering/Lighting/MaterialSetUniformBlock.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
{

MaterialSetUniformSetter::MaterialSetUniformSetter(
    MaterialSet const & materials,
    MaterialSetUniformBlock & block)
    : materials{&materials}
    , block{&block}
    , uniformBuffer{makeUniformBuffer()}
    , mappedBuffer{mapUniformBuffer()}
    , dirty{true}
{
}

auto MaterialSetUniformSetter::flush()
    -> void
{
    if (not dirty)
    {
        return;
    }

    block->set(*materials, mappedBuffer);

    dirty = false;
}

auto MaterialSetUniformSetter::getUniformBuffer()
    -> glow::UniformBufferObject &
{
    return uniformBuffer;
}

auto MaterialSetUniformSetter::makeUniformBuffer() const
    -> glow::UniformBufferObject
{
    auto buffer = glow::UniformBufferObject{};

    auto const size = block->getSize();

    auto const storageFlags = 
        glow::BufferStorageFlags::mapWrite |
        glow::BufferStorageFlags::mapPersistent |
        glow::BufferStorageFlags::mapCoherent;

    buffer.createStorage(nullptr, size, storageFlags);

    return buffer;
}

auto MaterialSetUniformSetter::mapUniformBuffer() const
    -> std::byte *
{
    auto const mappingFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_UNSYNCHRONIZED_BIT;

    auto const bytes = glMapNamedBufferRange(
        uniformBuffer.getId(),
        0,
        block->getSize(),
        mappingFlags);

    assert(bytes != nullptr);

    return reinterpret_cast<std::byte *>(bytes);
}

} // namespace ape
