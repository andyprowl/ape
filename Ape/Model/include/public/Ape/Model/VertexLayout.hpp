#pragma once

#include <Model/VertexComponent.hpp>

#include <Meta/HomogenousVariadics.hpp>

#include <array>
#include <initializer_list>
#include <type_traits>

#define getComponentSizeInFloats(Type, component) \
    sizeof(decltype(std::declval<Type>().component)) / sizeof(float)

#define encodeComponentOffset(Type, component) offsetof(Type, component)

#define encodeComponentLayout(Type, component) VertexComponent{ \
    getComponentSizeInFloats(Type, component), \
    encodeComponentOffset(Type, component), \
    sizeof(Type)}

namespace ape
{

template<int NumOfComponents>
class VertexLayout
{

public:

    template<staticListOf(NumOfComponents, VertexComponent, Ts)>
    VertexLayout(Ts const & ... components)
        : components{components...}
    {
    }

    auto getComponent(const int index) const
        -> const VertexComponent &
    {
        return components[index];
    }

private:

    std::array<VertexComponent, static_cast<std::size_t>(NumOfComponents)> components;

};

auto sendVertexLayoutToGpu(std::initializer_list<VertexComponent> components)
    -> void;

} // namespace ape

namespace ape::detail
{

template<int NumOfComponents, int... Is>
auto sendVertexLayoutToGpu(
    VertexLayout<NumOfComponents> components,
    std::integer_sequence<int, Is...>)
    -> void
{
    ape::sendVertexLayoutToGpu({components.getComponent(Is)...});
}

} // namespace ape::detail

namespace ape
{

template<int NumOfComponents>
auto sendVertexLayoutToGpu(VertexLayout<NumOfComponents> components)
    -> void
{
    detail::sendVertexLayoutToGpu(components, std::make_integer_sequence<int, NumOfComponents>{});
}

// To be specialized individually for concrete vertex types.
template<typename VertexType>
auto sendVertexLayoutToGpu()
    -> void;

} // namespace ape
