#pragma once

#include <Glow/Shader/TextureUniform.hpp>

#include <Glow/Texture/TextureCubeArray.hpp>

namespace glow
{

template<>
class Uniform<TextureCubeArray> : public detail::TextureUniform<TextureCubeArray>
{

public:
     
    using TextureUniform::TextureUniform;

    using TextureUniform::operator =;

};

} // namespace glow
