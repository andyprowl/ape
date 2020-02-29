#pragma once

#include <Glow/Shader/TextureUniform.hpp>

#include <Glow/Texture/TextureCube.hpp>

namespace glow
{

template<>
class Uniform<TextureCube> : public detail::TextureUniform<TextureCube>
{

public:
     
    using TextureUniform::TextureUniform;

    using TextureUniform::operator =;

};

} // namespace glow
