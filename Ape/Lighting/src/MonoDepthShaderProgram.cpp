#include <Ape/Lighting/MonoDepthShaderProgram.hpp>

#include <Ape/Shader/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto buildDepthShader()
    -> ShaderProgram
{
    auto const builder = ShaderBuilder{{resourceFolder "/shaders"}};

    return builder.buildProgram("Depth/Mono.Depth.Vertex.glsl", "Depth/Mono.Depth.Fragment.glsl");
}

} // unnamed namespace

MonoDepthShaderProgram::MonoDepthShaderProgram()
    : ShaderProgram{buildDepthShader()}
    , lightTransformation{*this, "lightTransformation"}
{
}

} // namespace ape
