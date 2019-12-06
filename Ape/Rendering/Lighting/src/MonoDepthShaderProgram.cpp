#include <Ape/Rendering/Lighting/MonoDepthShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto buildDepthShader()
    -> glow::ShaderProgram
{
    auto const builder = glow::ShaderBuilder{{resourceFolder "/shaders"}};

    return builder.buildProgram(
        glow::VertexShaderPath{"Depth/Mono.Depth.Vertex.glsl"},
        glow::FragmentShaderPath{"Depth/Mono.Depth.Fragment.glsl"},
        "Mono.Depth");
}

} // unnamed namespace

MonoDepthShaderProgram::MonoDepthShaderProgram()
    : ShaderProgram{buildDepthShader()}
    , lightTransformation{*this, "lightTransformation"}
{
}

} // namespace ape
