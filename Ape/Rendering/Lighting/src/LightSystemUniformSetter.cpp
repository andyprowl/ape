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
}

auto LightSystemUniformSetter::flush()
    -> void
{
    auto const mappingFlags = GL_MAP_WRITE_BIT;

    auto const buffer = glMapNamedBufferRange(
        uniformBuffer.getId(),
        0,
        block->getSize(),
        mappingFlags);

    assert(buffer != nullptr);

    block->set(*lightSystem, reinterpret_cast<std::byte *>(buffer));

    glUnmapNamedBuffer(uniformBuffer.getId());
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

    auto const storageFlags = glow::BufferStorageFlags::mapWrite;

    buffer.createStorage(nullptr, size, storageFlags);

    return buffer;
}

} // namespace ape
