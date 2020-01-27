#pragma once

#include <Glow/BufferObject/BufferObject.hpp>

namespace glow
{

class UniformBufferObject : public BufferObject
{

public:

    UniformBufferObject();

    auto bind(int bindingPoint) const
        -> void;

    auto bind(int bindingPoint, int offset, int size) const
        -> void;

};

} // namespace glow
