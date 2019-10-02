#pragma once

#include <Core/ShaderProgram.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace ape
{

class PickingShaderProgram : public ShaderProgram
{

public:

    PickingShaderProgram();

public:

    Uniform<glm::mat4> transformation;

    Uniform<glm::vec3> borderColor;

};

} // namespace ape
