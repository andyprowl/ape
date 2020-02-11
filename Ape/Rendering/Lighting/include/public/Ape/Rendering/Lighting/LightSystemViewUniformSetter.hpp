#pragma once

#include <Glow/BufferObject/UniformBufferObject.hpp>

namespace ape
{

class LightSystemView;
class LightSystemViewUniformBlock;

class LightSystemViewUniformSetter
{

public:

    LightSystemViewUniformSetter(
        LightSystemView & lightSystem,
        LightSystemViewUniformBlock & block);

    auto flush()
        -> void;

    auto getUniformBuffer()
        -> glow::UniformBufferObject &;

private:

    auto makeUniformBuffer() const
        -> glow::UniformBufferObject;

private:

    LightSystemView * lightSystemView;

    LightSystemViewUniformBlock * block;

    glow::UniformBufferObject uniformBuffer;

};

} // namespace
