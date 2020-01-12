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
        Ape_Rendering_Lighting_ResourceFolder "/shaders"}};

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
    , fogDensity{*this, "fogDensity", 0.0f}
    , fogColor{*this, "fogColor", glm::vec3{0.0f, 0.0f, 0.0f}}
{
}

} // namespace ape
