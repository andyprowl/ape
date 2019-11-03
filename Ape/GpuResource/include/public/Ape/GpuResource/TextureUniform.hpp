#pragma once

#include <Ape/GpuResource/GenericTextureUniform.hpp>
#include <Ape/GpuResource/Texture.hpp>

namespace ape
{

template<>
class Uniform<Texture> : public detail::GenericTextureUniform<Texture>
{

public:
     
    using GenericTextureUniform::GenericTextureUniform;

    using GenericTextureUniform::operator =;

};

} // namespace ape