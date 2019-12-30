#include <Ape/Rendering/Skybox/SkyboxShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto buildStandardShader()
    -> glow::ShaderProgram
{
    auto const builder = glow::ShaderBuilder{{resourceFolder "/shaders"}};

    return builder.buildProgram(
        glow::VertexShaderPath{"Skybox/Skybox.Vertex.glsl"},
        glow::FragmentShaderPath{"Skybox/Skybox.Fragment.glsl"},
        "Skybox");
}

} // unnamed namespace

SkyboxShaderProgram::SkyboxShaderProgram()
    : ShaderProgram{buildStandardShader()}
    , transform{*this, "transform"}
    , skybox{*this, "skybox", 0}
    , cameraHeight{*this, "cameraHeight"}
{
}

} // namespace ape
