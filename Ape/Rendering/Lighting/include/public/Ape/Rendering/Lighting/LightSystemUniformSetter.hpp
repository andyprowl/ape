#pragma once

#include <Glow/BufferObject/UniformBufferObject.hpp>

namespace ape
{

class LightSystem;
class LightSystemUniformBlock;

class LightSystemUniformSetter
{

public:

    LightSystemUniformSetter(LightSystem & lightSystem, LightSystemUniformBlock & block);

    auto flush()
        -> void;

    auto getUniformBuffer()
        -> glow::UniformBufferObject &;

private:

    auto makeUniformBuffer() const
        -> glow::UniformBufferObject;

private:

    LightSystem * lightSystem;

    LightSystemUniformBlock * block;

    glow::UniformBufferObject uniformBuffer;

    std::byte * data;

};

} // namespace
