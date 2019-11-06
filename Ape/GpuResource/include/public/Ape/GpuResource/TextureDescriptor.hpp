#pragma once

#include <Ape/GpuResource/TextureImage.hpp>
#include <Ape/GpuResource/TextureInternalFormat.hpp>
#include <Ape/GpuResource/TextureStorageType.hpp>
#include <Ape/GpuResource/TextureWrapping.hpp>

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
