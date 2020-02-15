#include <Ape/Rendering/Lighting/LightSystemUniformSetter.hpp>

#include <Ape/Rendering/Lighting/LightSystemUniformBlock.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
{

LightSystemUniformSetter::LightSystemUniformSetter(
    LightSystem & lightSystem,
    LightSystemUniformBlock & block)
    : lightSystem{&lightSystem}
    , block{&block}
    , uniformBuffer{makeUniformBuffer()}
    , mappedBuffer{mapUniformBuffer()}
{
}

auto LightSystemUniformSetter::flush()
    -> void
{
    block->set(*lightSystem, mappedBuffer);
}

auto LightSystemUniformSetter::getUniformBuffer()
    -> glow::UniformBufferObject &
{
    return uniformBuffer;
}

auto LightSystemUniformSetter::makeUniformBuffer() const
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

auto LightSystemUniformSetter::mapUniformBuffer() const
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
