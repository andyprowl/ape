#pragma once

#include <Ape/Texture/TextureImage.hpp>
#include <Ape/Texture/TextureInternalFormat.hpp>
#include <Ape/Texture/TextureStorageType.hpp>
#include <Ape/Texture/TextureWrapping.hpp>

#include <Foundational/Mathematics/Size.hpp>

#include <cstddef>
#include <stdexcept>

namespace ape
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

} // namespace ape
