#pragma once

#include <Ape/BufferObject.hpp>

namespace ape
{

class ElementBufferObject : public BufferObject
{

public:

    ElementBufferObject();

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

};

} // namespace ape
