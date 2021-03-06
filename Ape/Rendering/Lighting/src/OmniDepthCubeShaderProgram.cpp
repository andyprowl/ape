#include <Ape/Rendering/Lighting/OmniDepthCubeShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>
#include <Glow/Shader/ShaderBuilderStreamLogger.hpp>

#include <iostream>

namespace ape
{

namespace
{

auto buildOmnidirectionalCubeDepthShader()
    -> glow::ShaderProgram
{
    auto logger = glow::logging::ShaderBuilderStreamLogger{std::cout};

    auto const builder = glow::ShaderBuilder{{resourceFolder "/shaders"}, logger};

    return builder.buildProgram(
        glow::VertexShaderPath{"Depth/Omni.Depth.Cube.Vertex.glsl"},
        glow::GeometryShaderPath{"Depth/Omni.Depth.Cube.Geometry.glsl"},
        glow::FragmentShaderPath{"Depth/Omni.Depth.Cube.Fragment.glsl"},
        "Omni.Depth.Cube");
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
