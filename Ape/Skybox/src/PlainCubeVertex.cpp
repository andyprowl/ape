#include <Ape/Skybox/PlainCubeVertex.hpp>

#include <Ape/BufferObject/VertexLayout.hpp>

namespace ape
{

namespace
{

auto getVertexLayout()
    -> VertexLayout<1>
{
    return {encodeComponentLayout(PlainCubeVertex, position)};
}

} // unnamed namespace

template<>
auto sendVertexLayoutToGpu<PlainCubeVertex>()
    -> void
{
    auto const layout = getVertexLayout();

    sendVertexLayoutToGpu(layout);
}

} // namespace ape
