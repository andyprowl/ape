#pragma once

#include <Ape/GpuResource/ShaderProgram.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class DepthShaderProgram : public ShaderProgram
{

public:

    DepthShaderProgram();

public:

    Uniform<glm::mat4> lightTransformation;

};

} // namespace ape
