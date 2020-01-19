#include <Ape/Rendering/Effect/PlainQuadVertex.hpp>

#include <Glow/BufferObject/VertexLayout.hpp>

namespace ape
{

namespace
{

auto getVertexLayout()
    -> glow::VertexLayout<2>
{
    return {
        encodeAttributeLayout(PlainQuadVertex, position),
        encodeAttributeLayout(PlainQuadVertex, textureCoordinates)};
}

} // unnamed namespace

} // namespace ape

namespace glow
{

template<>
auto sendVertexLayoutToGpu<ape::PlainQuadVertex>()
    -> void
{
    auto const layout = ape::getVertexLayout();

    sendVertexLayoutToGpu(layout);
}

} // namespace glow
