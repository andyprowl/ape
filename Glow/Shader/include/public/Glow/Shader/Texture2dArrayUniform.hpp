#pragma once

#include <Glow/Shader/TextureUniform.hpp>

#include <Glow/Texture/Texture2dArray.hpp>

namespace glow
{

template<>
class Uniform<Texture2dArray> : public detail::TextureUniform<Texture2dArray>
{

public:
     
    using TextureUniform::TextureUniform;

};

} // namespace glow
