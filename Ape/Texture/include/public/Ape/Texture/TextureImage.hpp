#pragma once

#include <Ape/Texture/PixelType.hpp>
#include <Ape/Texture/TextureImageFormat.hpp>

#include <Foundational/Mathematics/Size.hpp>

#include <cstddef>
#include <filesystem>
#include <stdexcept>

namespace ape
{

class CouldNotReadImageFile : public std::logic_error
{

public:

    explicit CouldNotReadImageFile(std::filesystem::path const & path)
        : logic_error{"Failed to read image file '" + path.string() + "'"}
    {
    }

};

class TextureImage
{

public:

    TextureImage(
        std::byte * const bytes,
        Size<int> const size,
        TextureImageFormat format,
        PixelType const pixelType)
        : bytes{bytes}
        , size{size}
        , format{format}
        , pixelType{pixelType}
    {
    }

public:

    std::byte * bytes;

    Size<int> const size;

    TextureImageFormat format;

    PixelType pixelType;

};

auto readImageFromFile(std::filesystem::path const & path, bool flipVertically)
    -> TextureImage;

} // namespace ape
