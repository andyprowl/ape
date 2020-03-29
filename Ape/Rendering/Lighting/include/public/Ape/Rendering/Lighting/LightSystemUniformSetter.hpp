#pragma once

#include <Glow/BufferObject/UniformBufferObject.hpp>

namespace ape
{

class LightSystem;
class LightSystemUniformBlock;

class LightSystemUniformSetter
{

public:

    LightSystemUniformSetter(LightSystem & lightSystem, int size);

    auto flush(LightSystemUniformBlock & block)
        -> void;

    auto getUniformBuffer()
        -> glow::UniformBufferObject &;

private:

    auto makeUniformBuffer(int size) const
        -> glow::UniformBufferObject;

    auto mapUniformBuffer(int size) const
        -> std::byte *;

private:

    LightSystem * lightSystem;

    glow::UniformBufferObject uniformBuffer;

    std::byte * mappedBuffer;

};

} // namespace
