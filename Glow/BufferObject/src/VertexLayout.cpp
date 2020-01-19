#include <Glow/BufferObject/VertexLayout.hpp>

#include <glad/glad.h>

namespace glow
{

namespace
{

auto encodeAttributeOffset(VertexAttribute const & attribute)
    -> void const *
{
    auto const relativeOffset = static_cast<std::size_t>(attribute.relativeOffset);

    return reinterpret_cast<void const *>(relativeOffset);
}

} // unnamed namespace

auto sendLayoutAttributeToGpu(int const position, VertexAttribute const & attribute)
    -> void
{
    auto const type = convertToOpenGLDataType(attribute.componentType);

    auto const encodedOffset = encodeAttributeOffset(attribute);

    glVertexAttribPointer(
        position,
        attribute.numOfComponents,
        type,
        GL_FALSE,
        attribute.stride,
        encodedOffset);

    glEnableVertexAttribArray(position);
}

auto sendVertexLayoutToGpu(std::initializer_list<VertexAttribute> attributes)
    -> void
{
    auto index = 0;

    for (auto const & attribute : attributes)
    {
        sendLayoutAttributeToGpu(index, attribute);

        ++index;
    }
}

} // namespace glow
