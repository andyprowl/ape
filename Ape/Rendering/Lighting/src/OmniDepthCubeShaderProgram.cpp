#include <Ape/Rendering/Lighting/OmniDepthCubeShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto buildOmnidirectionalDepthShader()
    -> glow::ShaderProgram
{
    auto const builder = glow::ShaderBuilder{{resourceFolder "/shaders"}};

    return builder.buildProgram(
        "Depth/Omni.Depth.Cube.Vertex.glsl",
        "Depth/Omni.Depth.Cube.Geometry.glsl",
        "Depth/Omni.Depth.Cube.Fragment.glsl");
}

} // unnamed namespace

OmniDepthCubeShaderProgram::OmniDepthCubeShaderProgram()
    : ShaderProgram{buildOmnidirectionalDepthShader()}
    , worldTransformation{*this, "worldTransformation"}
    , lightTransformation{*this, "lightTransformation"}
    , lightPosition{*this, "lightPosition"}
{
}

} // namespace ape
