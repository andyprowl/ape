#pragma once

#include <Ape/Rendering/Lighting/DepthMappingUniform.hpp>
#include <Ape/Rendering/Lighting/FogUniform.hpp>
#include <Ape/Rendering/Lighting/LightSystemUniform.hpp>
#include <Ape/Rendering/Lighting/LightSystemViewUniform.hpp>
#include <Ape/Rendering/Lighting/MaterialUniform.hpp>

#include <Glow/Shader/ShaderProgram.hpp>

namespace ape
{

class BlinnPhongShaderProgram : public glow::ShaderProgram
{

public:

    BlinnPhongShaderProgram();

public:

    glow::Uniform<glm::mat4> worldTransformation;

    glow::Uniform<glm::mat4> cameraTransformation;

    glow::Uniform<glm::mat3> normalTransformation;

    glow::Uniform<glm::vec3> cameraPosition;

    glow::Uniform<LightSystem> lightSystem;

    glow::Uniform<LightSystemView> lightSystemView;

    glow::Uniform<Material> material;

    glow::Uniform<Fog> fog;

    glow::Uniform<DepthMapping> depthMapping;

    glow::Uniform<bool> usePhongModel;

    glow::Uniform<bool> usePercentageCloserFiltering;

    glow::Uniform<bool> useNormalMapping;

};

} // namespace ape
