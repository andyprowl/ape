#pragma once

#include <Rendering/CameraUniform.hpp>
#include <Rendering/DepthMappingUniform.hpp>
#include <Rendering/LightingUniform.hpp>
#include <Rendering/LightingViewUniform.hpp>
#include <Rendering/MaterialUniform.hpp>

#include <GpuResource/ShaderProgram.hpp>

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

    Uniform<Lighting> lighting;

    Uniform<LightingView> lightingView;

    Uniform<Material> material;

    Uniform<DepthMapping> depthMapping;

    Uniform<bool> useBlinnPhongModel;

};

} // namespace ape
