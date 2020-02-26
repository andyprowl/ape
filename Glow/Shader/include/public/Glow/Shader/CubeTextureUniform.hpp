#pragma once

#include <Glow/Shader/GenericTextureUniform.hpp>

#include <Glow/Texture/TextureCube.hpp>

namespace glow
{

template<>
class Uniform<TextureCube> : public detail::GenericTextureUniform<TextureCube>
{

public:
     
    using GenericTextureUniform::GenericTextureUniform;

    using GenericTextureUniform::operator =;

};

} // namespace glow
