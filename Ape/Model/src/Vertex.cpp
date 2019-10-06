#include <Model/Vertex.hpp>

#include <Model/VertexLayout.hpp>

namespace ape
{

auto getVertexLayout()
    -> VertexLayout<3>
{
    return {
        encodeComponentLayout(Vertex, position),
        encodeComponentLayout(Vertex, normal),
        encodeComponentLayout(Vertex, textureCoordinates)};
}

auto sendVertexLayoutToGpu()
    -> void
{
    auto const layout = getVertexLayout();

    sendVertexLayoutToGpu(layout);
}

} // namespace ape
