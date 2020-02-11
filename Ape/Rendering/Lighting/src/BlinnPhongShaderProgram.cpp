#include <Ape/Rendering/Lighting/BlinnPhongShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>

namespace ape
{

namespace
{

auto const lightSystemBlockBindingPoint = 0;

auto const lightSystemViewBlockBindingPoint = 1;

auto buildBlinnPhongShader()
    -> glow::ShaderProgram
{
    auto const builder = glow::ShaderBuilder{{resourceFolder "/shaders"}};

    return builder.buildProgram(
        std::vector<glow::VertexShaderPath>{"BlinnPhong/BlinnPhong.Vertex.glsl"},
        std::vector<glow::GeometryShaderPath>{},
        std::vector<glow::FragmentShaderPath>{
            "BlinnPhong/BlinnPhong.Fragment.glsl",
            "Fog/Fog.Fragment.glsl"},
        std::string{"BlinnPhong"});
}

} // unnamed namespace

BlinnPhongShaderProgram::BlinnPhongShaderProgram()
    : ShaderProgram{buildBlinnPhongShader()}
    , worldTransformation{*this, "transform.model"}
    , cameraTransformation{*this, "transform.camera"}
    , normalTransformation{*this, "transform.normal"}
    , cameraPosition{*this, "camera.position"}
    , lightSystem{*this, "LightSystemUniformBlock"}
    , lightSystemView{*this, "LightSystemViewUniformBlock"}
    , material{*this, "material", 0, 1, 2}
    , fog{*this, "fog"}
    , depthMapping{*this, "depthMapping", 3}
    , usePhongModel{*this, "usePhongModel", false}
    , usePercentageCloserFiltering{*this, "usePercentageCloserFiltering", false}
    , useNormalMapping{*this, "useNormalMapping", true}
{
    lightSystem.setBindingPoint(lightSystemBlockBindingPoint);

    lightSystemView.setBindingPoint(lightSystemViewBlockBindingPoint);
}

} // namespace ape
