#pragma once

#include <Ape/GpuResource/PixelType.hpp>
#include <Ape/GpuResource/TextureImageFormat.hpp>
#include <Ape/GpuResource/TextureInternalFormat.hpp>

#include <Foundational/Mathematics/Size.hpp>

#include <cstddef>
#include <stdexcept>
#include <string>

namespace ape
{

class TextureDescriptor
{

public:

    TextureDescriptor(
        Size<int> const size,
        TextureImageFormat imageFormat,
        TextureInternalFormat internalFormat,
        PixelType const pixelType,
        std::byte * const bytes)
        : size{size}
        , imageFormat{imageFormat}
        , internalFormat{internalFormat}
        , pixelType{pixelType}
        , bytes{bytes}
    {
    }

public:

    Size<int> size;

    TextureImageFormat imageFormat;

    TextureInternalFormat internalFormat;

    PixelType pixelType;

    std::byte * bytes;

};

} // namespace ape
