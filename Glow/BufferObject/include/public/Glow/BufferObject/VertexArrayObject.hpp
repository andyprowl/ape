#pragma once

#include <Glow/BufferObject/BufferObject.hpp>
#include <Glow/BufferObject/VertexArrayAttributeBinding.hpp>

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

    auto getBinding(int bindingIndex) const
        -> VertexArrayAttributeBinding;

};

} // namespace glow
