#include <Core/PickingShaderProgram.hpp>

namespace ape
{

PickingShaderProgram::PickingShaderProgram()
    : ShaderProgram{"Wire/Wire.Vertex.glsl", "Wire/Wire.Geometry.glsl", "Wire/Wire.Fragment.glsl"}
    , transformation{*this, "transformation"}
    , borderColor{*this, "color"}
    , lineWidth{*this, "lineWidth"}
{
}

} // namespace ape
