#include <Ape/World/Shape/ShapeVertex.hpp>

#include <Glow/BufferObject/VertexLayout.hpp>

namespace glow
{

template<>
auto getVertexLayout<ape::ShapeVertex>()
    -> glow::VertexLayout<4>
{
    return {
        encodeAttributeLayout(ape::ShapeVertex, position),
        encodeAttributeLayout(ape::ShapeVertex, normal),
        encodeAttributeLayout(ape::ShapeVertex, tangent),
        encodeAttributeLayout(ape::ShapeVertex, textureCoordinates)};
}

template<>
auto sendVertexLayoutToGpu<ape::ShapeVertex>()
    -> void
{
    auto const layout = getVertexLayout<ape::ShapeVertex>();

    sendVertexLayoutToGpu(layout);
}

} // namespace glow
