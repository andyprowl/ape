#include <Ape/Rendering/Lighting/LightSystemViewUniformSetter.hpp>

#include <Ape/Rendering/Lighting/LightSystemViewUniformBlock.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
{

LightSystemViewUniformSetter::LightSystemViewUniformSetter(
    LightSystemView & lightSystemView,
    int const size)
    : lightSystemView{&lightSystemView}
    , uniformBuffer{makeUniformBuffer(size)}
    , mappedBuffer{mapUniformBuffer(size)}
{
}

auto LightSystemViewUniformSetter::flush(LightSystemViewUniformBlock & block)
    -> void
{
    block.set(*lightSystemView, mappedBuffer);
}

auto LightSystemViewUniformSetter::getUniformBuffer()
    -> glow::UniformBufferObject &
{
    return uniformBuffer;
}

auto LightSystemViewUniformSetter::makeUniformBuffer(int const size) const
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

auto LightSystemViewUniformSetter::mapUniformBuffer(int const size) const
    -> std::byte *
{
    auto const mappingFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_UNSYNCHRONIZED_BIT;

    auto const bufferId = uniformBuffer.getId();

    auto const bytes = glMapNamedBufferRange(bufferId, 0, size, mappingFlags);

    assert(bytes != nullptr);

    return reinterpret_cast<std::byte *>(bytes);
}

} // namespace ape
