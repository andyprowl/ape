#include <Core/PickingShaderProgram.hpp>

namespace ape
{

PickingShaderProgram::PickingShaderProgram()
    : ShaderProgram{"Vertex.Plain.glsl", "Geometry.Plain.glsl", "Fragment.Plain.glsl"}
    , transformation{*this, "transformation"}
    , borderColor{*this, "color"}
{
}

} // namespace ape
