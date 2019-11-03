#include <Ape/Model/ShapeVertex.hpp>

#include <Ape/GpuResource/VertexLayout.hpp>

namespace ape
{

namespace
{

auto getVertexLayout()
    -> VertexLayout<3>
{
    return {
        encodeComponentLayout(ShapeVertex, position),
        encodeComponentLayout(ShapeVertex, normal),
        encodeComponentLayout(ShapeVertex, textureCoordinates)};
}

} // unnamed namespace

template<>
auto sendVertexLayoutToGpu<ShapeVertex>()
    -> void
{
    auto const layout = getVertexLayout();

    sendVertexLayoutToGpu(layout);
}

} // namespace ape
