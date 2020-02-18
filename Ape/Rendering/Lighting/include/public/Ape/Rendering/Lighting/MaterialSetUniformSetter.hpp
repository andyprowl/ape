#pragma once

#include <Glow/BufferObject/UniformBufferObject.hpp>

namespace ape
{

class Material;
class MaterialSetUniformBlock;

class MaterialSetUniformSetter
{

public:

    using MaterialSet = std::vector<Material *>;

public:

    MaterialSetUniformSetter(MaterialSet const & materials, MaterialSetUniformBlock & block);

    auto flush()
        -> void;

    auto getUniformBuffer()
        -> glow::UniformBufferObject &;

private:

    auto makeUniformBuffer() const
        -> glow::UniformBufferObject;

    auto mapUniformBuffer() const
        -> std::byte *;

private:

    MaterialSet const * materials;

    MaterialSetUniformBlock * block;

    glow::UniformBufferObject uniformBuffer;

    std::byte * mappedBuffer;

    bool dirty;

};

} // namespace
