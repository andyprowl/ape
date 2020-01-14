#pragma once

#include <Glow/Texture/CubeTextureImageSet.hpp>
#include <Glow/Texture/TextureFiltering.hpp>
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
        TextureFiltering const filtering,
        TextureWrapping const wrapping,
        int const numOfMipmapLevels = 1)
        : imageSet{std::move(imageSet)}
        , internalFormat{internalFormat}
        , filtering{filtering}
        , wrapping{wrapping}
        , numOfMipmapLevels{numOfMipmapLevels}
    {
    }

public:

    CubeTextureImageSet imageSet;

    TextureInternalFormat internalFormat;

    TextureFiltering filtering;

    TextureWrapping wrapping;

    int const numOfMipmapLevels;

};

} // namespace glow
