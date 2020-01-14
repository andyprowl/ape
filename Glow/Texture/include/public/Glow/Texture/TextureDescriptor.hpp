#pragma once

#include <Glow/Texture/TextureImage.hpp>
#include <Glow/Texture/TextureInternalFormat.hpp>
#include <Glow/Texture/TextureStorageType.hpp>
#include <Glow/Texture/TextureFiltering.hpp>
#include <Glow/Texture/TextureWrapping.hpp>

#include <Basix/Mathematics/Size.hpp>

#include <cstddef>
#include <stdexcept>

namespace glow
{

class TextureDescriptor
{

public:

    TextureDescriptor(
        TextureImage image,
        TextureInternalFormat const internalFormat,
        TextureFiltering const filtering,
        TextureWrapping const wrapping,
        TextureStorageType const storageType,
        int const numOfMipmapLevels = 1)
        : image{std::move(image)}
        , internalFormat{internalFormat}
        , filtering{filtering}
        , wrapping{wrapping}
        , storageType{storageType}
        , numOfMipmapLevels{numOfMipmapLevels}
    {
    }

public:

    TextureImage image;

    TextureInternalFormat internalFormat;

    TextureFiltering filtering;

    TextureWrapping wrapping;

    TextureStorageType storageType;

    int numOfMipmapLevels;

};

} // namespace glow
