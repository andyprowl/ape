#include <Engine/StandardShaderProgram.hpp>

#include <Glow/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto buildStandardShader()
    -> ShaderProgram
{
    auto const builder = ShaderBuilder{{resourceFolder "/shaders"}};

    return builder.buildProgram(
        "Standard/Standard.Vertex.glsl",
        "Standard/Standard.Fragment.glsl");
}

} // unnamed namespace

StandardShaderProgram::StandardShaderProgram()
    : ShaderProgram{buildStandardShader()}
    , camera{*this, "camera"}
    , lighting{*this, "lighting"}
    , modelTransformation{*this, "transform.model"}
    , cameraTransformation{*this, "transform.camera"}
    , normalTransformation{*this, "transform.normal"}
    , materialAmbient{*this, "material.ambient"}
    , materialShininess{*this, "material.shininess"}
    , useBlinnPhongModel{*this, "useBlinnPhongModel", true}
{
    use();

    getUniform<int>("material.diffuse") = 0;

    getUniform<int>("material.specular") = 1;
}

} // namespace ape
