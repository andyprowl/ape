#pragma once

#include <Ape/Shader/GenericTextureUniform.hpp>
#include <Ape/Texture/CubeTexture.hpp>

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
