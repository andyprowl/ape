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
{
    auto const mappingFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT;

    auto const mappedBuffer = glMapNamedBufferRange(
        uniformBuffer.getId(),
        0,
        block.getSize(),
        mappingFlags);

    assert(mappedBuffer != nullptr);

    data = reinterpret_cast<std::byte *>(mappedBuffer);
}

auto LightSystemUniformSetter::flush()
    -> void
{
    block->set(*lightSystem, data);
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

} // namespace ape
