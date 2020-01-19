#include <Glow/BufferObject/VertexLayout.hpp>

#include <glad/glad.h>

namespace glow
{

auto sendLayoutAttributeToGpu(int const position, VertexAttribute const & attribute)
    -> void
{
    auto const type = convertToOpenGLDataType(attribute.type);

    glVertexAttribPointer(
        position,
        attribute.numOfComponents,
        type,
        GL_FALSE,
        attribute.stride,
        reinterpret_cast<void const *>(attribute.relativeOffset));

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
