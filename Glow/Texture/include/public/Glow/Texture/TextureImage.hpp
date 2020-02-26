#pragma once

#include <Glow/Texture/PixelType.hpp>
#include <Glow/Texture/TextureImageFormat.hpp>

#include <Basix/Mathematics/Size.hpp>

#include <cstddef>
#include <filesystem>
#include <memory>
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

    class ImageBytesDeleter
    {

    public:

        auto operator () (std::byte * const bytes) const
            -> void;

    };

    using ImageBytesPtr = std::unique_ptr<std::byte, ImageBytesDeleter>;

public:

    TextureImage(
        ImageBytesPtr bytes,
        basix::Size2d<int> const size,
        TextureImageFormat format,
        PixelType const pixelType)
        : bytes{std::move(bytes)}
        , size{size}
        , format{format}
        , pixelType{pixelType}
    {
    }

public:

    ImageBytesPtr bytes;

    basix::Size2d<int> const size;

    TextureImageFormat format;

    PixelType pixelType;

};

auto readImageFromFile(std::filesystem::path const & path, bool flipVertically)
    -> TextureImage;

} // namespace glow
