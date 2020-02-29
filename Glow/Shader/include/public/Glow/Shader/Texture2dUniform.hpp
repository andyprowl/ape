#pragma once

#include <Glow/Shader/TextureUniform.hpp>

#include <Glow/Texture/Texture2d.hpp>

namespace glow
{

template<>
class Uniform<Texture2d> : public detail::TextureUniform<Texture2d>
{

public:
     
    using TextureUniform::TextureUniform;

};

} // namespace glow
