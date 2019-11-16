#include <Ape/Rendering/Lighting/BlinnPhongShaderProgram.hpp>

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

BlinnPhongShaderProgram::BlinnPhongShaderProgram()
    : ShaderProgram{buildStandardShader()}
    , worldTransformation{*this, "transform.model"}
    , cameraTransformation{*this, "transform.camera"}
    , normalTransformation{*this, "transform.normal"}
    , cameraPosition{*this, "camera.position"}
    , lightSystem{*this, "lightSystem"}
    , lightSystemView{*this, "lightSystemView"}
    , material{*this, "material", 0, 1}
    , depthMapping{*this, "depthMapping", 2}
    , usePhongModel{*this, "usePhongModel", false}
    , usePercentageCloserFiltering{*this, "usePercentageCloserFiltering", false}
{
}

} // namespace ape
