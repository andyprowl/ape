#include <Asset/Vertex.hpp>

#include <glad/glad.h>

namespace ape
{

namespace
{

class Component
{

public:

    Component(int const sizeInFloats, std::size_t const offsetInType)
        : sizeInFloats{sizeInFloats}
        , encodedOffsetInType{reinterpret_cast<void *>(offsetInType)}
    {
    }

public:

    int sizeInFloats;

    void * const encodedOffsetInType;

};

#define getComponentSizeInFloats(Type, component) \
    sizeof(decltype(std::declval<Type>().component)) / sizeof(float)

#define encodeComponentOffset(Type, component) offsetof(Type, component)

#define encodeComponentLayout(Type, component) Component{ \
    getComponentSizeInFloats(Type, component), \
    encodeComponentOffset(Type, component)}

auto setVertexAttribute(int const position, Component const & component)
    -> void
{
    constexpr auto const stride = sizeof(Vertex);

    glVertexAttribPointer(
        position,
        component.sizeInFloats,
        GL_FLOAT,
        GL_FALSE,
        stride,
        component.encodedOffsetInType);

    glEnableVertexAttribArray(position);
}

auto setVertexAttributeForComponents(std::initializer_list<Component> components)
    -> void
{
    auto index = 0;

    for (auto const & component : components)
    {
        setVertexAttribute(index, component);

        ++index;
    }
}

} // unnamed namespace

auto sendVertexLayoutToGpu()
    -> void
{
    setVertexAttributeForComponents({
        encodeComponentLayout(Vertex, position),
        encodeComponentLayout(Vertex, normal),
        encodeComponentLayout(Vertex, textureCoordinates)});
}

} // namespace ape
