#include <Ape/Wireframe/WireframeShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto buildWireframeShader()
    -> glow::ShaderProgram
{
    auto const builder = glow::ShaderBuilder{{resourceFolder "/shaders"}};

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
