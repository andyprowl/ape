#include <Ape/Rendering/Lighting/OmniDepthFlatShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto buildOmnidirectionalFlatDepthShader()
    -> glow::ShaderProgram
{
    auto const builder = glow::ShaderBuilder{{resourceFolder "/shaders"}};

    return builder.buildProgram(
        "Depth/Omni.Depth.Flat.Vertex.glsl",
        "Depth/Omni.Depth.Flat.Fragment.glsl");
}

} // unnamed namespace

OmniDepthFlatShaderProgram::OmniDepthFlatShaderProgram()
    : ShaderProgram{buildOmnidirectionalFlatDepthShader()}
    , worldTransformation{*this, "worldTransformation"}
    , lightTransformation{*this, "lightTransformation"}
    , lightPosition{*this, "lightPosition"}
{
}

} // namespace ape
