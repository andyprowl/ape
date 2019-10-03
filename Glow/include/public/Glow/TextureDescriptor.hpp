#pragma once

#include <Glow/TextureFormat.hpp>

#include <Mathematics/Size.hpp>

#include <cstddef>
#include <stdexcept>
#include <string>

namespace ape
{

class TextureDescriptor
{

public:

    TextureDescriptor(Size<int> const size, TextureFormat format, std::byte * bytes)
        : size{size}
        , format{format}
        , bytes{std::move(bytes)}
    {
    }

public:

    Size<int> size;

    TextureFormat format;

    std::byte * bytes;

};

} // namespace ape
