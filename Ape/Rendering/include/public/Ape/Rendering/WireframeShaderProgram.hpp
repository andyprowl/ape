#pragma once

#include <Ape/GpuResource/ShaderProgram.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace ape
{

class WireframeShaderProgram : public ShaderProgram
{

public:

    WireframeShaderProgram();

public:


    Uniform<glm::mat4> transformation;

    Uniform<glm::vec3> lineColor;

    Uniform<float> lineWidth;

};

} // namespace ape
