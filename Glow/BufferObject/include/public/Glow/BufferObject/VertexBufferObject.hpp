#pragma once

#include <Glow/BufferObject/BufferObject.hpp>

namespace glow
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

} // namespace glow
