#include <Ape/Model/ShapeVertex.hpp>

#include <Ape/Model/VertexLayout.hpp>

namespace ape
{

auto getVertexLayout()
    -> VertexLayout<3>
{
    return {
        encodeComponentLayout(ShapeVertex, position),
        encodeComponentLayout(ShapeVertex, normal),
        encodeComponentLayout(ShapeVertex, textureCoordinates)};
}

template<>
auto sendVertexLayoutToGpu<ShapeVertex>()
    -> void
{
    auto const layout = getVertexLayout();

    sendVertexLayoutToGpu(layout);
}

} // namespace ape
