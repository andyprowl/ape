#pragma once

#include <Glow/Texture/CubeTextureImageSet.hpp>
#include <Glow/Texture/TextureInternalFormat.hpp>
#include <Glow/Texture/TextureWrapping.hpp>

namespace glow
{

class CubeTextureDescriptor
{

public:

    CubeTextureDescriptor(
        CubeTextureImageSet const & imageSet,
        TextureInternalFormat const internalFormat,
        TextureWrapping const wrapping)
        : imageSet{imageSet}
        , internalFormat{internalFormat}
        , wrapping{wrapping}
    {
    }

public:

    CubeTextureImageSet imageSet;

    TextureInternalFormat internalFormat;

    TextureWrapping wrapping;

};

} // namespace glow
