#include <Glow/Texture/TextureImage.hpp>

#include <Stb/stb_image.h>

#include <cassert>

namespace glow
{

namespace
{

auto asBytes(stbi_uc * const imageData)
    -> TextureImage::ImageBytesPtr
{
    return TextureImage::ImageBytesPtr{reinterpret_cast<std::byte *>(imageData)};
}

auto determineImageFormat(int const numOfChannels)
    -> TextureImageFormat
{
    switch (numOfChannels)
    {
        case 1:
        {
            return TextureImageFormat::r;
        }

        case 3:
        {
            return TextureImageFormat::rgb;
        }

        case 4:
        {
            return TextureImageFormat::rgba;
        }

        default:
        {
            assert(false);

            return TextureImageFormat::unknown;
        }
    }
}

} // unnamed namespace

auto TextureImage::ImageBytesDeleter::operator () (std::byte * const bytes) const
    -> void
{
    stbi_image_free(reinterpret_cast<void *>(bytes));
}

auto readImageFromFile(std::filesystem::path const & path, bool const flipVertically)
    -> TextureImage
{
    stbi_set_flip_vertically_on_load(flipVertically);

    auto size = basix::Size<int>{0, 0};

    auto numOfChannels = 0;

    auto const pathString = path.string();

    auto const data = stbi_load(pathString.c_str(), &size.width, &size.height, &numOfChannels, 0);

    if (data == nullptr)
    {
        throw CouldNotReadImageFile{path};
    }

    auto const imageFormat = determineImageFormat(numOfChannels);

    return {asBytes(data), size, imageFormat, PixelType::unsignedByte};
}

} // namespace glow
