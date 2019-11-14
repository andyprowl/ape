#pragma once

#include <Glow/Texture/TextureImage.hpp>
#include <Glow/Texture/TextureInternalFormat.hpp>
#include <Glow/Texture/TextureStorageType.hpp>
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
        TextureImage const & image,
        TextureInternalFormat const internalFormat,
        TextureWrapping const wrapping,
        TextureStorageType const storageType)
        : image{image}
        , internalFormat{internalFormat}
        , wrapping{wrapping}
        , storageType{storageType}
    {
    }

public:

    TextureImage image;

    TextureInternalFormat internalFormat;

    TextureWrapping wrapping;

    TextureStorageType storageType;

};

} // namespace glow
