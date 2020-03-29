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

    MaterialSetUniformSetter(MaterialSet const & materials, int size);

    auto flush(MaterialSetUniformBlock & block)
        -> void;

    auto getUniformBuffer()
        -> glow::UniformBufferObject &;

private:

    auto makeUniformBuffer(int size) const
        -> glow::UniformBufferObject;

    auto mapUniformBuffer(int size) const
        -> std::byte *;

private:

    MaterialSet const * materials;

    glow::UniformBufferObject uniformBuffer;

    std::byte * mappedBuffer;

    bool dirty;

};

} // namespace
