#include <Ape/Rendering/FlatQuadVertex.hpp>

#include <Ape/GpuResource/VertexLayout.hpp>

namespace ape
{

auto getVertexLayout()
    -> VertexLayout<2>
{
    return {
        encodeComponentLayout(FlatQuadVertex, position),
        encodeComponentLayout(FlatQuadVertex, textureCoordinates)};
}

template<>
auto sendVertexLayoutToGpu<FlatQuadVertex>()
    -> void
{
    auto const layout = getVertexLayout();

    sendVertexLayoutToGpu(layout);
}

} // namespace ape
