#pragma once

#include <Ape/Shader/ShaderProgram.hpp>

#include <glm/mat4x4.hpp>

namespace ape
{

class MonoDepthShaderProgram : public ShaderProgram
{

public:

    MonoDepthShaderProgram();

public:

    Uniform<glm::mat4> lightTransformation;

};

} // namespace ape
