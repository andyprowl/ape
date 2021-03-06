#pragma once

#include <Glow/BufferObject/VertexAttribute.hpp>

#include <Basix/Meta/HomogenousVariadics.hpp>

#include <array>
#include <initializer_list>
#include <type_traits>

#define getAttributeType(Type, attribute) decltype(std::declval<Type>().attribute)

#define getAttributeBaseType(Type, attribute) \
    glow::BaseType<getAttributeType(Type, attribute)>

#define getAttributesize(Type, attribute) \
    sizeof(getAttributeType(Type, attribute)) / sizeof(getAttributeBaseType(Type, attribute))

#define encodeAttributeLayout(Type, attribute) glow::VertexAttribute{ \
    glow::asDataType<getAttributeBaseType(Type, attribute)>(), \
    static_cast<int>(getAttributesize(Type, attribute)), \
    static_cast<int>(offsetof(Type, attribute)), \
    sizeof(Type)}

namespace glow
{

template<int NumOfAttributes>
class VertexLayout
{

public:

    using Attributes = std::array<VertexAttribute, static_cast<std::size_t>(NumOfAttributes)>;

public:

    template<staticListOf(NumOfAttributes, VertexAttribute, Ts)>
    VertexLayout(Ts const & ... attributes)
        : attributes{attributes...}
    {
    }

    auto getAttributes() const
        -> Attributes const &
    {
        return attributes;
    }

    auto getAttribute(const int index) const
        -> VertexAttribute const &
    {
        return attributes[index];
    }

private:

    Attributes attributes;

};

auto sendVertexLayoutToGpu(std::initializer_list<VertexAttribute> attributes)
    -> void;

} // namespace glow

namespace glow::detail
{

template<int NumOfAttributes, int... Is>
auto sendVertexLayoutToGpu(
    VertexLayout<NumOfAttributes> attributes,
    std::integer_sequence<int, Is...>)
    -> void
{
    glow::sendVertexLayoutToGpu({attributes.getAttribute(Is)...});
}

} // namespace glow::detail

namespace glow
{

template<int NumOfAttributes>
auto sendVertexLayoutToGpu(VertexLayout<NumOfAttributes> attributes)
    -> void
{
    detail::sendVertexLayoutToGpu(attributes, std::make_integer_sequence<int, NumOfAttributes>{});
}

// To be specialized individually for concrete vertex types.
template<typename VertexType>
auto getVertexLayout()
    -> VertexLayout<VertexType::numOfComponents>;

// To be specialized individually for concrete vertex types.
template<typename VertexType>
auto sendVertexLayoutToGpu()
    -> void;

} // namespace glow
