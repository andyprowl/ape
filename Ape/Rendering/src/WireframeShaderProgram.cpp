#include <Rendering/WireframeShaderProgram.hpp>

#include <GpuResource/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto buildWireframeShader()
    -> ShaderProgram
{
    auto const builder = ShaderBuilder{{resourceFolder "/shaders"}};

    return builder.buildProgram(
        "Wireframe/Wire.Vertex.glsl",
        "Wireframe/Wire.Geometry.glsl",
        "Wireframe/Wire.Fragment.glsl");
}

} // unnamed namespace

WireframeShaderProgram::WireframeShaderProgram()
    : ShaderProgram{buildWireframeShader()}
    , transformation{*this, "transformation"}
    , lineColor{*this, "color"}
    , lineWidth{*this, "lineWidth"}
{
}

} // namespace ape
