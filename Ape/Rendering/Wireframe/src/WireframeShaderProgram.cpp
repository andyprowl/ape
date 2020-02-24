#include <Ape/Rendering/Wireframe/WireframeShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>
#include <Glow/Shader/ShaderBuilderStreamLogger.hpp>

#include <iostream>

namespace ape
{

namespace
{

auto buildWireframeShader()
    -> glow::ShaderProgram
{
    auto logger = glow::logging::ShaderBuilderStreamLogger{std::cout};

    auto const builder = glow::ShaderBuilder{{resourceFolder "/shaders"}, logger};

    return builder.buildProgram(
        glow::VertexShaderPath{"Wireframe/Wire.Vertex.glsl"},
        glow::GeometryShaderPath{"Wireframe/Wire.Geometry.glsl"},
        glow::FragmentShaderPath{"Wireframe/Wire.Fragment.glsl"},
        "Wire");
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
