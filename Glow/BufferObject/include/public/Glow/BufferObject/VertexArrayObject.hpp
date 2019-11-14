#pragma once

#include <Glow/BufferObject/BufferObject.hpp>

namespace glow
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

} // namespace glow
