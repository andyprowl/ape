#pragma once

#include <Glow/BufferObject/BufferObject.hpp>

namespace glow
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

} // namespace glow
