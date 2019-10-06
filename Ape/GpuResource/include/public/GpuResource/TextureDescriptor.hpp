#pragma once

#include <GpuResource/PixelType.hpp>
#include <GpuResource/TextureFormat.hpp>

#include <Mathematics/Size.hpp>

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
        std::byte * bytes)
        : size{size}
        , format{format}
        , pixelType{pixelType}
        , bytes{std::move(bytes)}
    {
    }

public:

    Size<int> size;

    TextureFormat format;

    PixelType pixelType;

    std::byte * bytes;

};

} // namespace ape
