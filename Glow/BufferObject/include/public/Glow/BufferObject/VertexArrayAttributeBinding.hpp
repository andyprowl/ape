#pragma once

namespace glow
{

class VertexArrayObject;
class VertexBufferObject;
class VertexAttribute;

class VertexArrayAttributeBinding
{

public:

    VertexArrayAttributeBinding(VertexArrayObject const & parent, int bindingIndex);

    auto getBindingIndex() const
        -> int;

    auto getVertexArrayObject() const
        -> const VertexArrayObject &;

    auto setAttributeIndex(int index)
        -> void;

    auto setFormat(VertexAttribute const & format)
        -> void;

    auto setSource(VertexBufferObject const & sourceBuffer, int offsetFromStart, int stride)
        -> void;

private:

    VertexArrayObject const * parent;

    int bindingIndex;

};

} // namespace glow
