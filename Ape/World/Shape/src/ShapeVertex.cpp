#include <Ape/World/Shape/ShapeVertex.hpp>

#include <Glow/BufferObject/VertexLayout.hpp>

namespace ape
{

namespace
{

auto getVertexLayout()
    -> glow::VertexLayout<4>
{
    return {
        encodeComponentLayout(ShapeVertex, position),
        encodeComponentLayout(ShapeVertex, normal),
        encodeComponentLayout(ShapeVertex, tangent),
        encodeComponentLayout(ShapeVertex, textureCoordinates)};
}

} // unnamed namespace

} // namespace ape

namespace glow
{

template<>
auto sendVertexLayoutToGpu<ape::ShapeVertex>()
    -> void
{
    auto const layout = ape::getVertexLayout();

    sendVertexLayoutToGpu(layout);
}

} // namespace glow