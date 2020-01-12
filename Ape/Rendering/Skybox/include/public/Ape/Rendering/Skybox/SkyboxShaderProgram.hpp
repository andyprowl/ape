#pragma once

#include <Ape/Rendering/Lighting/LightSystemUniform.hpp>

#include <Glow/Shader/CubeTextureUniform.hpp>
#include <Glow/Shader/ShaderProgram.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class SkyboxShaderProgram : public glow::ShaderProgram
{

public:

    SkyboxShaderProgram();

    glow::Uniform<ape::LightSystem> lightSystem;

    glow::Uniform<glm::mat4> transform;

    glow::Uniform<glow::CubeTexture> skybox;

    glow::Uniform<glm::vec3> cameraPosition;

    glow::Uniform<float> fogDensity;

    glow::Uniform<glm::vec3> fogColor;

};

} // namespace ape
