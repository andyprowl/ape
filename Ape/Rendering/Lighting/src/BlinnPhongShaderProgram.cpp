#include <Ape/Rendering/Lighting/BlinnPhongShaderProgram.hpp>

#include <Glow/Shader/ShaderBuilder.hpp>
#include <Glow/Shader/ShaderBuilderStreamLogger.hpp>

#include <iostream>

namespace ape
{

namespace
{

auto const lightSystemBlockBindingPoint = 0;

auto const lightSystemViewBlockBindingPoint = 1;

auto const materialSetBindingPoint = 2;

auto buildBlinnPhongShader(
    glow::VertexShaderPath const & vertexShader,
    std::vector<glow::FragmentShaderPath> const & fragmentShaders)
    -> glow::ShaderProgram
{
    auto logger = glow::logging::ShaderBuilderStreamLogger{std::cout};

    auto folder = vertexShader.parent_path();

    auto const builder = glow::ShaderBuilder{{std::move(folder)}, logger};

    auto const geometryShaders = std::vector<glow::GeometryShaderPath>{};

    return builder.buildProgram({vertexShader}, geometryShaders, fragmentShaders, "BlinnPhong");
}

} // unnamed namespace

BlinnPhongShaderProgram::BlinnPhongShaderProgram(
    glow::VertexShaderPath const & vertexShader,
    std::vector<glow::FragmentShaderPath> const & fragmentShaders)
    : ShaderProgram{buildBlinnPhongShader(vertexShader, fragmentShaders)}
    , objectToWorldTransformation{*this, "transform.objectToWorld"}
    , worldToClipTransformation{*this, "transform.worldToClip"}
    , normalTransformation{*this, "transform.normal"}
    , cameraPosition{*this, "camera.position"}
    , lightSystem{*this, "LightSystemUniformBlock"}
    , lightSystemView{*this, "LightSystemViewUniformBlock"}
    , materialSet{*this, "MaterialSetBlock"}
    , materialMaps{*this, "materialMaps", 0, 1, 2}
    , activeMaterialIndex{*this, "activeMaterialIndex"}
    , fog{*this, "fog"}
    , depthMapping{*this, "depthMapping", 3}
    , usePhongModel{*this, "usePhongModel", false}
    , usePercentageCloserFiltering{*this, "usePercentageCloserFiltering", false}
    , useNormalMapping{*this, "useNormalMapping", true}
{
    lightSystem.setBindingPoint(lightSystemBlockBindingPoint);

    lightSystemView.setBindingPoint(lightSystemViewBlockBindingPoint);

    materialSet.setBindingPoint(materialSetBindingPoint);
}

} // namespace ape
