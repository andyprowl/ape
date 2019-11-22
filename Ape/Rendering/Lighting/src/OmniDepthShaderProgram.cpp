#include <Ape/Rendering/Lighting/OmniDepthShaderProgram.hpp>

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
        "Depth/Omni.Depth.Vertex.glsl",
        "Depth/Omni.Depth.Geometry.glsl",
        "Depth/Omni.Depth.Fragment.glsl");
}

} // unnamed namespace

OmniDepthShaderProgram::OmniDepthShaderProgram()
    : ShaderProgram{buildOmnidirectionalDepthShader()}
    , worldTransformation{*this, "worldTransformation"}
    , lightTransformation{*this, "lightTransformation"}
    , lightPosition{*this, "lightPosition"}
{
}

} // namespace ape
