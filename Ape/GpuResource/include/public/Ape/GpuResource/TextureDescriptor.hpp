#pragma once

#include <Ape/GpuResource/PixelType.hpp>
#include <Ape/GpuResource/TextureFormat.hpp>
#include <Ape/GpuResource/TextureWrapping.hpp>

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
        TextureFormat format,
        PixelType const pixelType,
        TextureWrapping const wrapping,
        std::byte * bytes)
        : size{size}
        , format{format}
        , pixelType{pixelType}
        , wrapping{wrapping}
        , bytes{std::move(bytes)}
    {
    }

public:

    Size<int> size;

    TextureFormat format;

    PixelType pixelType;

    TextureWrapping wrapping;

    std::byte * bytes;

};

} // namespace ape
