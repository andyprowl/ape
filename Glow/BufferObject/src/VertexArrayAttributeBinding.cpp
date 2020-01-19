#include <Glow/BufferObject/VertexArrayAttributeBinding.hpp>

#include <Glow/BufferObject/VertexArrayObject.hpp>
#include <Glow/BufferObject/VertexBufferObject.hpp>
#include <Glow/BufferObject/VertexAttribute.hpp>

#include <glad/glad.h>



namespace glow
{

VertexArrayAttributeBinding::VertexArrayAttributeBinding(
    VertexArrayObject const & parent,
    int const bindingIndex)
    : parent{&parent}
    , bindingIndex{bindingIndex}
{
}

auto VertexArrayAttributeBinding::getBindingIndex() const
    -> int
{
    return bindingIndex;
}

auto VertexArrayAttributeBinding::getVertexArrayObject() const
    -> const VertexArrayObject &
{
    return *parent;
}

auto VertexArrayAttributeBinding::setAttributeIndex(int const index)
    -> void
{
    auto const vaoId = parent->getId();

    glVertexArrayAttribBinding(vaoId, index, bindingIndex);
}

auto VertexArrayAttributeBinding::setFormat(VertexAttribute const & format)
    -> void
{
    auto const vaoId = parent->getId();

    auto const isNormalized = false;

    auto const type = convertToOpenGLDataType(format.type);

    glVertexArrayAttribFormat(
        vaoId,
        bindingIndex,
        format.numOfComponents,
        type,
        isNormalized,
        static_cast<GLuint>(format.relativeOffset));
}

auto VertexArrayAttributeBinding::setSource(
    VertexBufferObject const & sourceBuffer,
    int const offsetFromStart,
    int const stride)
    -> void
{
    auto const vaoId = parent->getId();

    const auto bufferId = sourceBuffer.getId();

    glVertexArrayVertexBuffer(vaoId, bindingIndex, bufferId, offsetFromStart, stride);
}

} // namespace glow
