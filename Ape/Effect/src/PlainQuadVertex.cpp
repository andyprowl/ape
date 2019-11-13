#include <Ape/Effect/PlainQuadVertex.hpp>

#include <Ape/BufferObject/VertexLayout.hpp>

namespace ape
{

namespace
{

auto getVertexLayout()
    -> VertexLayout<2>
{
    return {
        encodeComponentLayout(PlainQuadVertex, position),
        encodeComponentLayout(PlainQuadVertex, textureCoordinates)};
}

} // unnamed namespace

template<>
auto sendVertexLayoutToGpu<PlainQuadVertex>()
    -> void
{
    auto const layout = getVertexLayout();

    sendVertexLayoutToGpu(layout);
}

} // namespace ape
