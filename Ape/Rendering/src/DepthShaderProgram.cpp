#include <Rendering/DepthShaderProgram.hpp>

#include <GpuResource/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto buildDepthShader()
    -> ShaderProgram
{
    auto const builder = ShaderBuilder{{resourceFolder "/shaders"}};

    return builder.buildProgram("Depth/Depth.Vertex.glsl", "Depth/Depth.Fragment.glsl");
}

} // unnamed namespace

DepthShaderProgram::DepthShaderProgram()
    : ShaderProgram{buildDepthShader()}
    , lightTransformation{*this, "lightTransformation"}
{
}

} // namespace ape
