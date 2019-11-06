#pragma once

#include <Ape/GpuResource/CubeTextureImageSet.hpp>
#include <Ape/GpuResource/TextureInternalFormat.hpp>
#include <Ape/GpuResource/TextureWrapping.hpp>

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
