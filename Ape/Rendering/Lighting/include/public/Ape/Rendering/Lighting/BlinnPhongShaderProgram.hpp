#pragma once

#include <Ape/Rendering/Lighting/DepthMappingUniform.hpp>
#include <Ape/Rendering/Lighting/FogUniform.hpp>
#include <Ape/Rendering/Lighting/LightSystemUniformBlock.hpp>
#include <Ape/Rendering/Lighting/LightSystemViewUniformBlock.hpp>
#include <Ape/Rendering/Lighting/MaterialMapsUniform.hpp>
#include <Ape/Rendering/Lighting/MaterialSetUniformBlock.hpp>

#include <Glow/Shader/ShaderPath.hpp>
#include <Glow/Shader/ShaderProgram.hpp>

namespace ape
{

class BlinnPhongShaderProgram : public glow::ShaderProgram
{

public:

    BlinnPhongShaderProgram(
        glow::VertexShaderPath const & vertexShader,
        std::vector<glow::FragmentShaderPath> const & fragmentShaders);

public:

    glow::Uniform<glm::mat4> objectToWorldTransformation;

    glow::Uniform<glm::mat4> worldToClipTransformation;

    glow::Uniform<glm::mat3> normalTransformation;

    glow::Uniform<glm::vec3> cameraPosition;

    LightSystemUniformBlock lightSystem;

    LightSystemViewUniformBlock lightSystemView;

    MaterialSetUniformBlock materialSet;

    MaterialMapsUniform materialMaps;

    glow::Uniform<int> activeMaterialIndex;

    glow::Uniform<Fog> fog;

    glow::Uniform<DepthMapping> depthMapping;

    glow::Uniform<bool> usePhongModel;

    glow::Uniform<bool> usePercentageCloserFiltering;

    glow::Uniform<bool> useNormalMapping;

};

} // namespace ape
