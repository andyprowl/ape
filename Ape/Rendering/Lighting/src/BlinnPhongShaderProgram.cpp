#include <Ape/Rendering/Lighting/BlinnPhongShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto buildBlinnPhongShader()
    -> glow::ShaderProgram
{
    auto const builder = glow::ShaderBuilder{{resourceFolder "/shaders"}};

    return builder.buildProgram(
        glow::VertexShaderPath{"BlinnPhong/BlinnPhong.Vertex.glsl"},
        glow::FragmentShaderPath{"BlinnPhong/BlinnPhong.Fragment.glsl"},
        std::string{"BlinnPhong"});
}

} // unnamed namespace

BlinnPhongShaderProgram::BlinnPhongShaderProgram()
    : ShaderProgram{buildBlinnPhongShader()}
    , worldTransformation{*this, "transform.model"}
    , cameraTransformation{*this, "transform.camera"}
    , normalTransformation{*this, "transform.normal"}
    , cameraPosition{*this, "camera.position"}
    , lightSystem{*this, "lightSystem"}
    , lightSystemView{*this, "lightSystemView"}
    , material{*this, "material", 0, 1, 2}
    , depthMapping{*this, "depthMapping", 3}
    , usePhongModel{*this, "usePhongModel", false}
    , usePercentageCloserFiltering{*this, "usePercentageCloserFiltering", false}
    , useNormalMapping{*this, "useNormalMapping", true}
{
}

} // namespace ape
