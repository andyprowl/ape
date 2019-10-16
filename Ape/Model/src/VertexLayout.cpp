#include <Ape/Model/VertexLayout.hpp>

#include <glad/glad.h>

namespace ape
{

auto sendLayoutComponentToGpu(int const position, VertexComponent const & component)
    -> void
{
    glVertexAttribPointer(
        position,
        component.sizeInFloats,
        GL_FLOAT,
        GL_FALSE,
        component.stride,
        component.encodedOffsetInType);

    glEnableVertexAttribArray(position);
}

auto sendVertexLayoutToGpu(std::initializer_list<VertexComponent> components)
    -> void
{
    auto index = 0;

    for (auto const & component : components)
    {
        sendLayoutComponentToGpu(index, component);

        ++index;
    }
}

} // namespace ape
