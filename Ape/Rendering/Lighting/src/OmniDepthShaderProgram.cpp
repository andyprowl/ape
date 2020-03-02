#include <Ape/Rendering/Lighting/OmniDepthShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>
#include <Glow/Shader/ShaderBuilderStreamLogger.hpp>

#include <iostream>

namespace ape
{

namespace
{

auto buildOmnidirectionalFlatDepthShader()
    -> glow::ShaderProgram
{
    auto logger = glow::logging::ShaderBuilderStreamLogger{std::cout};

    auto const builder = glow::ShaderBuilder{{resourceFolder "/shaders"}, logger};

    return builder.buildProgram(
        glow::VertexShaderPath{"Depth/Omni.Depth.Vertex.glsl"},
        glow::FragmentShaderPath{"Depth/Omni.Depth.Fragment.glsl"},
        "Omni.Depth");
}

} // unnamed namespace

OmniDepthShaderProgram::OmniDepthShaderProgram()
    : ShaderProgram{buildOmnidirectionalFlatDepthShader()}
    , worldTransformation{*this, "worldTransformation"}
    , lightTransformation{*this, "lightTransformation"}
    , lightPosition{*this, "lightPosition"}
{
}

} // namespace ape
