#pragma once

#include <Glow/Shader/CubeTextureUniform.hpp>
#include <Glow/Shader/ShaderProgram.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class SkyboxShaderProgram : public glow::ShaderProgram
{

public:

    SkyboxShaderProgram();

    glow::Uniform<glm::mat4> transform;

    glow::Uniform<glow::CubeTexture> skybox;

    glow::Uniform<float> cameraHeight;

};

} // namespace ape
