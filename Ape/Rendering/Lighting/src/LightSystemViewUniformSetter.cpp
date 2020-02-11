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
}

auto LightSystemViewUniformSetter::flush()
    -> void
{
    auto const mappingFlags = GL_MAP_WRITE_BIT;

    auto const buffer = glMapNamedBufferRange(
        uniformBuffer.getId(),
        0,
        block->getSize(),
        mappingFlags);

    assert(buffer != nullptr);

    block->set(*lightSystemView, reinterpret_cast<std::byte *>(buffer));

    glUnmapNamedBuffer(uniformBuffer.getId());
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

    auto const storageFlags = glow::BufferStorageFlags::mapWrite;

    buffer.createStorage(nullptr, size, storageFlags);

    return buffer;
}

} // namespace ape
