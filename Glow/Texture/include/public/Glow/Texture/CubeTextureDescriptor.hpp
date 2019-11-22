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
        CubeTextureImageSet imageSet,
        TextureInternalFormat const internalFormat,
        TextureWrapping const wrapping)
        : imageSet{std::move(imageSet)}
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
