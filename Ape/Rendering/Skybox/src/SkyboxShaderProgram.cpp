#include <Ape/Rendering/Skybox/SkyboxShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto buildStandardShader()
    -> glow::ShaderProgram
{
    auto const builder = glow::ShaderBuilder{{
        resourceFolder "/shaders",
        resourceFolder_Ape_Rendering_Lighting "/shaders"}};

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
    : ShaderProgram{buildStandardShader()}
    , lightSystem{*this, "lightSystem"}
    , transform{*this, "transform"}
    , skybox{*this, "skybox", 0}
    , cameraPosition{*this, "camera.position"}
    , fog{*this, "fog"}
{
}

} // namespace ape
