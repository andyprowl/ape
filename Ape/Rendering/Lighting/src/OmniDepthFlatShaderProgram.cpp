#include <Ape/Rendering/Lighting/OmniDepthFlatShaderProgram.hpp>

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
