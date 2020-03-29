#include <Ape/Rendering/Lighting/LightSystemUniformSetter.hpp>

#include <Ape/Rendering/Lighting/LightSystemUniformBlock.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
{

LightSystemUniformSetter::LightSystemUniformSetter(LightSystem & lightSystem, int const size)
    : lightSystem{&lightSystem}
    , uniformBuffer{makeUniformBuffer(size)}
    , mappedBuffer{mapUniformBuffer(size)}
{
}

auto LightSystemUniformSetter::flush(LightSystemUniformBlock & block)
    -> void
{
    block.set(*lightSystem, mappedBuffer);
}

auto LightSystemUniformSetter::getUniformBuffer()
    -> glow::UniformBufferObject &
{
    return uniformBuffer;
}

auto LightSystemUniformSetter::makeUniformBuffer(int const size) const
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

auto LightSystemUniformSetter::mapUniformBuffer(int const size) const
    -> std::byte *
{
    auto const mappingFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_UNSYNCHRONIZED_BIT;

    auto const bufferId = uniformBuffer.getId();

    auto const bytes = glMapNamedBufferRange(bufferId, 0, size, mappingFlags);

    assert(bytes != nullptr);

    return reinterpret_cast<std::byte *>(bytes);
}

} // namespace ape
