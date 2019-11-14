#include <Ape/Rendering/Skybox/PlainCubeVertex.hpp>

#include <Glow/BufferObject/VertexLayout.hpp>

namespace ape
{

namespace
{

auto getVertexLayout()
    -> glow::VertexLayout<1>
{
    return {encodeComponentLayout(PlainCubeVertex, position)};
}

} // unnamed namespace

} // namespace ape

namespace glow
{

template<>
auto sendVertexLayoutToGpu<ape::PlainCubeVertex>()
    -> void
{
    auto const layout = ape::getVertexLayout();

    sendVertexLayoutToGpu(layout);
}

} // namespace ape
