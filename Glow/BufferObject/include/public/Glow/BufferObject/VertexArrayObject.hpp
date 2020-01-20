#pragma once

#include <Glow/BufferObject/BufferObject.hpp>
#include <Glow/BufferObject/VertexArrayAttributeBinding.hpp>

namespace glow
{

class ElementBufferObject;

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

    auto setIndexSource(ElementBufferObject const & buffer)
        -> void;

    auto enableAttribute(int attributeIndex)
        -> void;

    auto disableAttribute(int attributeIndex)
        -> void;

};

} // namespace glow
