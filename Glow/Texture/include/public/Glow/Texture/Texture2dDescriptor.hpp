#pragma once

#include <Glow/Texture/TextureImage.hpp>
#include <Glow/Texture/TextureInternalFormat.hpp>
#include <Glow/Texture/TextureFiltering.hpp>
#include <Glow/Texture/TextureWrapping.hpp>

#include <Basix/Mathematics/Size.hpp>

#include <cstddef>
#include <stdexcept>

namespace glow
{

class Texture2dDescriptor
{

public:

    Texture2dDescriptor(
        TextureImage image,
        TextureInternalFormat const internalFormat,
        TextureFiltering const filtering,
        TextureWrapping const wrapping,
        int const numOfMipmapLevels = 1)
        : image{std::move(image)}
        , internalFormat{internalFormat}
        , filtering{filtering}
        , wrapping{wrapping}
        , numOfMipmapLevels{numOfMipmapLevels}
    {
    }

public:

    TextureImage image;

    TextureInternalFormat internalFormat;

    TextureFiltering filtering;

    TextureWrapping wrapping;

    int numOfMipmapLevels;

};

} // namespace glow
