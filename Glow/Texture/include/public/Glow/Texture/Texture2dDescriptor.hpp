#pragma once

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
        basix::Size2d<int> const& size,
        TextureInternalFormat const internalFormat,
        TextureFiltering const filtering,
        TextureWrapping const wrapping,
        int const numOfMipmapLevels = 1)
        : size{size}
        , internalFormat{internalFormat}
        , filtering{filtering}
        , wrapping{wrapping}
        , numOfMipmapLevels{numOfMipmapLevels}
    {
    }

public:

    basix::Size2d<int> size;

    TextureInternalFormat internalFormat;

    TextureFiltering filtering;

    TextureWrapping wrapping;

    int numOfMipmapLevels;

};

} // namespace glow
