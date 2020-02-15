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

auto LightSystemViewUniformSetter::flush()
    -> void
{
    block->set(*lightSystemView, data);
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

} // namespace ape
