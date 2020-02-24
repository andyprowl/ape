#include <Ape/Rendering/Skybox/SkyboxShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>
#include <Glow/Shader/ShaderBuilderStreamLogger.hpp>

#include <iostream>

namespace ape
{

namespace
{

auto buildSkyboxShader()
    -> glow::ShaderProgram
{
    auto logger = glow::logging::ShaderBuilderStreamLogger{std::cout};

    auto const builder = glow::ShaderBuilder{{
        resourceFolder "/shaders",
        resourceFolder_Ape_Rendering_Lighting "/shaders"},
        logger};

    return builder.buildProgram(
        std::vector<glow::VertexShaderPath>{"Skybox/Skybox.Vertex.glsl"},
        std::vector<glow::GeometryShaderPath>{},
        std::vector<glow::FragmentShaderPath>{
            "Skybox/Skybox.Fragment.glsl",
            "Fog/Fog.Fragment.glsl"},
        "Skybox");
}

} // unnamed namespace

SkyboxShaderProgram::SkyboxShaderProgram()
    : ShaderProgram{buildSkyboxShader()}
    , transform{*this, "transform"}
    , skybox{*this, "skybox", 0}
    , cameraPosition{*this, "camera.position"}
    , fog{*this, "fog"}
{
}

} // namespace ape
