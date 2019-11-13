#include <Ape/Lighting/LightingShaderProgram.hpp>

#include <Ape/Shader/ShaderBuilder.hpp>

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

LightingShaderProgram::LightingShaderProgram()
    : ShaderProgram{buildStandardShader()}
    , modelTransformation{*this, "transform.model"}
    , cameraTransformation{*this, "transform.camera"}
    , normalTransformation{*this, "transform.normal"}
    , camera{*this, "camera"}
    , lightSystem{*this, "lightSystem"}
    , lightSystemView{*this, "lightSystemView"}
    , material{*this, "material", 0, 1}
    , depthMapping{*this, "depthMapping", 2}
    , useBlinnPhongModel{*this, "useBlinnPhongModel", true}
    , usePercentageCloserFiltering{*this, "usePercentageCloserFiltering", false}
{
}

} // namespace ape
