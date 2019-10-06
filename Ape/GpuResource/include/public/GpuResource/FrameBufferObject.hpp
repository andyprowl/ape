#pragma once

#include <GpuResource/BufferObject.hpp>

namespace ape
{

class FrameBufferObject : public BufferObject
{

public:

    FrameBufferObject();

    auto bind() const
        -> void;

    auto unbind() const
        -> void;

    // WARNING: This is an expensive operation! Use with caution.
    auto isBound() const
        -> bool;

    auto isComplete(bool bind = true) const
        -> bool;

};

} // namespace ape
