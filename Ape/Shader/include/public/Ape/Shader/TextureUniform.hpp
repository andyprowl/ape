#pragma once

#include <Ape/Shader/GenericTextureUniform.hpp>
#include <Ape/Texture/Texture.hpp>

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
