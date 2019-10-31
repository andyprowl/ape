#pragma once

#include <Ape/GpuResource/PixelType.hpp>
#include <Ape/GpuResource/TextureFormat.hpp>

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
        std::byte * const bytes)
        : size{size}
        , format{format}
        , pixelType{pixelType}
        , bytes{bytes}
    {
    }

public:

    Size<int> size;

    TextureFormat format;

    PixelType pixelType;

    std::byte * bytes;

};

} // namespace ape
