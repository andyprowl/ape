#include <Ape/Rendering/Lighting/LightSystemViewUniformSetter.hpp>

#include <Ape/Rendering/Lighting/LightSystemViewUniformBlock.hpp>

#include <glad/glad.h>

#include <cassert>

namespace ape
{

LightSystemViewUniformSetter::LightSystemViewUniformSetter(
    LightSystemView & lightSystemView,
    LightSystemViewUniformBlock & block)
    : lightSystemView{&lightSystemView}
    , block{&block}
    , uniformBuffer{makeUniformBuffer()}
    , mappedBuffer{mapUniformBuffer()}
{
}

auto LightSystemViewUniformSetter::flush()
    -> void
{
    block->set(*lightSystemView, mappedBuffer);
}

auto LightSystemViewUniformSetter::getUniformBuffer()
    -> glow::UniformBufferObject &
{
    return uniformBuffer;
}

auto LightSystemViewUniformSetter::makeUniformBuffer() const
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

auto LightSystemViewUniformSetter::mapUniformBuffer() const
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
