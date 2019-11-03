#pragma once

#include <Ape/GpuResource/CubeTextureUniform.hpp>
#include <Ape/GpuResource/ShaderProgram.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class SkyboxShaderProgram : public ShaderProgram
{

public:

    SkyboxShaderProgram();

    Uniform<glm::mat4> transform;

    Uniform<CubeTexture> skybox;

};

} // namespace ape
