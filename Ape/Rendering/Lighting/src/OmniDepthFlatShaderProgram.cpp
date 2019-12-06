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
        glow::VertexShaderPath{"Depth/Omni.Depth.Flat.Vertex.glsl"},
        glow::FragmentShaderPath{"Depth/Omni.Depth.Flat.Fragment.glsl"},
        "Omni.Depth.Flat");
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
