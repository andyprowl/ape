#pragma once

#include <Ape/GpuResource/GenericTextureUniform.hpp>
#include <Ape/GpuResource/CubeTexture.hpp>

namespace ape
{

template<>
class Uniform<CubeTexture> : public detail::GenericTextureUniform<CubeTexture>
{

public:
     
    using GenericTextureUniform::GenericTextureUniform;

    using GenericTextureUniform::operator =;

};

} // namespace ape
