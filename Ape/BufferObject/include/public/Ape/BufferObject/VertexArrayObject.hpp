#pragma once

#include <Ape/BufferObject/BufferObject.hpp>

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
