#include <Ape/Rendering/Lighting/OmniDepthCubeShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto buildOmnidirectionalCubeDepthShader()
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
    : ShaderProgram{buildOmnidirectionalCubeDepthShader()}
    , worldTransformation{*this, "worldTransformation"}
    , lightTransformation{*this, "lightTransformation"}
    , lightPosition{*this, "lightPosition"}
{
}

} // namespace ape
