#pragma once

#include <Glow/Shader/GenericTextureUniform.hpp>

#include <Glow/Texture/CubeTexture.hpp>

namespace glow
{

template<>
class Uniform<CubeTexture> : public detail::GenericTextureUniform<CubeTexture>
{

public:
     
    using GenericTextureUniform::GenericTextureUniform;

    using GenericTextureUniform::operator =;

};

} // namespace glow
