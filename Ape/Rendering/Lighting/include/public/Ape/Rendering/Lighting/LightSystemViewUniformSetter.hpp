#pragma once

#include <Glow/BufferObject/UniformBufferObject.hpp>

namespace ape
{

class LightSystemView;
class LightSystemViewUniformBlock;

class LightSystemViewUniformSetter
{

public:

    LightSystemViewUniformSetter(LightSystemView & lightSystem, int size);

    auto flush(LightSystemViewUniformBlock & block)
        -> void;

    auto getUniformBuffer()
        -> glow::UniformBufferObject &;

private:

    auto makeUniformBuffer(int size) const
        -> glow::UniformBufferObject;

    auto mapUniformBuffer(int size) const
        -> std::byte *;

private:

    LightSystemView * lightSystemView;

    glow::UniformBufferObject uniformBuffer;

    std::byte * mappedBuffer;

};

} // namespace
