#include <Ape/Rendering/OmniDepthShaderProgram.hpp>

#include <Ape/GpuResource/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto buildOmnidirectionalDepthShader()
    -> ShaderProgram
{
    auto const builder = ShaderBuilder{{resourceFolder "/shaders"}};

    return builder.buildProgram(
        "Depth/Omni.Depth.Vertex.glsl",
        "Depth/Omni.Depth.Geometry.glsl",
        "Depth/Omni.Depth.Fragment.glsl");
}

} // unnamed namespace

OmniDepthShaderProgram::OmniDepthShaderProgram()
    : ShaderProgram{buildOmnidirectionalDepthShader()}
    , modelTransformation{*this, "modelTransformation"}
    , lightTransformation{*this, "lightTransformation"}
    , lightPosition{*this, "lightPosition"}
{
}

} // namespace ape
