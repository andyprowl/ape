#pragma once

#include <Ape/Rendering/CameraUniform.hpp>
#include <Ape/Rendering/DepthMappingUniform.hpp>
#include <Ape/Rendering/LightSystemUniform.hpp>
#include <Ape/Rendering/LightSystemViewUniform.hpp>
#include <Ape/Rendering/MaterialUniform.hpp>

#include <Ape/GpuResource/ShaderProgram.hpp>

namespace ape
{

class StandardShaderProgram : public ShaderProgram
{

public:

    StandardShaderProgram();

public:

    Uniform<glm::mat4> modelTransformation;

    Uniform<glm::mat4> cameraTransformation;

    Uniform<glm::mat3> normalTransformation;

    Uniform<Camera> camera;

    Uniform<LightSystem> lightSystem;

    Uniform<LightSystemView> lightSystemView;

    Uniform<Material> material;

    Uniform<DepthMapping> depthMapping;

    Uniform<bool> useBlinnPhongModel;

    Uniform<bool> usePercentageCloserFiltering;

};

} // namespace ape
