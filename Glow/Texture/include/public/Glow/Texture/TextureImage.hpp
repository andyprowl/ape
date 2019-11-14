#pragma once

#include <Glow/Texture/PixelType.hpp>
#include <Glow/Texture/TextureImageFormat.hpp>

#include <Basix/Mathematics/Size.hpp>

#include <cstddef>
#include <filesystem>
#include <stdexcept>

namespace glow
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
        basix::Size<int> const size,
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

    basix::Size<int> const size;

    TextureImageFormat format;

    PixelType pixelType;

};

auto readImageFromFile(std::filesystem::path const & path, bool flipVertically)
    -> TextureImage;

} // namespace glow
