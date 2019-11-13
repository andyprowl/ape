#pragma once

#include <Ape/Texture/CubeTextureImageSet.hpp>
#include <Ape/Texture/TextureInternalFormat.hpp>
#include <Ape/Texture/TextureWrapping.hpp>

namespace ape
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

} // namespace ape
