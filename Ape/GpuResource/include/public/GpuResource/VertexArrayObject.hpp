#pragma once

#include <GpuResource/BufferObject.hpp>

namespace ape
{

class VertexArrayObject : public BufferObject
{

public:

    VertexArrayObject();

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

};

} // namespace ape
