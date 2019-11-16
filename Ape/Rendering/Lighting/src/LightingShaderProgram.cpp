#include <Ape/Rendering/Lighting/LightingShaderProgram.hpp>

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
        "BlinnPhong/BlinnPhong.Vertex.glsl",
        "BlinnPhong/BlinnPhong.Fragment.glsl");
}

} // unnamed namespace

LightingShaderProgram::LightingShaderProgram()
    : ShaderProgram{buildStandardShader()}
    , worldTransformation{*this, "transform.model"}
    , cameraTransformation{*this, "transform.camera"}
    , normalTransformation{*this, "transform.normal"}
    , camera{*this, "camera"}
    , lightSystem{*this, "lightSystem"}
    , lightSystemView{*this, "lightSystemView"}
    , material{*this, "material", 0, 1}
    , depthMapping{*this, "depthMapping", 2}
    , usePhongModel{*this, "usePhongModel", false}
    , usePercentageCloserFiltering{*this, "usePercentageCloserFiltering", false}
{
}

} // namespace ape
