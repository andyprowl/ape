#pragma once

#include <Glow/Shader/GenericTextureUniform.hpp>

#include <Glow/Texture/Texture2d.hpp>

namespace glow
{

template<>
class Uniform<Texture2d> : public detail::GenericTextureUniform<Texture2d>
{

public:
     
    using GenericTextureUniform::GenericTextureUniform;

};

} // namespace glow
