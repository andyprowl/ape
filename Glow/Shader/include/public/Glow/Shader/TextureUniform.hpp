#pragma once

#include <Glow/Shader/GenericTextureUniform.hpp>

#include <Glow/Texture/Texture.hpp>

namespace glow
{

template<>
class Uniform<Texture> : public detail::GenericTextureUniform<Texture>
{

public:
     
    using GenericTextureUniform::GenericTextureUniform;

};

} // namespace glow
