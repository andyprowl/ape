#pragma once

#include <Ape/GpuResource/ShaderProgram.hpp>
#include <Ape/GpuResource/TextureUniform.hpp>

namespace ape
{

class FlatQuadShaderProgram : public ShaderProgram
{

public:

    FlatQuadShaderProgram();

public:

    Uniform<Texture> source;

};

} // namespace ape
