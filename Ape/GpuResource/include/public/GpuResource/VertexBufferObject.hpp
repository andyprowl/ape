#pragma once

#include <GpuResource/BufferObject.hpp>

namespace ape
{

class VertexBufferObject : public BufferObject
{

public:

    VertexBufferObject();

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

};

} // namespace ape